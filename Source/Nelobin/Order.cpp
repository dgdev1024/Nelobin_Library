///
/// \file   Order.cpp
///

#include <Nelobin/Order.hpp>

namespace Nelobin
{
    namespace Private
    {

        const U8 G_UNKNOWN_ENDIAN = 0x00;
        const U8 G_LITTLE_ENDIAN  = 0x01;
        const U8 G_BIG_ENDIAN     = 0x02;

        ///
        /// \fn     checkEndian
        /// \brief  Determines and returns the endianness of the running machine.
        ///
        static U8 checkEndian ()
        {
            static U8 s_endian = G_UNKNOWN_ENDIAN;

            if (s_endian == G_UNKNOWN_ENDIAN)
            {
                // Get the four bytes from a 32-bit integer.
                U32 l_int = 1;
                U8* lp_bytes = reinterpret_cast<U8*>(&l_int);

                // Check to see where the most significant byte is in order
                // to determine endianness.
                if (lp_bytes[0] == 0x00)
                    s_endian = G_BIG_ENDIAN;
                else if (lp_bytes[0] == 0x01)
                    s_endian = G_LITTLE_ENDIAN;
            }

            return s_endian;
        }

    }

    template <>
    S16 swapBytes (const S16 a_data)
    {
        if (Private::checkEndian() == Private::G_BIG_ENDIAN)
        {
            return a_data;
        }

        return  ((a_data &   0xFF) << 8) |
                ((a_data & 0xFF00) >> 8) ;
    }

    template <>
    S32 swapBytes (const S32 a_data)
    {
        if (Private::checkEndian() == Private::G_BIG_ENDIAN)
        {
            return a_data;
        }

        return  ((a_data &       0xFF) << 24) |
                ((a_data &     0xFF00) <<  8) |
                ((a_data &   0xFF0000) >>  8) |
                ((a_data & 0xFF000000) >> 24) ;
    }

    template <>
    U16 swapBytes (const U16 a_data)
    {
        if (Private::checkEndian() == Private::G_BIG_ENDIAN)
        {
            return a_data;
        }

        return  ((a_data &   0xFF) << 8) |
                ((a_data & 0xFF00) >> 8) ;
    }

    template <>
    U32 swapBytes (const U32 a_data)
    {
        if (Private::checkEndian() == Private::G_BIG_ENDIAN)
        {
            return a_data;
        }

        return  ((a_data &       0xFF) << 24) |
                ((a_data &     0xFF00) <<  8) |
                ((a_data &   0xFF0000) >>  8) |
                ((a_data & 0xFF000000) >> 24) ;
    }

    U32 encodeFloat (const Float a_float)
    {
        const U32 l_encoded =
            *reinterpret_cast<const U32*>(&a_float);

        return swapBytes<U32>(l_encoded);
    }

    Float decodeFloat (const U32 a_integer)
    {
        const U32 l_swapped = swapBytes<U32>(a_integer);
        return *reinterpret_cast<const Float*>(&l_swapped);
    }
}
