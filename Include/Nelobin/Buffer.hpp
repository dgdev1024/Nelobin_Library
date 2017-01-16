///
/// \file   Buffer.hpp
/// \brief  Interface for loading, reading, writing, and saving bytes.
///

#pragma once

#include <Nelobin/Error.hpp>
#include <Nelobin/Types.hpp>
#include <Nelobin/Order.hpp>

namespace Nelobin
{

    ///
    /// \class  Buffer
    /// \brief  Interface for loading and saving byte arrays.
    ///
    class _NELOBIN_API Buffer
    {
    private:
        ByteArray   m_bytes;        ///< The array of bytes.
        Size        m_read;         ///< The position of the read cursor.

    private:
        ///
        /// \fn     canRead
        /// \brief  Checks to see if the given size of data can be read.
        ///
        /// This method checks to see if there is enough room in the byte
        /// array, starting from the current read position, to read a certain
        /// piece of data.
        ///
        /// The size given is usually the return value of the sizeof operator.
        ///
        /// \param  a_size          The size to check.
        ///
        /// \return True if we can read the data.
        ///
        bool canRead (const Size a_size);

    public:
        ///
        /// \brief  The default constructor.
        ///
        Buffer ();

        ///
        /// \brief  Constructs from a raw array of bytes.
        ///
        /// \param  ap_data         A pointer to the raw data.
        /// \param  a_size          The size of the byte array.
        ///
        Buffer (const void* ap_data,
                const Size a_size);

        ///
        /// \brief  The destructor.
        ///
        ~Buffer ();

    public:
        ///
        /// \fn     loadFromFile
        /// \brief  Loads bytes from the given file.
        ///
        /// \param  a_filename      The name of the file.
        ///
        /// \return A status code.
        ///
        Status loadFromFile (const String& a_filename);

        ///
        /// \fn     saveToFile
        /// \brief  Saves bytes to the given file.
        ///
        /// \param  a_filename      The name of the file.
        ///
        /// \return True if the bytes are saved successfully.
        ///
        Status saveToFile (const String& a_filename);

        ///
        /// \fn     clear
        /// \brief  Clears the contents of the byte array.
        ///
        void clear ();

        ///
        /// \fn     writeRaw
        /// \brief  Writes raw data to the byte array.
        ///
        /// \param  ap_data         A pointer to the bytes to be written.
        /// \param  a_size          The size of the array of bytes.
        ///
        /// \return This byte buffer.
        ///
        Buffer& writeRaw (const void* ap_data,
                          const Size a_size);

        ///
        /// \fn     writeData
        /// \brief  Writes a chunk of data to the byte array.
        ///
        /// \param  a_data          The data to be written.
        ///
        /// \return This byte buffer.
        ///
        template <typename T>
        Buffer& writeData (const T& a_data);

        ///
        /// \fn     readRaw
        /// \brief  Reads raw data from the byte array.
        ///
        /// \param  ap_data         Filled with the data read.
        /// \param  a_size          The size of the byte array.
        ///
        /// \return This byte buffer.
        ///
        Buffer& readRaw (void* ap_data,
                         const Size a_size);

        ///
        /// \fn     readData
        /// \brief  Reads a chunk of data from the byte array.
        ///
        /// \param  a_data          Filled with the data chunk
        ///
        /// \return This byte buffer.
        ///
        template <typename T>
        Buffer& readData (T& a_data);

    public:
        const char* getData () const;
        const Size getSize () const;
        bool isEmpty () const;

    };

}
