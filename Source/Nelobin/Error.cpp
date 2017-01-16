///
/// \file   Error.cpp
///

#include <Nelobin/Error.hpp>

namespace Nelobin
{

    String stringifyStatus (const Status a_status)
    {
        switch (a_status)
        {
        case Status::OK:                return "The operation completed successfully.";
        case Status::NoName:            return "No name was specified for the operation.";
        case Status::FileLoadError:     return "An error occured while loading the file.";
        case Status::FileSaveError:     return "An error occured while saving the file.";
        case Status::FileParseError:    return "An error occured while parsing the file.";
        default:                        return "An unknown error occured.";
        }
    }

}
