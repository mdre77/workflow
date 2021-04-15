#include <workflow/type/StructDataType.hpp>

#include <algorithm>

#include <workflow/utils/Error.hpp>

#include <workflow/type/DataStream.hpp>
#include <workflow/type/IDataTypeVisitor.hpp>

#include <internal/EqualsVisitor.hpp>

namespace workflow::type {

StructDataType::StructDataType( const std::string& name,
                                const NamedTypes& namedTypes )
    : mName( name )
    , mAttributes( namedTypes )
{
    SEQ_ASSERT_ARGUMENT( !name.empty(), "Invalid name" );
    SEQ_ASSERT_ARGUMENT( namedTypes.size(), "No attributes" );
}

StructDataType::StructDataType( DataStream& stream )
{
    stream.read( mName );
    SEQ_ASSERT_INVARIANT( !mName.empty(), "Invalid name" );

    uint32_t numAttributes = 0;
    stream.read( numAttributes );

    for ( uint32_t i = 0; i < numAttributes; ++i )
    {
        std::string attributeName;
        stream.read( attributeName );
        SEQ_ASSERT_INVARIANT( !attributeName.empty(), "Invalid attribute name" );

        IDataTypeSharedPtr attribute = IDataType::deserialize(stream);
        SEQ_ASSERT_INVARIANT( attribute, "Invalid attribute" );

        auto status = mAttributes.insert({ attributeName, attribute });
        SEQ_ASSERT_INVARIANT( status.second, "Dupliacted attribute '"
                              << attributeName << "'" );
    }
}

std::vector<std::string>
StructDataType::getAttributes() const
{
    std::vector<std::string> ret;
    ret.reserve( mAttributes.size() );
    std::transform( mAttributes.begin(), mAttributes.end(), std::back_inserter(ret),
                    [](const auto& item){ return item.first; });
    return ret;
}

bool
StructDataType::has( const std::string& name ) const
{
    return mAttributes.end() != mAttributes.find(name);
}

const IDataType&
StructDataType::get( const std::string& name ) const
{
    auto it = mAttributes.find(name);
    SEQ_ASSERT_ARGUMENT( mAttributes.end() != it, "No attribute called '" << name << "'" );
    return *it->second;
}

IDataType&
StructDataType::get( const std::string& name )
{
    auto it = mAttributes.find(name);
    SEQ_ASSERT_ARGUMENT( mAttributes.end() != it, "No attribute called '" << name << "'" );
    return *it->second;
}

bool
operator==( const StructDataType& lhs,
            const StructDataType& rhs )
{
    return lhs.mName == rhs.mName
        && std::equal( lhs.mAttributes.begin(), lhs.mAttributes.end(),
                       rhs.mAttributes.begin(), rhs.mAttributes.end(),
                       [](const auto& lhs, const auto& rhs)
                        {
                            return lhs.first == rhs.first && *lhs.second == *rhs.second;
                        } );
}

bool
operator!=( const StructDataType& lhs,
            const StructDataType& rhs )
{
    return !operator==( lhs, rhs );
}

std::ostream&
operator<<( std::ostream& os,
            const StructDataType& value )
{
    value.output( os );
    return os;
}

std::string
StructDataType::getName() const
{
    return mName;
}

StructDataType::Type
StructDataType::getType() const noexcept
{
    return StructDataType::Type::Struct;
}

void
StructDataType::visit( IDataTypeVisitor& visitor )
{
    visitor.visit( *this );
}

void
StructDataType::visit( IConstDataTypeVisitor& visitor ) const
{
    visitor.visit( *this );
}

void
StructDataType::serialize( DataStream& stream ) const
{
    // Also support 32 bit systems.
    SEQ_ASSERT_INVARIANT( mAttributes.size() < std::numeric_limits<uint32_t>::max(),
                          "Too many attributes" );

    stream.write(mName);
    stream.write(static_cast<uint32_t>(mAttributes.size()) );
    for ( const auto& attribute: mAttributes )
    {
        stream.write( attribute.first );
        IDataType::serialize( stream, *attribute.second );
    }
}

bool
StructDataType::equals( const IDataType& other ) const
{
    internal::EqualsVisitor<StructDataType> visitor( *this );
    return visitor.equals( other );
}

void
StructDataType::output( std::ostream& os ) const
{
    os << "Struct[" << mName << "](\n";
    for ( auto attribute: mAttributes )
    {
        os << "    " << attribute.first << "=" << *attribute.second << "\n";
    }
    os << ")";
}

} // end namespace workflow::type
