///
/// \file   Order.hpp
/// \brief  Contains functions for changing byte order.
///

#pragma once

#include <Nelobin/Types.hpp>
#include <Nelobin/Export.hpp>

namespace Nelobin
{

    ///
    /// \fn     swapBytes
    /// \brief  Flips the byte order of a piece of data.
    ///
    /// This function flips the byte order of the given piece of
    /// data between Host Byte Order and Network Byte Order (standardized
    /// to big endian). If the Host and Network Byte Orders match, then this
    /// function does nothing.
    ///
    /// \param  a_data              The data to be flipped.
    ///
    /// \return The flipped data.
    ///
    template <typename T>
    _NELOBIN_API T swapBytes (const T a_data);

    ///
    /// \fn     encodeFloat
    /// \brief  Encodes a floating-point number into a 32-bit unsigned integer.
    ///
    /// This integer is returned in Network Byte Order.
    ///
    /// \param  a_float             The float to be encoded.
    ///
    /// \return The encoded integer.
    ///
    _NELOBIN_API U32 encodeFloat (const Float a_float);

    ///
    /// \fn     decodeFloat
    /// \brief  Decodes a 32-bit unsigned integer into a floating-point number.
    ///
    /// The float is returned in Host Byte Order.
    ///
    /// \param  a_integer           The integer to be decoded.
    ///
    /// \return The decoded float.
    ///
    _NELOBIN_API Float decodeFloat (const U32 a_integer);

}
