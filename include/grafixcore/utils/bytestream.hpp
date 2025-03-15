#pragma once
#include <vector>

namespace grafix
{

class ByteStream
{
public:
    template <typename T>
    void write(const T& value)
    {
        const auto* ptr = reinterpret_cast<const std::byte*>(&value);
        m_bytes.insert(m_bytes.end(), ptr, ptr + sizeof(T));
    }

    template <typename T>
    auto read() const -> T
    {
        T value;
        std::memcpy(&value, m_bytes.data() + m_offset, sizeof(T));
        m_offset += sizeof(T);
        return value;
    }

    template <typename T>
    auto read(T* dst, size_t count) const -> void
    {
        std::memcpy(dst, m_bytes.data() + m_offset, count * sizeof(T));
        m_offset += count * sizeof(T);
    }

    auto data() const -> const std::vector<std::byte>&
    {
        return m_bytes;
    }

    void clear()
    {
        m_bytes.clear();
        m_offset = 0;
    }

    void setOffset(size_t offset)
    {
        m_offset = offset;
    }

private:
    std::vector<std::byte> m_bytes;
    mutable size_t m_offset = 0;
};

}  // namespace grafix