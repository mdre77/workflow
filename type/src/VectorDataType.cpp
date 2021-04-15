#include <workflow/type/VectorDataType.hpp>

#include <sstream>
#include <limits>

#include <workflow/type/IDataTypeVisitor.hpp>
#include <workflow/type/DataStream.hpp>
#include <workflow/type/IVariantMethods.hpp>
#include <workflow/type/VariantMethodsManager.hpp>

#include <internal/EqualsVisitor.hpp>

namespace workflow::type {

VectorDataType::VectorDataType( const Variant& value )
    : mType( value )
{
    const auto& manager = VariantMethodsManager::instance();
    SEQ_ASSERT_ARGUMENT( manager.has( value.getTypeIndex() ),
                         "No IVariantMethods registered to handle type '"
                         << value.getTypeName() << "'" );
}

VectorDataType::VectorDataType( DataStream& stream )
{
    const auto& manager = VariantMethodsManager::instance();

    // Read the hash
    VariantMethodsManager::Hash hash;
    stream.read( hash.value );

    // Get the method to create the type description
    const auto& method = manager.get( hash );
    mType = method.create();

    // Get the number of attributes
    uint32_t numAttributes = 0;
    stream.read( numAttributes );

    // Read the values
    mValues.reserve(numAttributes);
    for( uint32_t i = 0; i < numAttributes; ++i )
    {
        Variant value;
        method.deserialize(stream, value );
        mValues.push_back( value );
    }
}

bool
operator==( const VectorDataType& lhs,
            const VectorDataType& rhs )
{
    return lhs.mValues == rhs.mValues;
}

bool
operator!=( const VectorDataType& lhs,
            const VectorDataType& rhs )
{
    return !operator==( lhs, rhs );
}

std::string
VectorDataType::getName() const
{
    std::stringstream ss;
    ss << "Vector<" << mType.getTypeName() << ">";
    return ss.str();
}

VectorDataType::Type
VectorDataType::getType() const noexcept
{
    return Type::Vector;
}

void
VectorDataType::visit( IDataTypeVisitor& visitor )
{
    visitor.visit( *this );
}

void
VectorDataType::visit( IConstDataTypeVisitor& visitor ) const
{
    visitor.visit( *this );
}

void
VectorDataType::serialize( DataStream& stream ) const
{
    const auto& manager = VariantMethodsManager::instance();
    auto hash = manager.calculateHash( mType.getTypeIndex() );
    const auto& method = manager.get( mType.getTypeIndex() );

    // Also support 32 bit systems.
    SEQ_ASSERT_INVARIANT( mValues.size() < std::numeric_limits<uint32_t>::max(),
                          "Too many elements" );
    stream.write( hash.value );
    stream.write(static_cast<uint32_t>(mValues.size()) );
    for ( const auto& value: mValues )
    {
        method.serialize(stream, value);
    }
}

bool
VectorDataType::equals( const IDataType& other ) const
{
    internal::EqualsVisitor<VectorDataType> visitor( *this );
    return visitor.equals( other );
}

void
VectorDataType::output( std::ostream& os ) const
{
    os << "Vector[" << mValues.size() <<"](";
    bool first = true;
    for ( const auto& v: mValues )
    {
        if ( !first )
        {
            os << ", ";
        }
        os << v;
        first = false;
    }
    os << ")";
}

} // end namespace workflow::type
