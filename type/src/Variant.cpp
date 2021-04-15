#include <workflow/type/Variant.hpp>

#include <iostream>

namespace workflow::type {

Variant::Variant( const Variant& other )
    : mValue( other.mValue->clone() )
{
}

Variant::Variant( Variant&& other ) noexcept
    : mValue( std::move(other.mValue) )
{
}

Variant&
Variant::operator=( const Variant& other )
{
    Variant tmp(other);
    std::swap(tmp.mValue, mValue);
    return *this;
}

Variant&
Variant::operator=( Variant&& other ) noexcept
{
    std::swap(mValue, other.mValue);
    return *this;
}

bool
Variant::empty() const
{
    return nullptr == mValue;
}

void
Variant::clear()
{
    mValue.reset();
}

std::type_index
Variant::getTypeIndex() const
{
    return mValue ? mValue->getTypeIndex() : typeid(void);
}

std::string
Variant::getTypeName() const
{
    return utils::demangle(getTypeIndex().name());
}

bool
operator==( const Variant& lhs,
            const Variant& rhs )
{
    bool ret = lhs.getTypeIndex() == rhs.getTypeIndex();
    if ( ret )
    {
        if ( lhs.mValue && rhs.mValue )
        {
            ret = lhs.mValue->equals( *rhs.mValue );
        }
        else if ( !lhs.mValue && rhs.mValue || lhs.mValue && !rhs.mValue )
        {
            ret = false;
        }
        else
        {
            ret = true;
        }
    }
    return ret;
}

bool
operator!=( const Variant& lhs,
            const Variant& rhs )
{
    return !operator==(lhs, rhs );
}

std::ostream&
operator<<( std::ostream& os,
            const Variant& value )
{
    if ( value.mValue )
    {
        value.mValue->output( os );
    }
    else
    {
        os << "<void>()";
    }
    return os;
}

} // end namespace workflow::type