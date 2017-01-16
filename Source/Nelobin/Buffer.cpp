///
/// \file   Buffer.cpp
///

#include <fstream>
#include <cstring>
#include <Nelobin/Buffer.hpp>

namespace Nelobin
{

    bool Buffer::canRead (const Size a_size)
    {
        return a_size + m_read <= m_bytes.size();
    }

    Buffer::Buffer () :
        m_read { 0 }
    {

    }

    Buffer::Buffer (const void *ap_data, const Size a_size) :
        m_read { 0 }
    {
        writeRaw(ap_data, a_size);
    }

    Buffer::~Buffer ()
    {
        m_bytes.clear();
    }

    Status Buffer::loadFromFile (const String &a_filename)
    {
        if (a_filename.empty() == true)
            return Status::NoName;

        // Load the file...
        std::fstream l_file { a_filename, std::ios::in |
                                          std::ios::binary |
                                          std::ios::ate };

        if (l_file.is_open() == false)
            return Status::FileLoadError;

        // Get the file's size.
        Size l_size = l_file.tellg();
        l_file.seekg(0);

        // Clear the buffer.
        m_bytes.clear();
        m_read = 0;

        // If the file is not empty, then load the bytes.
        if (l_size != 0)
        {
            m_bytes.resize(l_size);
            l_file.read(m_bytes.data(), l_size);
        }

        // Don't forget to close the file stream before we leave.
        l_file.close();
        return Status::OK;
    }

    Status Buffer::saveToFile (const String &a_filename)
    {
        // Check to see if the user specified a file.
        if (a_filename.empty() == true)
            return Status::NoName;

        // Open the file...
        std::fstream l_file { a_filename, std::ios::out |
                                          std::ios::binary };
        if (l_file.is_open() == false)
            return Status::FileSaveError;

        // Place the bytes into the file.
        l_file.write(m_bytes.data(), m_bytes.size());

        // Close the file and exit.
        l_file.close();
        return Status::OK;
    }

    void Buffer::clear ()
    {
        m_bytes.clear();
        m_read = 0;
    }

    Buffer& Buffer::writeRaw (const void *ap_data, const Size a_size)
    {
        // Check to see if the array is not null, and is populated.
        if (ap_data != nullptr && a_size > 0)
        {
            // Expand the buffer and emplace the new data.
            Size l_end = m_bytes.size();
            m_bytes.resize(l_end + a_size);
            std::memcpy(m_bytes.data() + l_end, ap_data, a_size);
        }

        return *this;
    }

    template <>
    Buffer& Buffer::writeData (const S8& a_data)
    {
        return writeRaw(&a_data, sizeof(a_data));
    }

    template <>
    Buffer& Buffer::writeData (const S16& a_data)
    {
        S16 l_swapped = swapBytes<S16>(a_data);
        return writeRaw(&l_swapped, sizeof(l_swapped));
    }

    template <>
    Buffer& Buffer::writeData (const S32& a_data)
    {
        S32 l_swapped = swapBytes<S32>(a_data);
        return writeRaw(&l_swapped, sizeof(l_swapped));
    }

    template <>
    Buffer& Buffer::writeData (const U8& a_data)
    {
        return writeRaw(&a_data, sizeof(a_data));
    }

    template <>
    Buffer& Buffer::writeData (const U16& a_data)
    {
        U16 l_swapped = swapBytes<U16>(a_data);
        return writeRaw(&l_swapped, sizeof(l_swapped));
    }

    template <>
    Buffer& Buffer::writeData (const U32& a_data)
    {
        U32 l_swapped = swapBytes<U32>(a_data);
        return writeRaw(&l_swapped, sizeof(l_swapped));
    }

    template <>
    Buffer& Buffer::writeData (const Float& a_data)
    {
        U32 l_encoded = encodeFloat(a_data);
        return writeRaw(&l_encoded, sizeof(l_encoded));
    }

    template <>
    Buffer& Buffer::writeData (const String& a_data)
    {
        // First, write the string's length.
        U32 l_length = static_cast<U32>(a_data.size());
        writeData<U32>(l_length);

        // If the string is not empty, then write it.
        if (l_length != 0)
        {
            writeRaw(a_data.c_str(), l_length);
        }

        return *this;
    }

    Buffer& Buffer::readRaw (void *ap_data, const Size a_size)
    {
        if (ap_data != nullptr && a_size > 0 && canRead(a_size) == true)
        {
            std::memcpy(ap_data, m_bytes.data() + m_read, a_size);
            m_read += a_size;
        }

        return *this;
    }

    template <>
    Buffer& Buffer::readData (S8& a_data)
    {
        if (canRead(sizeof(a_data)) == true)
        {
            a_data = *reinterpret_cast<S8*>(m_bytes.data() + m_read);
            m_read += sizeof(a_data);
        }

        return *this;
    }

    template <>
    Buffer& Buffer::readData (S16& a_data)
    {
        if (canRead(sizeof(a_data)) == true)
        {
            a_data = swapBytes<S16>(
                *reinterpret_cast<S16*>(m_bytes.data() + m_read)
            );

            m_read += sizeof(a_data);
        }

        return *this;
    }

    template <>
    Buffer& Buffer::readData (S32& a_data)
    {
        if (canRead(sizeof(a_data)) == true)
        {
            a_data = swapBytes<S32>(
                *reinterpret_cast<S32*>(m_bytes.data() + m_read)
            );

            m_read += sizeof(a_data);
        }

        return *this;
    }

    template <>
    Buffer& Buffer::readData (U8& a_data)
    {
        if (canRead(sizeof(a_data)) == true)
        {
            a_data = *reinterpret_cast<U8*>(m_bytes.data() + m_read);
            m_read += sizeof(a_data);
        }

        return *this;
    }

    template <>
    Buffer& Buffer::readData (U16& a_data)
    {
        if (canRead(sizeof(a_data)) == true)
        {
            a_data = swapBytes<U16>(
                *reinterpret_cast<U16*>(m_bytes.data() + m_read)
            );

            m_read += sizeof(a_data);
        }

        return *this;
    }

    template <>
    Buffer& Buffer::readData (U32& a_data)
    {
        if (canRead(sizeof(a_data)) == true)
        {
            a_data = swapBytes<U32>(
                *reinterpret_cast<U32*>(m_bytes.data() + m_read)
            );

            m_read += sizeof(a_data);
        }

        return *this;
    }

    template <>
    Buffer& Buffer::readData (Float& a_data)
    {
        if (canRead(sizeof(a_data)) == true)
        {
            a_data = decodeFloat(
                *reinterpret_cast<U32*>(m_bytes.data() + m_read)
            );

            m_read += sizeof(a_data);
        }

        return *this;
    }

    template <>
    Buffer& Buffer::readData (String& a_data)
    {
        // Clear the string.
        a_data.clear();

        // Get the string's length.
        U32 l_length = 0;
        readData<U32>(l_length);

        // If the string is not empty, then read it.
        if (l_length > 0)
        {
            a_data.append(
                m_bytes.data() + m_read, l_length
            );

            m_read += l_length;
        }

        return *this;
    }

    const char* Buffer::getData () const
    {
        return (m_bytes.empty()) ? nullptr : m_bytes.data();
    }

    const Size Buffer::getSize () const
    {
        return m_bytes.size();
    }

    bool Buffer::isEmpty () const
    {
        return m_bytes.empty();
    }

}
