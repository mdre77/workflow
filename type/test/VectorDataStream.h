#pragma once

#include <vector>
#include <algorithm>

#include <workflow/type/IDataStream.hpp>

class VectorStream : public workflow::type::IDataStream
{
public:

    /**************************************************************************
     * IDataStream pure virtual overrides
     *************************************************************************/
    virtual void
    write( const size_t length,
           const void* data ) override
    {
        SEQ_ASSERT_ARGUMENT( length, "Invalid length" );
        SEQ_ASSERT_ARGUMENT( data, "Invalid data pointer" );

        std::copy( reinterpret_cast<const uint8_t*>(data),
                   reinterpret_cast<const uint8_t*>(data) + length,
                   std::back_inserter(mData) );
    }

    virtual void
    read( const size_t length,
          void* data ) override
    {
        SEQ_ASSERT_ARGUMENT( length, "Invalid length" );
        SEQ_ASSERT_ARGUMENT( data, "Invalid data pointer" );
        SEQ_ASSERT_ARGUMENT( mReadPos + length <= mData.size(),
                             "Invalid read size" );

        std::copy( mData.begin() + mReadPos,
                   mData.begin() + mReadPos + length,
                   reinterpret_cast<uint8_t*>( data ) );
        mReadPos += length;
    }
private:
    size_t               mReadPos;
    std::vector<uint8_t> mData;
};