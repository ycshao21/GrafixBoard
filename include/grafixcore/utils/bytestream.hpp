#pragma once
#include <vector>
#include <cstring>

namespace grafix
{

class ByteStream
{
public:
    template <typename T>
        requires std::is_trivially_copyable_v<T>
    void write(const T& value)
    {
        const auto* ptr = reinterpret_cast<const std::byte*>(&value);
        m_bytes.insert(m_bytes.end(), ptr, ptr + sizeof(T));
    }

    template <typename T>
        requires std::is_trivially_copyable_v<T>
    void write(const T* src, size_t count)
    {
        const auto* ptr = reinterpret_cast<const std::byte*>(src);
        m_bytes.insert(m_bytes.end(), ptr, ptr + count * sizeof(T));
    }

    template <typename T>
        requires std::is_trivially_copyable_v<T>
    auto read() const -> T
    {
        T value;
        std::memcpy(&value, m_bytes.data() + m_offset, sizeof(T));
        m_offset += sizeof(T);
        return value;
    }

    template <typename T>
        requires std::is_trivially_copyable_v<T>
    auto read(T* dst, size_t count) const -> void
    {
        std::memcpy(dst, m_bytes.data() + m_offset, count * sizeof(T));
        m_offset += count * (sizeof(T) / sizeof(std::byte));
    }

    [[nodiscard]] auto data() const noexcept -> const std::vector<std::byte>&
    {
        return m_bytes;
    }

    void clear() noexcept
    {
        m_bytes.clear();
        m_offset = 0;
    }

    void setOffset(size_t offset) const noexcept
    {
        m_offset = offset;
    }

    [[nodiscard]] auto getOffset() const noexcept -> size_t
    {
        return m_offset;
    }

    void resetOffset() const noexcept
    {
        m_offset = 0;
    }

private:
    std::vector<std::byte> m_bytes;
    mutable size_t m_offset = 0;
};

}  // namespace grafix