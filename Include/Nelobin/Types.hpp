///
/// \file   Types.hpp
/// \brief  Utilities for defining the types and headers used in the Nelobin Library.
///

#pragma once

#include <string>
#include <vector>
#include <cstdint>
#include <Nelobin/Export.hpp>

namespace Nelobin
{

    // Typedefs
    using S8        = std::int8_t;
    using S16       = std::int16_t;
    using S32       = std::int32_t;
    using U8        = std::uint8_t;
    using U16       = std::uint16_t;
    using U32       = std::uint32_t;
    using Float     = float;
    using String    = std::string;
    using Boolean   = bool;
    using Size      = std::size_t;
    using ByteArray = std::vector<char>;

    // Header Constants
    const U32 G_ENTRY_HEADER    = 0x9E708199;
    const U32 G_KEY_HEADER      = 0x9E70819E;
    const U32 G_REGISTRY_HEADER = 0x9E70819F;
    const U16 G_VERSION         = 0x0100;

    ///
    /// \enum   TypeAlias
    /// \brief  Enumerates byte aliases, which are mapped to valid types.
    ///
    enum Datatypes : U8
    {
        Unknown,
        SignedByte,
        SignedShort,
        SignedInteger,
        UnsignedByte,
        UnsignedShort,
        UnsignedInteger,
        FloatingPoint,
        StringLiteral
    };

    ///
    /// \struct TT_Type
    /// \brief  Helper to determine which types are valid for our registry entries.
    ///
    template <typename T>
    struct TT_Type
    {
        static const Boolean Valid = false;
        static const U8 Alias = Datatypes::Unknown;
    };

    ///
    /// \def    _Nelobin_Datatype
    /// \brief  Defines a specialization of the 'TT_Type' struct above.
    ///
    /// This macro is not intended for use outside of this file.
    ///
    #define _Nelobin_Datatype(a_type, a_alias) \
        template <> \
        struct TT_Type<a_type> \
        { \
            static const Boolean Valid = true; \
            static const U8 Alias = a_alias; \
        };

    // Valid Type Mappings...
    _Nelobin_Datatype(S8,       Datatypes::SignedByte)
    _Nelobin_Datatype(S16,      Datatypes::SignedShort)
    _Nelobin_Datatype(S32,      Datatypes::SignedInteger)
    _Nelobin_Datatype(U8,       Datatypes::UnsignedByte)
    _Nelobin_Datatype(U16,      Datatypes::UnsignedShort)
    _Nelobin_Datatype(U32,      Datatypes::UnsignedInteger)
    _Nelobin_Datatype(Float,    Datatypes::FloatingPoint)
    _Nelobin_Datatype(String,   Datatypes::StringLiteral)

    ///
    /// \fn     stringifyType
    /// \brief  Gets a string representation of the given type alias.
    ///
    /// \param  a_type          The type alias.
    ///
    /// \return The string form of the given type alias.
    ///
    _NELOBIN_API String stringifyType (const U8 a_type);

}
