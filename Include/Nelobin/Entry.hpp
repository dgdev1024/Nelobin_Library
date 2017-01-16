///
/// \file   Entry.hpp
/// \brief  The template-specialized registry entry class.
///

#include <Nelobin/EntryBase.hpp>

namespace Nelobin
{

    ///
    /// \class  Entry
    /// \brief  The template-specialized registry entry class.
    ///
    template <typename T>
    class Entry : public EntryBase
    {
    private:
        static_assert(TT_Type<T>::Valid == true,
                      "Attempt to use 'Nelobin::Entry' with invalid type!");

    public:
        /// A "null" entry, returned in case of errors retriving or adding.
        static Entry<T> s_null;

    private:
        T   m_data;             ///< The entry's value.

    public:
        ///
        /// \brief  The default constructor.
        ///
        /// \param  a_name      The entry's name.
        /// \param  a_data      Optional. The entry's value.
        ///
        Entry (const String& a_name,
               const T& a_data = T()) :
            EntryBase   { a_name, TT_Type<T>::Alias },
            m_data      { a_data }
        {}
        
    public:
        ///
        /// \fn     serialize
        /// \brief  Serializes the contents of this field into the given byte buffer.
        ///
        inline void serialize (Buffer& a_buffer) const override
        {
            a_buffer.writeData<U32>(G_ENTRY_HEADER)
                    .writeData<String>(m_name)
                    .writeData<U8>(m_alias)
                    .writeData<T>(m_data);
        }

    public:
        ///
        /// \fn     get
        /// \brief  Gets the value of the entry.
        ///
        /// \return The value of the entry.
        ///
        inline T& get () { return m_data; }
        inline const T& get () const { return m_data; }

        ///
        /// \fn     set
        /// \brief  Sets the value of the entry
        ///
        /// \param  a_data      The entry's value.
        ///
        inline void set (const T& a_data) { m_data = a_data; }

    public:
        ///
        /// Disallow copying of this class. This prevents the 'use of deleted function'
        /// compiler error when attempting to add entries to keys.
        ///
        Entry<T> (const Entry<T>&) = delete;
        void operator= (const Entry<T>&) = delete;

    };

    template <typename T>
    Entry<T> Entry<T>::s_null { "null" };

    ///
    /// \fn     stringifyEntryBase
    /// \brief  Gets a string form of the given entry base.
    ///
    /// This function takes the base form of an entry, not the entry itself.
    /// This function is intended to be used from the Key class's 'stringifyEntry'
    /// and 'stringifyEntryAt' methods.
    ///
    /// \param  a_entry         The entry to be stringified. Remember - base entries only!
    ///
    /// \return The string form of the entry's value.
    ///
    _NELOBIN_API String stringifyEntryBase (EntryBase& a_entry);

}
