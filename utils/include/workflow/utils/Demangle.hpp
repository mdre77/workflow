#pragma once

#include <string>

namespace workflow::utils {

/**
 * Demangle a C++ symbol name
 *
 * @param [in]  name        The mangled name
 *
 * @return The demangled name
 */
std::string
demangle( const std::string& name );

} // end namespace workflow::utils