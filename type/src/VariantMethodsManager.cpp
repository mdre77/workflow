#include <workflow/type/VariantMethodsManager.hpp>

#include <unordered_map>
#include <algorithm>
#include <functional>
#include <iomanip>

#include <boost/lexical_cast.hpp>

#include <workflow/utils/Demangle.hpp>
#include <workflow/utils/Error.hpp>

#include <workflow/type/IVariantMethods.hpp>
#include <workflow/type/DataStream.hpp>

namespace workflow::type {
namespace {

template<typename T>
std::string
toStringImpl( const T& value )
{
    std::stringstream ss;
    ss << value;
    return ss.str();
}

template<>
std::string
toStringImpl( const bool& value )
{
    std::stringstream ss;
    ss << (value ? "true" : "false");
    return ss.str();
}

template<>
std::string
toStringImpl( const uint8_t& value )
{
    return toStringImpl<uint16_t>( value );
}

template<>
std::string
toStringImpl( const int8_t& value )
{
    return toStringImpl<int16_t>( value );
}

template<>
std::string
toStringImpl( const std::string& value )
{
    return value;
}

template<typename T>
T
fromStringImpl( const std::string& value )
{
    try
    {
        if constexpr (std::is_unsigned_v<T> )
        {
            SEQ_ASSERT_ARGUMENT( value[0] != '-', "Failed to convert '"
                                 << value << "' to " << utils::demangle(typeid(T).name()));
        }

        return boost::lexical_cast<T>(value);
    }
    catch ( ... )
    {
        SEQ_ASSERT_ARGUMENT( false, "Failed to convert '" << value << "' to "
                             << utils::demangle(typeid(T).name()));
    }
}

template<>
bool
fromStringImpl( const std::string& value )
{
    if ( value == "true" )
    {
        return true;
    }
    else if ( value == "false" )
    {
        return false;
    }
    else
    {
        SEQ_ASSERT_ARGUMENT( false, "Failed to convert '" << value
                              << "' to bool. Value must be either true or false" );
    }
}

template<>
uint8_t
fromStringImpl( const std::string& value )
{
    auto tmp = fromStringImpl<uint16_t>(value);
    SEQ_ASSERT_ARGUMENT( tmp <= std::numeric_limits<uint8_t>::max(),
                         "Failed to convert '" << value << "' to uint8_t" );
    return static_cast<uint8_t>( tmp );
}

template<>
int8_t
fromStringImpl( const std::string& value )
{
    auto tmp = fromStringImpl<int16_t>(value);
    SEQ_ASSERT_ARGUMENT( tmp <= std::numeric_limits<int8_t>::max()
                         && tmp >= std::numeric_limits<int8_t>::min(),
                         "Failed to convert '" << value << "' to uint8_t" );
    return static_cast<uint8_t>( tmp );
}

template<>
std::string
fromStringImpl( const std::string& value )
{
    return value;
}

template<typename T>
class DefaultVariantMethods : public IVariantMethods
{
public:
    DefaultVariantMethods( const std::string& name )
        : mName( name )
    {
        SEQ_ASSERT_ARGUMENT( !mName.empty(), "Empty name" );
    }

    virtual std::string
    getName() const override
    {
        return mName;
    }

    virtual Variant
    create() const override
    {
        return Variant(T());
    }

    virtual std::string
    toString( const Variant& value ) const override
    {
        return toStringImpl<T>( value.template get<T>() );
    }

    virtual Variant
    fromString( const std::string& value ) const override
    {
        return Variant( fromStringImpl<T>( value ) );
    }

    virtual void
    serialize( DataStream& stream,
               const Variant& value ) const override
    {
        stream.write( value.template get<T>() );
    }

    virtual void
    deserialize( DataStream& stream,
                 Variant& value ) const override
    {
        T tmp  = {};
        stream.read( tmp );
        value = Variant( tmp );
    }

private:
    std::string mName;
};

std::ostream&
operator<<( std::ostream& os,
            const VariantMethodsManager::Hash& hash )
{
    auto flags = os.flags();
    os << "0x" << std::hex << std::setw(2*sizeof(std::size_t)) << hash.value;
    os.setf( flags );
    return os;
}

} // end namespace

class VariantMethodsManager::Impl
{
public:
    // Consider a robust hash
    std::unordered_map<std::type_index, IVariantMethodsUniquePtr> mMethods;
    std::unordered_map<std::uint64_t, std::type_index> mHashMap;
};

VariantMethodsManager::VariantMethodsManager()
    : mImpl( std::make_unique<Impl>() )
{
    insert( std::make_unique<DefaultVariantMethods<bool>>( "bool" ) );
    insert( std::make_unique<DefaultVariantMethods<uint8_t>>( "uint8" ) );
    insert( std::make_unique<DefaultVariantMethods<uint16_t>>( "uint16" ) );
    insert( std::make_unique<DefaultVariantMethods<uint32_t>>( "uint32" ) );
    insert( std::make_unique<DefaultVariantMethods<uint64_t>>( "uint64" ) );
    insert( std::make_unique<DefaultVariantMethods<int8_t>>( "sint8" ) );
    insert( std::make_unique<DefaultVariantMethods<int16_t>>( "sint16" ) );
    insert( std::make_unique<DefaultVariantMethods<int32_t>>( "sint32" ) );
    insert( std::make_unique<DefaultVariantMethods<int64_t>>( "sint64" ) );
    insert( std::make_unique<DefaultVariantMethods<float>>( "float" ) );
    insert( std::make_unique<DefaultVariantMethods<double>>( "double" ) );
    insert( std::make_unique<DefaultVariantMethods<std::string>>( "string" ) );
}

VariantMethodsManager::~VariantMethodsManager() = default;

VariantMethodsManager&
VariantMethodsManager::instance()
{
    static VariantMethodsManager instance;
    return instance;
}

VariantMethodsManager::Hash
VariantMethodsManager::calculateHash( const std::type_index& type ) const
{
    const auto& methods = get( type );

    return Hash{ std::hash<std::string>{}(methods.getName()) };
}

void
VariantMethodsManager::insert( IVariantMethodsUniquePtr methods )
{
    SEQ_ASSERT_ARGUMENT( methods, "Invalid methods pointer" );

    auto variant = methods->create();

    auto hash = Hash{ std::hash<std::string>{}(methods->getName()) };

    auto status = mImpl->mMethods.insert({ variant.getTypeIndex(), std::move(methods) });
    SEQ_ASSERT_ARGUMENT( status.second, "Type '" << variant.getTypeName()
                         << "' already known to the manager" );

    try
    {
        auto hashStatus = mImpl->mHashMap.insert({ hash.value, variant.getTypeIndex() });
        SEQ_ASSERT_ARGUMENT( hashStatus.second, "Hash collision" );
    }
    catch ( ... )
    {
        mImpl->mMethods.erase( status.first );
        throw;
    }

}


void
VariantMethodsManager::remove( const std::type_index& type )
{
    auto it = mImpl->mMethods.find( type );
    SEQ_ASSERT_ARGUMENT( mImpl->mMethods.end() != it, "Type '"
            << utils::demangle(type.name()) << "' not known to the manager" );
    auto hash = Hash{ std::hash<std::string>{}(it->second->getName()) };
    mImpl->mMethods.erase(it);

    auto hashIt = mImpl->mHashMap.find( hash.value );
    SEQ_ASSERT_ARGUMENT( mImpl->mHashMap.end() != hashIt, "Type hash '"
                         << utils::demangle(type.name()) << "' not known to the manager" );
    mImpl->mHashMap.erase( hashIt );
}

bool
VariantMethodsManager::has( const std::type_index& type ) const
{
    return mImpl->mMethods.end() != mImpl->mMethods.find( type );
}

const IVariantMethods&
VariantMethodsManager::get( const std::type_index& type ) const
{
    auto it = mImpl->mMethods.find( type );
    SEQ_ASSERT_ARGUMENT( mImpl->mMethods.end() != it, "Type '"
            << utils::demangle(type.name()) << "' not known to the manager" );
    return *it->second;
}

const IVariantMethods&
VariantMethodsManager::get( const Hash& hash ) const
{
    auto hashIt = mImpl->mHashMap.find( hash.value );
    SEQ_ASSERT_ARGUMENT( mImpl->mHashMap.end() != hashIt, "Has not registered" );

    auto methodsIt = mImpl->mMethods.find( hashIt->second );
    SEQ_ASSERT_ARGUMENT( mImpl->mMethods.end() != methodsIt, "Method not found" );
    return *methodsIt->second;
}

std::vector<std::type_index>
VariantMethodsManager::getTypes() const
{
    std::vector<std::type_index> ret;
    ret.reserve( mImpl->mMethods.size() );
    std::transform( mImpl->mMethods.begin(), mImpl->mMethods.end(), std::back_inserter(ret),
                    [](const auto& item)
                    {
                        return item.first;
                    } );
    return ret;
}

} // end namespace workflow::type
