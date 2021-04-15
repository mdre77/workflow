#include <workflow/type/IDataType.hpp>
#include <workflow/type/DataStream.hpp>

#include <workflow/type/VariantDataType.hpp>
#include <workflow/type/StructDataType.hpp>

namespace workflow::type {

SEQ_INTERFACE_IMPL( IDataType );

void
IDataType::serialize( DataStream& stream,
                      const IDataType& dataType )
{
    stream.write( static_cast<uint32_t>(dataType.getType()) );
    dataType.serialize( stream );
}

IDataTypeUniquePtr
IDataType::deserialize( DataStream& stream )
{
    // NOTE: Here we depend on all the data type implementations. This is odd,
    // but we cannot overcome this. At some place we have to dispatch
    // deserialization
    uint32_t type = 0;
    stream.read( type );

    switch ( type )
    {
        case static_cast<uint32_t>(Type::Variant):
            return std::make_unique<VariantDataType>( stream );

        case static_cast<uint32_t>(Type::Struct):
            return std::make_unique<StructDataType>( stream );
    }

    SEQ_ASSERT_INVARIANT( false, "No data type with id '" << type << "'" );
}

bool
operator==( const IDataType& lhs,
            const IDataType& rhs )
{
    return lhs.equals( rhs );
}

bool
operator!=( const IDataType& lhs,
            const IDataType& rhs )
{
    return !operator==( lhs, rhs );
}

std::ostream&
operator<<( std::ostream& os,
            const IDataType& type )
{
    type.output( os );
    return os;
}

} // end namespace workflow::type