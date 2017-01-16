///
/// \file   Key.cpp
///

#include <Nelobin/Key.hpp>

namespace Nelobin
{

    Key Key::s_null { "null" };

    EntryBase::Iterator Key::seekEntry (const String &a_name)
    {
        return std::find_if(m_entries.begin(), m_entries.end(),
                            [&a_name] (const EntryBase::Ptr& a_entry)
        {
            return a_name == a_entry->getName();
        });
    }

    EntryBase::CIterator Key::cseekEntry (const String &a_name) const
    {
        return std::find_if(m_entries.cbegin(), m_entries.cend(),
                            [&a_name] (const EntryBase::Ptr& a_entry)
        {
            return a_name == a_entry->getName();
        });
    }

    Key::Iterator Key::seekKey (const String &a_name, const U32 a_instance)
    {
        return std::find_if(m_keys.begin(), m_keys.end(),
                            [&a_name, &a_instance] (const Key::Ptr& a_key)
        {
            return a_name == a_key->getName() &&
                   a_instance == a_key->getInstanceID();
        });
    }

    Key::CIterator Key::cseekKey (const String &a_name, const U32 a_instance) const
    {
        return std::find_if(m_keys.cbegin(), m_keys.cend(),
                            [&a_name, &a_instance] (const Key::Ptr& a_key)
        {
            return a_name == a_key->getName() &&
                   a_instance == a_key->getInstanceID();
        });
    }

    Key::Iterator Key::seekKeyFrom (const String &a_name, Key::Iterator a_from)
    {
        if (a_from >= m_keys.end())
            return m_keys.end();

        return std::find_if(a_from, m_keys.end(),
                            [&a_name] (const Key::Ptr& a_key)
        {
            return a_name == a_key->getName();
        });
    }

    Key::CIterator Key::cseekKeyFrom (const String &a_name, Key::CIterator a_from) const
    {
        if (a_from >= m_keys.cend())
            return m_keys.cend();

        return std::find_if(a_from, m_keys.cend(),
                            [&a_name] (const Key::Ptr& a_key)
        {
            return a_name == a_key->getName();
        });
    }

    Boolean Key::deserializeEntry (Buffer &a_buffer)
    {
        // Get the entry's name and type alias
        String l_name = "";
        U8 l_alias = 0;

        a_buffer.readData<String>(l_name)
                .readData<U8>(l_alias);

        // Make sure that we found them.
        if (l_name.empty() == true)
        {
            _Nelobin_Error("Key::deserializeEntry(): In key \"" + m_name + "\":");
            _Nelobin_Error("Key::deserializeEntry(): Parse error - No entry name found.");
            return false;
        }

        // The macro below creates a new entry with the value extracted from the
        // byte buffer, then adds it to this key.
        #define _Nelobin_Deserialize_Entry(a_type) { \
            a_type l_data; \
            Entry<a_type>& l_entry = addEntry<a_type>(l_name); \
            a_buffer.readData<a_type>(l_data); \
            l_entry.set(l_data); \
            } break;

        // Depending upon the type of entry that we found, add it to our key.
        switch (l_alias)
        {
        case Datatypes::SignedByte:         _Nelobin_Deserialize_Entry(S8)
        case Datatypes::SignedShort:        _Nelobin_Deserialize_Entry(S16)
        case Datatypes::SignedInteger:      _Nelobin_Deserialize_Entry(S32)
        case Datatypes::UnsignedByte:       _Nelobin_Deserialize_Entry(U8)
        case Datatypes::UnsignedShort:      _Nelobin_Deserialize_Entry(U16)
        case Datatypes::UnsignedInteger:    _Nelobin_Deserialize_Entry(U32)
        case Datatypes::FloatingPoint:      _Nelobin_Deserialize_Entry(Float)
        case Datatypes::StringLiteral:      _Nelobin_Deserialize_Entry(String)
        case Datatypes::Unknown:
        default:
            _Nelobin_Error("Key::deserializeEntry(): In key \"" + m_name + "\":");
            _Nelobin_Error("Key::deserializeEntry(): Parse error - Entry type alias not recognized.");
            return false;
        }

        return true;
    }

    Boolean Key::deserializeKey (Buffer &a_buffer)
    {
        // Get the key's name and expected number of entities.
        String l_name = "";
        U32 l_entityCount = 0;

        a_buffer.readData<String>(l_name)
                .readData<U32>(l_entityCount);

        // Check to see if a name was provided.
        if (l_name.empty() == true)
        {
            _Nelobin_Error("Key::deserializeKey(): In key \"" << m_name << "\":");
            _Nelobin_Error("Key::deserializeKey(): Parse error - No subkey name found.");
            return false;
        }

        // Add the key and verify its existance.
        Key& l_key = addKey(l_name);
        if (l_key.isNull() == true)
        {
            _Nelobin_Error("Key::deserializeKey(): In key \"" << m_name << "\":");
            _Nelobin_Error("Key::deserializeKey(): Parse error - Subkey name reserved.");
            return false;
        }

        // Now deserialize the expected contents of the key.
        for (U32 i = 0; i < l_entityCount; ++i)
        {
            if (l_key.deserializeNext(a_buffer) == false)
            {
                _Nelobin_Error("Key::deserializeKey(): In key \"" << m_name << "\":");
                _Nelobin_Error("Key::deserializeKey(): Parse error - Could not parse subkey.");
                return false;
            }
        }

        return true;
    }

    Boolean Key::deserializeNext (Buffer &a_buffer)
    {
        // First, get the binary header.
        // This determines what kind of entity we are deserializing.
        //
        // Are we deserializing an entry or a subkey?
        U32 l_header = 0;
        a_buffer.readData<U32>(l_header);

        // Make sure the header is valid.
        if (l_header == G_ENTRY_HEADER)
            return deserializeEntry(a_buffer);
        else if (l_header == G_KEY_HEADER)
            return deserializeKey(a_buffer);

        // If we end up here, then we have an invalid header.
        _Nelobin_Error("Key::deserializeNext(): In key \"" << m_name << "\":");
        _Nelobin_Error("Key::deserializeNext(): Parse error - Invalid header found.");
        return false;
    }

    Key::Key (const String &a_name,
              const U32 a_instance,
              Key *ap_parent) :
        m_name      { a_name },
        m_instance  { a_instance },
        mp_parent   { ap_parent }
    {

    }

    Key::~Key ()
    {
        mp_parent = nullptr;
        m_entries.clear();
        m_keys.clear();
    }

    void Key::serialize (Buffer &a_buffer)
    {
        // Don't serialize the key if its name is "null".
        // That is a reserved keyword which indicates the "null" key
        // that is returned in case of errors retriving or adding keys.
        if (m_name == "null")
            return;

        // If the key's name is "root", then that indicates that
        // this is the root key of the Nelobin registry. It is not necessary
        // to record that key's name in the buffer. Do record that name and
        // header if it is not the root.
        if (m_name != "root")
        {
            a_buffer.writeData<U32>(G_KEY_HEADER)
                    .writeData<String>(m_name);
        }

        // Record the number of entries and subkeys in this key.
        U32 l_entryCount = static_cast<U32>(m_entries.size());
        U32 l_keyCount   = static_cast<U32>(m_keys.size());
        a_buffer.writeData<U32>(l_entryCount + l_keyCount);

        // Record each entry...
        for (const auto& l_entry : m_entries)
            l_entry->serialize(a_buffer);

        // ...and then record each subkey.
        for (const auto& l_key : m_keys)
            l_key->serialize(a_buffer);
    }

    U32 Key::getInstanceCount (const String &a_name)
    {
        U32 l_return = 0;
        auto l_iter = m_keys.begin();

        while (true)
        {
            // Get the next key with the same name.
            l_iter = seekKeyFrom(a_name, l_iter);

            // Break out if such key is not found.
            if (l_iter == m_keys.end())
                break;
            else
            {
                (*l_iter)->m_instance = l_return;
                l_return++;
                l_iter += 1;
            }
        }

        return l_return;
    }

    void Key::rename (const String &a_name)
    {
        // Store the key's old name.
        String l_oldName = m_name;

        // Check to see if this is the root key. The root key cannot
        // be renamed.
        if (mp_parent == nullptr)
        {
            _Nelobin_Error("Key::rename(): The root key cannot be renamed.");
            return;
        }

        // Don't rename if the new name is empty or reserved.
        if (a_name.empty() == true)
        {
            _Nelobin_Error("Key::rename(): In key \"" << m_name << "\":");
            _Nelobin_Error("Key::rename(): No name specified for subkey.");
            return;
        }
        else if (a_name == "root" || a_name == "null")
        {
            _Nelobin_Error("Key::rename(): In key \"" << m_name << "\":");
            _Nelobin_Error("Key::rename(): \"" << a_name << "\" is reserved.");
            return;
        }

        // Change the name of the key.
        m_name = a_name;

        // Now refresh the instance ID of keys with this new name and
        // keys with the old name.
        mp_parent->getInstanceCount(l_oldName);
        mp_parent->getInstanceCount(a_name);
    }

    Key& Key::getKey (const String &a_name, const U32 a_instance)
    {
        // Don't try to find a key with no name, or an invalid name.
        if (a_name.empty() == true)
        {
            _Nelobin_Error("Key::getKey(): In key \"" << m_name << "\":");
            _Nelobin_Error("Key::getKey(): No name specified for subkey.");
            return Key::s_null;
        }
        else if (a_name == "root" || a_name == "null")
        {
            _Nelobin_Error("Key::getKey(): In key \"" << m_name << "\":");
            _Nelobin_Error("Key::getKey(): \"" << a_name << "\" is reserved.");
            return Key::s_null;
        }

        // Now check to see if the key exists.
        auto l_find = seekKey(a_name, a_instance);
        if (l_find == m_keys.end())
        {
            _Nelobin_Error("Key::getKey(): In key \"" << m_name << "\":");
            _Nelobin_Error("Key::getKey(): Key \"" << a_name << "\" (" << a_instance << ") not found.");
            return Key::s_null;
        }

        // Return what we found.
        return *(*l_find);
    }

    const Key& Key::getKey (const String &a_name, const U32 a_instance) const
    {
        // Don't try to find a key with no name, or an invalid name.
        if (a_name.empty() == true)
        {
            _Nelobin_Error("Key::getKey(): In key \"" << m_name << "\":");
            _Nelobin_Error("Key::getKey(): No name specified for subkey.");
            return Key::s_null;
        }
        else if (a_name == "root" || a_name == "null")
        {
            _Nelobin_Error("Key::getKey(): In key \"" << m_name << "\":");
            _Nelobin_Error("Key::getKey(): \"" << a_name << "\" is reserved.");
            return Key::s_null;
        }

        // Now check to see if the key exists.
        auto l_find = cseekKey(a_name, a_instance);
        if (l_find == m_keys.cend())
        {
            _Nelobin_Error("Key::getKey(): In key \"" << m_name << "\":");
            _Nelobin_Error("Key::getKey(): Key \"" << a_name << "\" (" << a_instance << ") not found.");
            return Key::s_null;
        }

        // Return what we found.
        return *(*l_find);
    }

    Key& Key::getKey (const Size a_index)
    {
        // Check to see if we are in bounds.
        if (a_index >= m_keys.size())
        {
            _Nelobin_Error("Key::getKey(): In key \"" << m_name << "\":");
            _Nelobin_Error("Key::getKey(): Index " << a_index << " is out of range.");
            return Key::s_null;
        }

        return *m_keys[a_index];
    }

    const Key& Key::getKey (const Size a_index) const
    {
        // Check to see if we are in bounds.
        if (a_index >= m_keys.size())
        {
            _Nelobin_Error("Key::getKey(): In key \"" << m_name << "\":");
            _Nelobin_Error("Key::getKey(): Index " << a_index << " is out of range.");
            return Key::s_null;
        }

        return *m_keys[a_index];
    }

    Key& Key::addKey (const String &a_name)
    {
        // Don't try to add a key with no name, or an invalid name.
        if (a_name.empty() == true)
        {
            _Nelobin_Error("Key::addKey(): In key \"" << m_name << "\":");
            _Nelobin_Error("Key::addKey(): No name specified for subkey.");
            return Key::s_null;
        }
        else if (a_name == "root" || a_name == "null")
        {
            _Nelobin_Error("Key::addKey(): In key \"" << m_name << "\":");
            _Nelobin_Error("Key::addKey(): \"" << a_name << "\" is reserved.");
            return Key::s_null;
        }

        // Generate the new key's instance ID, and update those of other
        // keys bearing the same name.
        U32 l_instance = getInstanceCount(a_name);

        // Add the new key and push it in.
        Key* lp_new = new Key(a_name, l_instance, this);
        Key::Ptr l_wrap { lp_new };
        m_keys.push_back(std::move(l_wrap));
        return *lp_new;
    }

    void Key::removeKey (const String &a_name, const U32 a_instance)
    {
        // Check to see if the key could be found.
        auto l_find = seekKey(a_name, a_instance);
        if (l_find == m_keys.end())
        {
            _Nelobin_Error("Key::removeKey(): In key \"" << m_name << "\":");
            _Nelobin_Error("Key::removeKey(): Key \"" << a_name << "\" (" << a_instance << ") not found.");
            return;
        }

        // Remove the key and refresh the instance IDs of other keys with the same
        // name.
        l_find = m_keys.erase(l_find);
        getInstanceCount(a_name);
    }

    void Key::clear ()
    {
        m_keys.clear();
        m_entries.clear();
    }

    const String& Key::getName () const
    {
        return m_name;
    }

    U32 Key::getInstanceID () const
    {
        return m_instance;
    }

    Size Key::getEntryCount () const
    {
        return m_entries.size();
    }

    Size Key::getKeyCount () const
    {
        return m_keys.size();
    }

    Boolean Key::isNull () const
    {
        return m_name == "null";
    }

    Boolean Key::isParent () const
    {
        return isNull() == false && mp_parent == nullptr;
    }

    Key& Key::getParent ()
    {
        if (mp_parent == nullptr)
            return *this;
        else
            return *mp_parent;
    }

}
