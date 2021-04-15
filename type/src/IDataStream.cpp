#include <workflow/type/IDataStream.hpp>

namespace workflow::type {

SEQ_INTERFACE_IMPL( IDataStream );

void
write( bool value );

void
write( uint8_t value );

void
write( uint16_t value );

void
write( uint32_t value );

void
write( uint64_t value );

void
write( int8_t value );

void
write( int16_t value );

void
write( int32_t value );

void
write( int64_t value );

void
write( float value );

void
write( double value );

void
write( const std::string& value );

void
read( bool& value );

void
read( uint8_t& value );

void
read( uint16_t& value );

void
read( uint32_t& value );

void
read( uint64_t& value );

void
read( int8_t& value );

void
read( int16_t& value );

void
read( int32_t& value );

void
read( int64_t& value );

void
read( float& value );

void
read( double& value );

void
read( std::string& value );


} // end namespace workflow::type