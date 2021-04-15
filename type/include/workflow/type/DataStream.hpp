#pragma once

#include <workflow/type/IDataStream.hpp>

namespace workflow::type {

class DataStream : public IDataStream
{
public:
    DataStream( IDataStreamUniquePtr backend );

    /**
     * Write boolean value
     *
     * @param [in]  value       Value to write
     */
    void
    write( bool value );

    /**
     * Write uint8_t value
     *
     * @param [in]  value       Value to write
     */
    void
    write( uint8_t value );

    /**
     * Write uint16_t value
     *
     * @param [in]  value       Value to write
     */
    void
    write( uint16_t value );

    /**
     * Write uint32_t value
     *
     * @param [in]  value       Value to write
     */
    void
    write( uint32_t value );

    /**
     * Write uint64_t value
     *
     * @param [in]  value       Value to write
     */
    void
    write( uint64_t value );

    /**
     * Write int8_t value
     *
     * @param [in]  value       Value to write
     */
    void
    write( int8_t value );

    /**
     * Write int16_t value
     *
     * @param [in]  value       Value to write
     */
    void
    write( int16_t value );

    /**
     * Write int32_t value
     *
     * @param [in]  value       Value to write
     */
    void
    write( int32_t value );

    /**
     * Write int64_t value
     *
     * @param [in]  value       Value to write
     */
    void
    write( int64_t value );

    /**
     * Write float value
     *
     * @param [in]  value       Value to write
     */
    void
    write( float value );

    /**
     * Write double value
     *
     * @param [in]  value       Value to write
     */
    void
    write( double value );

    /**
     * Write string value
     *
     * @param [in]  value       Value to write
     */
    void
    write( const std::string& value );

    /**
     * Read boolean value
     *
     * @param [out] value       Value to read
     */
    void
    read( bool& value );

    /**
     * Read uint8_t value
     *
     * @param [out] value       Value to read
     */
    void
    read( uint8_t& value );

    /**
     * Read uint16_t value
     *
     * @param [out] value       Value to read
     */
    void
    read( uint16_t& value );

    /**
     * Read uint32_t value
     *
     * @param [out] value       Value to read
     */
    void
    read( uint32_t& value );

    /**
     * Read uint64_t value
     *
     * @param [out] value       Value to read
     */
    void
    read( uint64_t& value );

    /**
     * Read int8_t value
     *
     * @param [out] value       Value to read
     */
    void
    read( int8_t& value );

    /**
     * Read int16_t value
     *
     * @param [out] value       Value to read
     */
    void
    read( int16_t& value );

    /**
     * Read int32_t value
     *
     * @param [out] value       Value to read
     */
    void
    read( int32_t& value );

    /**
     * Read int64_t value
     *
     * @param [out] value       Value to read
     */
    void
    read( int64_t& value );

    /**
     * Read float value
     *
     * @param [out] value       Value to read
     */
    void
    read( float& value );

    /**
     * Read double value
     *
     * @param [out] value       Value to read
     */
    void
    read( double& value );

    /**
     * Read string value
     *
     * @param [out] value       Value to read
     */
    void
    read( std::string& value );

    /**************************************************************************
     * IDataStream pure virtual overrides
     *************************************************************************/
    virtual void
    write( const size_t length,
           const void* data ) override;

    virtual void
    read( const size_t length,
          void* data ) override;

private:
    IDataStreamUniquePtr mBackend;
};

} // end namespace workflow::type