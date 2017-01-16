///
/// \file   Export.hpp
/// \brief  Macros for importing and exporting the library's functions.
///

#pragma once

// Our library's export and import symbols will depend upon
// what operating system we are using.
#if defined(_WINDOWS) || defined(_WIN32)
# if defined(_NELOBIN_BUILDING)
#  define _NELOBIN_API __declspec(dllexport)
# else
#  define _NELOBIN_API __declspec(dllimport)
# endif
#else
# if defined(__GNUC__) && __GNUC__ >= 5
#  if defined(_NELOBIN_BUILDING)
#   define _NELOBIN_API __attribute__ ((visibility("default")))
#  else
#   define _NELOBIN_API
#  endif
# else
#  error The Nelobin Library uses some C++14 features. This will at least G++ version 5.
# endif
#endif
