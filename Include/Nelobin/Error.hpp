///
/// \file   Error.hpp
/// \brief  Utilities for handling errors.
///

#pragma once

#include <iostream>
#include <Nelobin/Types.hpp>
#include <Nelobin/Export.hpp>

#define _Nelobin_Error(a_msg) std::cerr << a_msg << std::endl;

namespace Nelobin
{

    ///
    /// \enum   Status
    /// \brief  Enumerates function status codes.
    ///
    enum class Status
    {
        OK,
        NoName,
        FileLoadError,
        FileSaveError,
        FileParseError
    };

    ///
    /// \fn     stringifyStatus
    /// \brief  Gets a string description of the given status code.
    ///
    /// \param  a_status            The status code to stringify.
    ///
    /// \return The string description of the given status.
    ///
    _NELOBIN_API String stringifyStatus (const Status a_status);

}
