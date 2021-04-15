#include <workflow/type/VariantDataType.hpp>

#include <workflow/type/DataStream.hpp>
#include <workflow/type/IDataTypeVisitor.hpp>
#include <workflow/type/IVariantMethods.hpp>
#include <workflow/type/VariantMethodsManager.hpp>

#include <internal/EqualsVisitor.hpp>

namespace workflow::type {

VariantDataType::VariantDataType( Variant value )
    : mValue(value)
{
    const auto& manager = VariantMethodsManager::instance();
    SEQ_ASSERT_ARGUMENT( manager.has( value.getTypeIndex() ),
                         "No IVariantMethods registered to handle type '"
                         << value.getTypeName() << "'" );
}

VariantDataType::VariantDataType(DataStream& stream)
{
    const auto& manager = VariantMethodsManager::instance();

    // Read the hash
    VariantMethodsManager::Hash hash;
    stream.read( hash.value );

    // Get the method to deserialize the value
    const auto& method = manager.get( hash );
    method.deserialize( stream, mValue );
}

Variant
VariantDataType::get() const
{
    return mValue;
}

void
VariantDataType::set(Variant value )
{
    SEQ_ASSERT_ARGUMENT( mValue.getTypeIndex() == value.getTypeIndex(),
                         "Ivnalid data type: expected '" << mValue.getTypeName()
                         << "' but got '" << value.getTypeName() << "'" );
    mValue = std::move(value);
}

bool
operator==( const VariantDataType& lhs,
            const VariantDataType& rhs )
{
    return lhs.mValue == rhs.mValue;
}

bool
operator!=( const VariantDataType& lhs,
            const VariantDataType& rhs )
{
    return !operator==( lhs, rhs );
}

std::ostream&
operator<<( std::ostream& os,
            const VariantDataType& value )
{
    value.output( os );
    return os;
}

std::string
VariantDataType::getName() const
{
    return mValue.getTypeName();
}

VariantDataType::Type
VariantDataType::getType() const noexcept
{
    return Type::Variant;
}

void
VariantDataType::visit( IDataTypeVisitor& visitor )
{
    visitor.visit( *this );
}

void
VariantDataType::visit( IConstDataTypeVisitor& visitor ) const
{
    visitor.visit( *this );
}

void
VariantDataType::serialize( DataStream& stream ) const
{
    const auto& manager = VariantMethodsManager::instance();
    auto hash = manager.calculateHash( mValue.getTypeIndex() );
    const auto& method = manager.get( hash );

    stream.write( hash.value );
    method.serialize( stream, mValue );
}

bool
VariantDataType::equals( const IDataType& other ) const
{
    internal::EqualsVisitor<VariantDataType> visitor(*this);
    return visitor.equals(other);
}

void
VariantDataType::output( std::ostream& os ) const
{
    os << "Variant" << mValue;
}

} // end namespace workflow::type