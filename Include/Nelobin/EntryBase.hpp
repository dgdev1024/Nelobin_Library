///
/// \file   EntryBase.hpp
/// \brief  The base class for our templated registry entry class.
///

#pragma once

#include <vector>
#include <memory>
#include <algorithm>
#include <Nelobin/Types.hpp>
#include <Nelobin/Buffer.hpp>
#include <Nelobin/Export.hpp>

namespace Nelobin
{

    ///
    /// \class  EntryBase
    /// \brief  The base class for our templated registry entry class.
    ///
    /// The Entry class will be a templated class with several specializations,
    /// one for each valid type in our engine. However, the Key class will contain
    /// a container for, and stringify the value of, these entries. We will need a
    /// way to allow the Key class to do that. That's where this base class comes in.
    ///
    class _NELOBIN_API EntryBase
    {
    public:
        using Ptr           = std::unique_ptr<EntryBase>;
        using Container     = std::vector<Ptr>;
        using Iterator      = Container::iterator;
        using CIterator     = Container::const_iterator;

    private:
        /// The Key class will have a method for renaming these entries.
        /// It will need access to the 'm_name' member.
        friend class Key;

    public:
        /// A "null" entry base to return, in case of an error in retrival.
        static EntryBase s_null;

    protected:
        String           m_name;        ///< The name of the entry.
        U8               m_alias;       ///< The entry's type alias byte.

    protected:
        ///
        /// \brief  The default constructor.
        ///
        /// \param  a_name              The name of the entry.
        /// \param  a_alias             The entry's type alias byte.
        ///
        EntryBase (const String& a_name,
                   const U8 a_alias) :
            m_name      { a_name },
            m_alias     { a_alias }
        {}

    public:
        ///
        /// \fn     serialize
        /// \brief  Serializes this entry into a byte buffer.
        ///
        /// In this base class, this method does nothing. It is intended to be
        /// called from one of the specialized entry classes which inherit this
        /// method.
        ///
        /// \param  a_buffer            The buffer into which the field will be serialized.
        ///
        virtual void serialize (Buffer& a_buffer) const { (void) a_buffer; }

    public:
        inline const String& getName () const { return m_name; }
        inline U8 getAlias () const { return m_alias; }
        inline Boolean isNull () const { return m_name == "null"; }

    };

}
