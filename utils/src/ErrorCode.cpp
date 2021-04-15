#include <workflow/utils/ErrorCode.hpp>

#include <iostream>

#include <boost/core/demangle.hpp>

namespace workflow::utils {

int64_t
ErrorCode::getCode() const
{
    return mValue;
}

std::type_index
ErrorCode::getTypeIndex() const
{
    return mTypeIndex;
}

std::ostream&
operator<<( std::ostream& os,
            const ErrorCode& code )
{
    os << boost::core::demangle(code.mTypeIndex.name()) << "(" << code.mValue << ")";
    return os;
}

} // end namespace workflow::utils
