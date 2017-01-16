///
/// \file   Key.hpp
/// \brief  A "folder" in a registry, containing entries and subkeys.
///

#pragma once

#include <Nelobin/Buffer.hpp>
#include <Nelobin/Entry.hpp>
#include <Nelobin/Error.hpp>
#include <Nelobin/Export.hpp>

namespace Nelobin
{

    ///
    /// \class  Key
    /// \brief  A "folder" in the registry, containing entries and subkeys.
    ///
    /// In a Nelobin Registry, a key is like a folder. It is a container that
    /// can hold registry entries - key-value pairs of data - and even other keys.
    ///
    class _NELOBIN_API Key
    {
    public:
        using Ptr           = std::unique_ptr<Key>;
        using Container     = std::vector<Ptr>;
        using Iterator      = Container::iterator;
        using CIterator     = Container::const_iterator;

    private:
        /// A "null" key, returned in case of errors retriving and adding keys.
        static Key              s_null;

    protected:
        String                  m_name;         ///< The non-unique name of the key.
        U32                     m_instance;     ///< The unique instance ID of the key.
        EntryBase::Container    m_entries;      ///< Contains the key's entries.
        Container               m_keys;         ///< Contains the registry's keys, or the key's subkeys.
        Key*                    mp_parent;      ///< A pointer to the key's parent key.

    private:
        ///
        /// The methods below seek out a registry entry mapped to the
        /// given name.
        ///
        EntryBase::Iterator seekEntry (const String& a_name);
        EntryBase::CIterator cseekEntry (const String& a_name) const;

        ///
        /// The following methods seek out a registry key mapped to the
        /// given name and instance ID number.
        ///
        Key::Iterator seekKey (const String& a_name,
                               const U32 a_instance);
        Key::CIterator cseekKey (const String& a_name,
                                 const U32 a_instance) const;

        ///
        /// And these methods seek out a registry key mapped to the given
        /// name, starting from the iterator position given.
        ///
        /// These methods are useful for iterating through the key container
        /// to find out how many keys are present with the same name.
        ///
        Key::Iterator seekKeyFrom (const String& a_name,
                                   Key::Iterator a_from);
        Key::CIterator cseekKeyFrom (const String& a_name,
                                     Key::CIterator a_from) const;

    protected:
        ///
        /// The methods below are for de-serializing the next entity in a key
        /// or registry, be it a entry or subkey.
        ///
        Boolean deserializeEntry (Buffer& a_buffer);
        Boolean deserializeKey (Buffer& a_buffer);
        Boolean deserializeNext (Buffer& a_buffer);

    protected:
        ///
        /// \brief  The default constructor.
        ///
        /// This constructor is not intended for use outside of this class, or the
        /// inheriting Registry class. Keys should be created via this class's 'addKey'
        /// method instead.
        ///
        /// \param  a_name      Optional. The name of the key. Leave blank to indicate a root key.
        /// \param  a_instance  Optional. The unique instance ID of the key.
        /// \param  ap_parent   Optional. A pointer to this key's parent. Leave null to indicate a root key.
        ///
        Key (const String& a_name = "root",
             const U32 a_instance = 0,
             Key* ap_parent = nullptr);

    public:
        ///
        /// \brief  The destructor.
        ///
        ~Key ();

    public:
        ///
        /// \fn     getEntry
        /// \brief  Gets the data entry with the given name.
        ///
        /// Make sure this entry is of the same type as the template given, or
        /// you will receive a null pointer.
        ///
        /// \param  a_name      The name of the entry.
        ///
        /// \return The entry in question.
        ///
        template <typename T>
        inline Entry<T>& getEntry (const String& a_name)
        {
            // Check to see if the user specified a name.
            if (a_name.empty() == true)
            {
                _Nelobin_Error("Key::getEntry(): In key \"" << m_name << "\":");
                _Nelobin_Error("Key::getEntry(): You need to specify a name for the entry.");
                return Entry<T>::s_null;
            }

            // Find the entry.
            auto l_find = seekEntry(a_name);
            if (l_find == m_entries.end())
            {
                _Nelobin_Error("Key::getEntry(): In key \"" << m_name << "\":");
                _Nelobin_Error("Key::getEntry(): Entry, \"" << a_name << "\", not found.");
                return Entry<T>::s_null;
            }

            // Check to see if the type of the entry found and the type expected match.
            if ((*l_find)->getAlias() != TT_Type<T>::Alias)
            {
                _Nelobin_Error("Key::getEntry(): In key \"" << m_name << "\":");
                _Nelobin_Error("Key::getEntry(): Entry, \"" << a_name << "\", was found, but a type mismatch occured.");
                return Entry<T>::s_null;
            }

            // Downcast and return the pointer.
            Entry<T>* lp_downcast = dynamic_cast<Entry<T>*>((*l_find).get());
            return *lp_downcast;
        }

        ///
        /// \fn     getEntry
        /// \brief  Const version of the 'getEntry' method above.
        ///
        /// See the 'getEntry' non-const method above for more information.
        ///
        template <typename T>
        inline const Entry<T>& getEntry (const String& a_name) const
        {
            // Check to see if the user specified a name.
            if (a_name.empty() == true)
            {
                _Nelobin_Error("Key::getEntry(): In key \"" << m_name << "\":");
                _Nelobin_Error("Key::getEntry(): You need to specify a name for the entry.");
                return Entry<T>::s_null;
            }

            // Find the entry.
            auto l_find = cseekEntry(a_name);
            if (l_find == m_entries.cend())
            {
                _Nelobin_Error("Key::getEntry(): In key \"" << m_name << "\":");
                _Nelobin_Error("Key::getEntry(): Entry, \"" << a_name << "\", not found.");
                return Entry<T>::s_null;
            }

            // Check to see if the type of the entry found and the type expected match.
            if ((*l_find)->getAlias() != TT_Type<T>::Alias)
            {
                _Nelobin_Error("Key::getEntry(): In key \"" << m_name << "\":");
                _Nelobin_Error("Key::getEntry(): Entry, \"" << a_name << "\", was found, but a type mismatch occured.");
                return Entry<T>::s_null;
            }

            // Downcast and return the pointer.
            Entry<T>* lp_downcast = dynamic_cast<Entry<T>*>((*l_find).get());
            return *lp_downcast;
        }

        ///
        /// \fn     getEntryBaseAt
        /// \brief  Gets the entry base located at the given index.
        ///
        /// Please note that this method retrieves the entry BASE at the given
        /// index. If you are looking for a specific entry, casted into a proper type,
        /// then use the 'getEntry' method.
        ///
        /// \param  a_index     The index to look at.
        ///
        /// \return The entry base at the given index.
        ///
        inline EntryBase& getEntryBaseAt (const Size a_index)
        {
            // Check to see if the index is in bounds.
            if (a_index >= m_entries.size())
            {
                _Nelobin_Error("Key::getEntryAt(): In key \"" << m_name << "\":");
                _Nelobin_Error("Key::getEntryAt(): Index " << a_index << " is out of range.");
                return EntryBase::s_null;
            }

            return *m_entries[a_index];
        }

        ///
        /// \fn     stringifyEntry
        /// \brief  Gets the value of the entry with the given name, in string form.
        ///
        /// \param  a_name      The name of the entry.
        ///
        /// \return The string form of the entry's value.
        ///
        inline String stringifyEntry (const String& a_name)
        {
            // Check to see if the user specified a name.
            if (a_name.empty() == true)
            {
                _Nelobin_Error("Key::stringifyEntry(): In key \"" << m_name << "\":");
                _Nelobin_Error("Key::stringifyEntry(): You need to specify a name for the entry.");
                return "";
            }

            // Find the entry.
            auto l_find = seekEntry(a_name);
            if (l_find == m_entries.end())
            {
                _Nelobin_Error("Key::stringifyEntry(): In key \"" << m_name << "\":");
                _Nelobin_Error("Key::stringifyEntry(): Entry, \"" << a_name << "\", not found.");
                return "";
            }

            // Now stringify it.
            return stringifyEntryBase(*(*l_find));
        }

        ///
        /// \fn     stringifyEntry
        /// \brief  Gets the value of the entry at the given index, in string form.
        ///
        /// \param  a_index     The index to look at.
        ///
        /// \return The string form of the entry's value.
        ///
        inline String stringifyEntryAt (const Size a_index)
        {
            // Check to see if the index is in bounds.
            if (a_index >= m_entries.size())
            {
                _Nelobin_Error("Key::stringifyEntryAt(): In key \"" << m_name << "\":");
                _Nelobin_Error("Key::stringifyEntryAt(): Index " << a_index << " is out of range.");
                return "";
            }

            return stringifyEntryBase(*m_entries.at(a_index));
        }

        ///
        /// \fn     addEntry
        /// \brief  Adds a new entry to the key with the given name.
        ///
        /// \param  a_name      The new entry's name.
        ///
        /// \return A handle to the newly-created (or already-existing) entry.
        ///
        template <typename T>
        inline Entry<T>& addEntry (const String& a_name)
        {
            // Early out if no name is specified.
            if (a_name.empty() == true)
            {
                _Nelobin_Error("Key::addEntry(): In key \"" << m_name << "\":");
                _Nelobin_Error("Key::addEntry(): You need to specify a name for the new entry.");
                return Entry<T>::s_null;
            }

            // Check to see if the entry already exists in the type specified.
            auto l_find = seekEntry(a_name);
            if (l_find != m_entries.end())
            {
                // Early out if the type expected does not match the type of the
                // entry found.
                if ((*l_find)->getAlias() != TT_Type<T>::Alias)
                {
                    _Nelobin_Error("Key::addEntry(): In key \"" << m_name << "\":");
                    _Nelobin_Error("Key::addEntry(): Entry \"" << a_name << "\" was found, but a type mismatch occured.");
                    return Entry<T>::s_null;
                }
                else
                {
                    Entry<T>* lp_downcast = dynamic_cast<Entry<T>*>((*l_find).get());
                    return *lp_downcast;
                }
            }

            // Add, downcast, and return the new entry.
            EntryBase* lp_new = new Entry<T>(a_name);
            EntryBase::Ptr l_wrap { lp_new };
            m_entries.push_back(std::move(l_wrap));

            Entry<T>* lp_downcast = dynamic_cast<Entry<T>*>(lp_new);
            return *lp_downcast;
        }

        ///
        /// \fn     renameEntry
        /// \brief  Renames the entry with the given name.
        ///
        /// \param  a_name      The entry's name.
        /// \param  a_newName   The entry's new name.
        ///
        inline void renameEntry (const String& a_name,
                                 const String& a_newName)
        {
            // Check to see if the user provided the name of the existing key.
            if (a_name.empty() == true)
            {
                _Nelobin_Error("Key::renameEntry(): In key \"" << m_name << "\":");
                _Nelobin_Error("Key::renameEntry(): You need to specify an entry to rename.");
                return;
            }

            // Check to see if the user provided a new name for the entry in question.
            if (a_newName.empty() == true)
            {
                _Nelobin_Error("Key::renameEntry(): In key \"" << m_name << "\":");
                _Nelobin_Error("Key::renameEntry(): You need to specify a new name for the entry.");
                return;
            }

            // Check to see if the new name is valid.
            if (a_newName == "root" || a_newName == "null")
            {
                _Nelobin_Error("Key::renameEntry(): In key \"" << m_name << "\":");
                _Nelobin_Error("Key::renameEntry(): The new name, \"" << a_newName << "\", is reserved.");
                return;
            }

            // Check to see if the new name is already used.
            if (cseekEntry(a_newName) != m_entries.end())
            {
                _Nelobin_Error("Key::renameEntry(): In key \"" << m_name << "\":");
                _Nelobin_Error("Key::renameEntry(): The new name, \"" << a_newName << "\", is already used.");
                return;
            }

            // Now check to see if the entry in question exists.
            auto l_find = seekEntry(a_name);
            if (l_find == m_entries.end())
            {
                _Nelobin_Error("Key::renameEntry(): In key \"" << m_name << "\":");
                _Nelobin_Error("Key::renameEntry(): Entry, \"" << a_name << "\", not found.");
                return;
            }

            // Now rename the entry.
            (*l_find)->m_name = a_newName;
        }

        ///
        /// \fn     removeEntry
        /// \brief  Removes the entry with the given name from this key.
        ///
        /// \param  a_name      The name of the entry to remove.
        ///
        inline void removeEntry (const String& a_name)
        {
            // Seek the entry.
            auto l_find = seekEntry(a_name);
            if (l_find == m_entries.end())
            {
                _Nelobin_Error("Key::removeEntry(): In key \"" << m_name << "\":");
                _Nelobin_Error("Key::removeEntry(): Entry \"" << a_name << "\" not found.");
                return;
            }

            // Now remove it.
            l_find = m_entries.erase(l_find);
        }

    public:
        ///
        /// \fn     serialize
        /// \brief  Serializes the contents of this key into the given byte buffer.
        ///
        /// \param  a_buffer    The byte buffer to serialize to.
        ///
        void serialize (Buffer& a_buffer);

        ///
        /// \fn     getInstanceCount
        /// \brief  Gets the number of subkeys in this key with the given name.
        ///
        /// Depending on how many keys in question are found, their instance IDs
        /// may be updated.
        ///
        /// \param  a_name      The name of the keys to search for.
        ///
        /// \return The number of keys found.
        ///
        U32 getInstanceCount (const String& a_name);

        ///
        /// \fn     rename
        /// \brief  Renames this key.
        ///
        /// \param  a_name      The key's new name.
        ///
        void rename (const String& a_name);

        ///
        /// \fn     getKey
        /// \brief  Retrieves the key with the given name and instance ID.
        ///
        /// \param  a_name      The name of the key.
        /// \param  a_instance  Optional. The key's instance ID.
        ///
        /// \return The key found, or the "null" key.
        ///
        Key& getKey (const String& a_name,
                     const U32 a_instance = 0);
        const Key& getKey (const String& a_name,
                           const U32 a_instance = 0) const;

        ///
        /// \fn     getKey
        /// \brief  Retrieves the key located at the given index.
        ///
        /// \param  a_index     The index to search at.
        ///
        /// \return The key found, or the "null" key.
        ///
        Key& getKey (const Size a_index);
        const Key& getKey (const Size a_index) const;

        ///
        /// \fn     addKey
        /// \brief  Adds a new key with the given name.
        ///
        /// \param  a_name      The name of the new key.
        ///
        /// \return The newly added key.
        ///
        Key& addKey (const String& a_name);

        ///
        /// \fn     removeKey
        /// \brief  Removes the key with the given name and instance ID.
        ///
        /// \param  a_name      The name of the key to remove.
        /// \param  a_instance  Optional. The instance ID of the key.
        ///
        void removeKey (const String& a_name,
                        const U32 a_instance = 0);

        ///
        /// \fn     clear
        /// \brief  Clears the contents of the key.
        ///
        void clear ();

    public:
        const String& getName () const;
        U32 getInstanceID () const;
        Size getEntryCount () const;
        Size getKeyCount () const;
        Boolean isNull () const;
        Boolean isParent () const;
        Key& getParent ();

    };

}
