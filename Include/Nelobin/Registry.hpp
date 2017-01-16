///
/// \file   Registry.hpp
/// \brief  Contains keys and entries which can be saved to file.
///

#pragma once

#include <Nelobin/Key.hpp>
#include <Nelobin/Export.hpp>

namespace Nelobin
{

    ///
    /// \class  Registry
    /// \brief  A collection of keys and entries.
    ///
    /// This collection can be saved to and loaded from a file.
    ///
    class _NELOBIN_API Registry : public Key
    {
    public:
        ///
        /// \brief  The default constructor.
        ///
        Registry ();

        ///
        /// \brief  The destructor.
        ///
        ~Registry ();

    public:
        ///
        /// \fn     loadFromBuffer
        /// \brief  Loads registry keys and entires from the given byte buffer.
        ///
        /// \param  a_buffer        The byte buffer.
        /// \param  a_append        Optional. Should we append the new entries?
        ///
        /// \return A status code.
        ///
        Status loadFromBuffer (Buffer& a_buffer,
                               const Boolean a_append = false);

        ///
        /// \fn     loadFromFile
        /// \brief  Loads registry keys and entries from the given file.
        ///
        /// \param  a_filename      The name of the file to load.
        /// \param  a_append        Optional. Should we append the new entries?
        ///
        /// \return A status code.
        ///
        Status loadFromFile (const String& a_filename,
                             const Boolean a_append = false);

        ///
        /// \fn     saveToBuffer
        /// \brief  Saves the registry to the given byte buffer.
        ///
        /// \param  a_buffer        The buffer to save to.
        ///
        /// \return A status code.
        ///
        Status saveToBuffer (Buffer& a_buffer);

        ///
        /// \fn     saveToFile
        /// \brief  Saves the registry to the given file.
        ///
        /// \param  a_filename      The name of the file.
        ///
        /// \return A status code.
        ///
        Status saveToFile (const String& a_filename);

    };

}
