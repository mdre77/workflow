#include <workflow/utils/Demangle.hpp>

#include <boost/core/demangle.hpp>

namespace workflow::utils {

std::string
demangle( const std::string& name )
{
    return boost::core::demangle( name.c_str() );
}

} // end namespace workflow::utils