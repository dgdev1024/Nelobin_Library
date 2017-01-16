///
/// \file   Types.cpp
///

#include <Nelobin/Types.hpp>

namespace Nelobin
{

    String stringifyType (const U8 a_type)
    {
        switch (a_type)
        {
        case Datatypes::Unknown:            return "Unknown";
        case Datatypes::SignedByte:         return "Signed Byte";
        case Datatypes::SignedShort:        return "Signed Short";
        case Datatypes::SignedInteger:      return "Signed Integer";
        case Datatypes::UnsignedByte:       return "Unsigned Byte";
        case Datatypes::UnsignedShort:      return "Unsigned Short";
        case Datatypes::UnsignedInteger:    return "Unsigned Integer";
        case Datatypes::FloatingPoint:      return "Floating-Point Number";
        case Datatypes::StringLiteral:      return "String Literal";
        default:                            return "Unknown";
        }
    }

}
