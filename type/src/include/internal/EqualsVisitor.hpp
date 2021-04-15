#pragma once

#include <workflow/type/IDataTypeVisitor.hpp>
#include <workflow/type/VariantDataType.hpp>
#include <workflow/type/StructDataType.hpp>
#include <workflow/type/VectorDataType.hpp>

namespace workflow::type::internal {

template <typename T>
class EqualsVisitor : public IConstDataTypeVisitor
{
public:
    EqualsVisitor( const T& lhs );

    bool
    equals( const IDataType& rhs );

    virtual void
    visit( const VariantDataType& type ) override;

    virtual void
    visit( const StructDataType& type ) override;

    virtual void
    visit( const VectorDataType& type ) override;

private:
    const T&    mLhs;
    bool        mIsEqual;
};

/******************************************************************************
 * Inlined implementations
 *****************************************************************************/
template <typename T>
EqualsVisitor<T>::EqualsVisitor( const T& lhs )
    : mLhs( lhs )
    , mIsEqual( false )
{
}

template <typename T>
bool
EqualsVisitor<T>::equals( const IDataType& rhs )
{
    rhs.visit( *this );
    return mIsEqual;
}

template <typename T>
void
EqualsVisitor<T>::visit( const VariantDataType& rhs )
{
    if constexpr ( std::is_same_v<T, VariantDataType> )
    {
        mIsEqual = (mLhs == rhs);
    }
}

template <typename T>
void
EqualsVisitor<T>::visit( const StructDataType& rhs )
{
    if constexpr ( std::is_same_v<T, StructDataType> )
    {
        mIsEqual = (mLhs == rhs);
    }
}

template <typename T>
void
EqualsVisitor<T>::visit( const VectorDataType& rhs )
{
    if constexpr ( std::is_same_v<T, VectorDataType> )
    {
        mIsEqual = (mLhs == rhs);
    }
}

} // end namespace workflow::type::internal