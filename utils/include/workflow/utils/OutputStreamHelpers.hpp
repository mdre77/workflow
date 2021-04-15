#pragma once

#include <iostream>

namespace workflow::utils {

template <typename T, class = void>
struct is_streamable : std::false_type { };

template <typename T>
struct is_streamable<T, std::void_t<decltype(std::cout << *(T*)0)>>
        : std::true_type { };

/**
 * Helper to write arbitrary value to an output stream. It uses T's output stream
 * operators if available else it prints '...'
 *
 * @tparam T        The type to print
 */
template<typename T>
struct Printer
{
    /**
     * Create the printer
     *
     * @param [in]  value       The value to print
     */
    explicit
    Printer( const T& value )
        : mValue( value )
    {
    }

    /**
     * Write printer to output stream
     *
     * @param [in]  os          The stream
     * @param [in]  p           The printer
     *
     * @return The stream
     */
    friend std::ostream&
    operator<<( std::ostream& os,
                const Printer& p )
    {
        if constexpr ( is_streamable<T>::value )
        {
            os << p.mValue;
        }
        else
        {
            os << "...";
        }
        return os;
    }

    const T& mValue;
};

} // end namespace workflow::utils