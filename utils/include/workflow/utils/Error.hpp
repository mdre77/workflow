#pragma once

#include <string>
#include <sstream>
#include <stdexcept>
#include <iosfwd>

#include <workflow/utils/ErrorCode.hpp>

#define SEQ_CAT(a, b) SEQ_CAT_I(a, b)
#define SEQ_CAT_I(a, b) SEQ_CAT_II(~, a ## b)
#define SEQ_CAT_II(p, res) res
#define SEQ_UNIQUE_NAME(base) SEQ_CAT(base, __LINE__)

#define UNIQUE_NAME(base) PP_CAT(base, __COUNTER__)

#define SEQ_ASSERT( expr, code, msg ) \
    do { \
        if ( !(expr) ) [[unlikely]] \
        { \
            std::stringstream SEQ_UNIQUE_NAME(ss); \
            SEQ_UNIQUE_NAME(ss) << msg; \
            throw ::workflow::utils::Error( __FILE__, __LINE__, __PRETTY_FUNCTION__, \
                                           SEQ_UNIQUE_NAME(ss).str(), \
                                           code ); \
        } \
    } while (0)

#define SEQ_ASSERT_ARGUMENT( expr, msg ) \
    SEQ_ASSERT( expr, ::workflow::utils::CommonError::InvalidArgument, msg )

#define SEQ_ASSERT_INVARIANT( expr, msg ) \
    SEQ_ASSERT( expr, ::workflow::utils::CommonError::InvariantFailed, msg )

namespace workflow::utils {

enum CommonError
{
    Success = 0,
    InvalidArgument,
    InvariantFailed,
};

/**
 * Common exception object containing additional diagnostic information
 */
class Error : public std::exception
{
public:
    /**
     * Create error
     *
     * @param [in]  file        File name
     * @param [in]  line        Line number
     * @param [in]  function    The function name
     * @param [in]  message     The message
     * @param [in]  errorCode   The error code
     */
    Error( const char* file,
           int line,
           const char* function,
           const std::string& message,
           const ErrorCode& errorCode = ErrorCode(CommonError::Success) );

    /**
     * Get file name
     */
    std::string
    getFile() const;

    /**
     * Get line number
     */
    int
    getLine() const;

    /**
     * Get function name
     */
    std::string
    getFunction() const;

    /**
     * Get message
     */
    std::string
    getMessage() const;

    /**
     * Get error code
     */
    ErrorCode
    getErrorCode() const;

    /**
     * Get stack trace
     */
    std::string
    getStackTrace() const;

    /**
     * Write error to output stream
     *
     * @param [in]  os      The stream
     * @param [in]  error   The error code
     *
     * @return The stream
     */
    friend std::ostream&
    operator<<( std::ostream& os,
                const Error& error );

private:
    std::string mFile;
    int         mLine;
    std::string mFunction;
    std::string mMessage;
    ErrorCode   mErrorCode;
    std::string mStackTrace;
};

} // end namespace workflow::utils