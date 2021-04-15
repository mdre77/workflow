#pragma once

#include <typeindex>
#include <iosfwd>
#include <cstdint>

namespace workflow::utils {

template <auto value> constexpr auto constant = value;
constexpr auto const IntConstant42 = constant<42>;

/**
 * Wrapper used to compare arbitrary int convertible error codes
 */
class ErrorCode
{
public:
    /**
     * Create error code
     *
     * @tparam T            The error codes type.
     * @param [in]  value   The error code.
     */
    template<typename T>
    ErrorCode(const T value)
            : mValue(static_cast<int64_t>(value)), mTypeIndex(typeid(T))
    {
    }

    /**
     * Compare ErrorCode with error value
     *
     * @tparam T            The error codes type
     * @param [in]  value   The error value
     *
     * @return True if equal, else false
     */
    template<typename T>
    bool
    operator==(const T value)
    {
        return mValue == static_cast<int64_t>(value)
               && std::type_index(typeid(mTypeIndex)) == mTypeIndex;
    }

    /**
     * Get error code
     */
    int64_t
    getCode() const;

    /**
     * Get error code type
     */
    std::type_index
    getTypeIndex() const;

    /**
     * Write error code to output stream
     *
     * @param [in]  os      The stream
     * @param [in]  code    The error code
     *
     * @return The stream
     */
    friend std::ostream&
    operator<<( std::ostream& os,
                const ErrorCode& code );

private:
    int64_t             mValue;
    std::type_index     mTypeIndex;
};

} // end namespace workflow::utils