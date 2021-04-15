#include <workflow/type/DataStream.hpp>

#include <algorithm>

#ifdef __MINGW32__
#include <sys/param.h>
#endif

#include <workflow/utils/Error.hpp>

namespace workflow::type {
namespace {

enum class ByteOrder
{
    LittleEndian,
    BigEndian
};

constexpr ByteOrder NETWORK_BYTEORDER = ByteOrder::LittleEndian;

#ifdef __MINGW32__
#if (BYTE_ORDER == LITTLE_ENDIAN)
    constexpr ByteOrder HOST_BYTEORDER = ByteOrder::LittleEndian;
#else
    constexpr ByteOrder HOST_BYTEORDER = ByteOrder::Big;
#endif
#else
#   error Add compiler support
#endif

template<typename T>
void
byteSwap( T& value )
{
    uint8_t* begin = reinterpret_cast<uint8_t>( &value );
    uint8_t* end = reinterpret_cast<uint8_t>( &value ) + sizeof(T);
    std::reverse( begin, end );
}

template<enum ByteOrder HOST, enum ByteOrder DEST>
struct Swapper
{
    template<typename T>
    void
    operator()( T& value )
    {
        if constexpr ( HOST != DEST )
        {
            byteSwap<T>( value );
        }
    }
};

using ByteSwapper = Swapper<HOST_BYTEORDER, NETWORK_BYTEORDER>;

enum class SerializerTypes : uint8_t
{
    Invalid     = 255,
    Bool        = 0,
    UInt8,
    UInt16,
    UInt32,
    UInt64,
    SInt8,
    SInt16,
    SInt32,
    SInt64,
    IEEE32,     // float
    IEEE64      // double
};

template<enum SerializerTypes S, typename T>
void
doWrite( DataStream& stream,
         T value )
{
    constexpr uint8_t TYPE = static_cast<uint8_t>(S);

    stream.write( sizeof(TYPE), &TYPE );
    ByteSwapper()( value );
    stream.write( sizeof(value), &value );
}

template<enum SerializerTypes S, typename T>
void
doRead( DataStream& stream,
        T& value )
{
    uint8_t type = static_cast<uint8_t>(SerializerTypes::Invalid);
    stream.read( sizeof(type), &type );
    SEQ_ASSERT_INVARIANT( type == static_cast<uint8_t>(S),
                          "Invalid stream: Expected type '"
                          << static_cast<uint16_t>(S) << "' but got '"
                          << static_cast<uint16_t>(type)  << "'" );

    stream.read( sizeof(value), &value );
    ByteSwapper()( value );
}


} // end namespace

DataStream::DataStream( IDataStreamUniquePtr backend )
    : mBackend( std::move(backend) )
{
    SEQ_ASSERT_ARGUMENT( mBackend, "Invalid backend" );
}

void
DataStream::write( bool value )
{
    const uint8_t VALUE = value ? 1 : 0;
    doWrite<SerializerTypes::Bool>( *this, VALUE );
}

void
DataStream::write( uint8_t value )
{
    doWrite<SerializerTypes::UInt8>( *this, value );
}

void
DataStream::write( uint16_t value )
{
    doWrite<SerializerTypes::UInt16>( *this, value );
}

void
DataStream::write( uint32_t value )
{
    doWrite<SerializerTypes::UInt32>( *this, value );
}

void
DataStream::write( uint64_t value )
{
    doWrite<SerializerTypes::UInt64>( *this, value );
}

void
DataStream::write( int8_t value )
{
    doWrite<SerializerTypes::SInt8>( *this, value );
}

void
DataStream::write( int16_t value )
{
    doWrite<SerializerTypes::SInt16>( *this, value );
}

void
DataStream::write( int32_t value )
{
    doWrite<SerializerTypes::SInt32>( *this, value );
}

void
DataStream::write( int64_t value )
{
    doWrite<SerializerTypes::SInt64>( *this, value );
}

void
DataStream::write( float value )
{
    doWrite<SerializerTypes::IEEE32>( *this, value );
}

void
DataStream::write( double value )
{
    doWrite<SerializerTypes::IEEE64>( *this, value );
}

void
DataStream::write( const std::string& value )
{
    SEQ_ASSERT_ARGUMENT( value.size() < std::numeric_limits<uint32_t>::max(),
                         "String size exceeds 32bit limit" );

    uint32_t length = static_cast<uint32_t>( value.size() );
    write( length );
    write( length, value.data() );
}

void
DataStream::read( bool& value )
{
    uint8_t tmp = 0;
    doRead<SerializerTypes::Bool>( *this, tmp );
    SEQ_ASSERT_INVARIANT( tmp == 0 || tmp == 1, "Invalid stream. Unexpected byte value" );
    value = !!tmp;
}

void
DataStream::read( uint8_t& value )
{
    doRead<SerializerTypes::UInt8>( *this, value );
}

void
DataStream::read( uint16_t& value )
{
    doRead<SerializerTypes::UInt16>( *this, value );
}

void
DataStream::read( uint32_t& value )
{
    doRead<SerializerTypes::UInt32>( *this, value );
}

void
DataStream::read( uint64_t& value )
{
    doRead<SerializerTypes::UInt64>( *this, value );
}

void
DataStream::read( int8_t& value )
{
    doRead<SerializerTypes::SInt8>( *this, value );
}

void
DataStream::read( int16_t& value )
{
    doRead<SerializerTypes::SInt16>( *this, value );
}

void
DataStream::read( int32_t& value )
{
    doRead<SerializerTypes::SInt32>( *this, value );
}

void
DataStream::read( int64_t& value )
{
    doRead<SerializerTypes::SInt64>( *this, value );
}

void
DataStream::read( float& value )
{
    doRead<SerializerTypes::IEEE32>( *this, value );
}

void
DataStream::read( double& value )
{
    doRead<SerializerTypes::IEEE64>( *this, value );
}

void
DataStream::read( std::string& value )
{
    uint32_t length = 0;
    read( length );
    value.resize( length );
    read( length, value.data() );
}

void
DataStream::write( const size_t length,
                   const void* data )
{
    mBackend->write( length, data );
}

void
DataStream::read( const size_t length,
                  void* data )
{
     mBackend->read( length, data );
}

} // end namespace workflow::type