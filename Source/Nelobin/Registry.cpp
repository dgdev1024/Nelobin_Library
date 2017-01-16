///
/// \file   Registry.cpp
///

#include <Nelobin/Registry.hpp>

namespace Nelobin
{

    Registry::Registry () :
        Key {}
    {

    }

    Registry::~Registry ()
    {
        m_keys.clear();
        m_entries.clear();
    }

    Status Registry::loadFromBuffer (Buffer &a_buffer, const Boolean a_append)
    {
        // If the user does not want to append this registry, then
        // clear all other entries in here.
        if (a_append == false)
        {
            m_entries.clear();
            m_keys.clear();
        }

        // Get the number of entities expected.
        U32 l_entityCount = 0;
        a_buffer.readData<U32>(l_entityCount);

        // Now parse through the expected entities.
        for (U32 i = 0; i < l_entityCount; ++i)
        {
            if (deserializeNext(a_buffer) == false)
                return Status::FileParseError;
        }

        return Status::OK;
    }

    Status Registry::loadFromFile (const String &a_filename, const Boolean a_append)
    {
        // Don't do anything if there is no filename specified.
        if (a_filename.empty() == true)
        {
            _Nelobin_Error("Registry::loadFromFile(): No filename specified.");
            return Status::NoName;
        }

        // Attempt to load the file into the buffer.
        Buffer l_buffer;
        if (l_buffer.loadFromFile(a_filename) != Status::OK)
        {
            _Nelobin_Error("Registry::loadFromFile(): Error loading \"" << a_filename << "\" into buffer!");
            return Status::FileLoadError;
        }

        // Check the start of the file for a valid file header.
        U32 l_fileHeader = 0;
        U16 l_fileVersion = 0;

        l_buffer.readData<U32>(l_fileHeader)
                .readData<U16>(l_fileVersion);

        // Check to see if the header and version found are valid.
        if (l_fileHeader != G_REGISTRY_HEADER)
        {
            _Nelobin_Error("Registry::loadFromFile(): Error loading \"" << a_filename << "\" - Invalid file header.");
            return Status::FileParseError;
        }
        else if (l_fileVersion != G_VERSION)
        {
            _Nelobin_Error("Registry::loadFromFile(): Error loading \"" << a_filename << "\" - Version mismatch.");
            return Status::FileParseError;
        }

        // Now load our keys and entries from the buffer.
        if (loadFromBuffer(l_buffer, a_append) != Status::OK)
        {
            _Nelobin_Error("Registry::loadFromFile(): Error parsing registry file \"" << a_filename << "\".");
            return Status::FileParseError;
        }

        return Status::OK;
    }

    Status Registry::saveToBuffer (Buffer &a_buffer)
    {
        serialize(a_buffer);
        return Status::OK;
    }

    Status Registry::saveToFile (const String &a_filename)
    {
        // Don't do anything if there is no filename specified.
        if (a_filename.empty() == true)
        {
            _Nelobin_Error("Registry::saveFromFile(): No filename specified.");
            return Status::NoName;
        }

        // Create the buffer and write the file header.
        Buffer l_buffer;
        l_buffer.writeData<U32>(G_REGISTRY_HEADER)
                .writeData<U16>(G_VERSION);

        // Now serialize the contents of our registry.
        if (saveToBuffer(l_buffer) != Status::OK)
        {
            _Nelobin_Error("Registry::saveToFile(): Error saving \"" << a_filename << "\" to buffer.");
            return Status::FileSaveError;
        }

        // Now save the buffer to file.
        if (l_buffer.saveToFile(a_filename) != Status::OK)
        {
            _Nelobin_Error("Registry::saveToFile(): Error saving \"" << a_filename << "\".");
            return Status::FileSaveError;
        }

        return Status::OK;
    }

}
