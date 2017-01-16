///
/// \file   EntryBase.cpp
///
/// Because we have a static member in the EntryBase class, and this file is
/// being referenced multiple types because of our template specializations,
/// we will need to define this in a CPP file.
///

#include <Nelobin/EntryBase.hpp>

namespace Nelobin
{
    EntryBase EntryBase::s_null { "null", Datatypes::Unknown };
}
