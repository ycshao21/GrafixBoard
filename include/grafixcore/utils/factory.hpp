#include <memory>
#include <shared_mutex>

namespace grafix
{

template <bool THREAD_SAFE, typename KeyT, typename ValT,
          typename MapT = std::unordered_map<KeyT, ValT>>
class Registry
{
public:
    explicit Registry() = default;
    auto operator=(const Registry&) -> Registry& = delete;

    void add(const KeyT& key, const ValT& val)
    {
        if constexpr (THREAD_SAFE) {
            std::unique_lock<std::shared_mutex> lock(m_mutex);
        }
        m_mapper.emplace(key, val);
    }

    void remove(const KeyT& key)
    {
        if constexpr (THREAD_SAFE) {
            std::unique_lock<std::shared_mutex> lock(m_mutex);
        }
        m_mapper.erase(key);
    }

    auto get(const KeyT& key) const -> ValT
    {
        if constexpr (THREAD_SAFE) {
            std::shared_lock<std::shared_mutex> lock(m_mutex);
        }
        auto it = m_mapper.find(key);
        if (it != m_mapper.end()) {
            return it->second;
        }
        throw std::runtime_error("Key not found.");
    }

private:
    std::conditional_t<THREAD_SAFE, std::shared_mutex, std::monostate> m_mutex;
    MapT m_mapper;
};

template <bool THREAD_SAFE, typename KeyT,
          typename ValT = std::function<std::shared_ptr<void>()>,
          typename MapT = std::unordered_map<KeyT, ValT>>
class Factory : public Registry<THREAD_SAFE, KeyT, ValT, MapT>
{
public:
    using BaseT = Registry<THREAD_SAFE, KeyT, ValT, MapT>;

    explicit Factory() = default;
    auto operator=(const Factory&) -> Factory& = delete;

    template <typename T>
    void add(const KeyT& key)
    {
        this->BaseT::add(key, []() -> std::shared_ptr<void> {
            return std::make_shared<T>();
        });
    }

    template <typename ObjT = void>
    auto create(const KeyT& key)
    {
        auto creator = this->BaseT::get(key);
        auto ptr = std::static_pointer_cast<ObjT>(creator());
        if (!ptr) {
            throw std::bad_cast();
        }
        return ptr;
    }
};
}  // namespace grafix