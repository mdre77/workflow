#pragma once

#include <string>
#include <memory>

#include <workflow/utils/Macros.hpp>

namespace workflow::type {

SEQ_POINTER_DECL( IDataStream );

/**
 * Interface to read and write data to a IDataStream. It supports the most common
 * data types.
 * If the end of the stream is reached, or a value cannot be read they must throw
 * an exception of type workflow::utils::Error.
 */
class IDataStream
{
public:
    /**
     * Write data
     *
     * @param [in]  length      Data length
     * @param [in]  data        Data pointer
     */
    virtual void
    write( const size_t length,
           const void* data ) = 0;

    /**
     * Read data
     *
     * @param [in]  length      Number of bytes to read
     * @param [in]  data        User supplied buffer to copy the data to
     */
    virtual void
    read( const size_t length,
          void* data ) = 0;

    SEQ_INTERFACE_DECL( IDataStream );
};

}// end namespace workflow::type