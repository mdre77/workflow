#include <workflow/utils/Error.hpp>

#include <boost/stacktrace.hpp>

namespace workflow::utils {

Error::Error( const char* file,
              int line,
              const char* function,
              const std::string& message,
              const ErrorCode& errorCode )
    : mFile( file )
    , mLine( line )
    , mFunction( function )
    , mMessage( message )
    , mErrorCode( errorCode )
{
    SEQ_ASSERT_ARGUMENT( !mFile.empty(), "Invalid file" );

    auto stacktrace = boost::stacktrace::stacktrace( 1, 20 );
    std::stringstream ss;
    ss << stacktrace;
    mStackTrace = ss.str();
}

std::string
Error::getFile() const
{
    return mFile;
}

int
Error::getLine() const
{
    return mLine;
}

std::string
Error::getFunction() const
{
    return mFunction;
}

std::string
Error::getMessage() const
{
    return mMessage;
}

ErrorCode
Error::getErrorCode() const
{
    return mErrorCode;
}

std::string
Error::getStackTrace() const
{
    return mStackTrace;
}

std::ostream&
operator<<( std::ostream& os,
            const Error& error )
{
    os << error.mFile << ":" << error.mLine << ": [" << error.mErrorCode << "] "
       << error.mMessage;
    return os;
}

} // end namespace workflow::utils