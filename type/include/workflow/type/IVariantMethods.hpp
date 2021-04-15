#pragma once

#include <string>

#include <workflow/utils/Macros.hpp>

#include <workflow/type/Variant.hpp>

namespace workflow::type {

class DataStream;

/**
 * Interface for defining methods how to convert variant from and to strings
 * and also how serialize and deserialize variants.
 */
class IVariantMethods
{
public:
    /**
     * Get the name of the data type. Do not use typeinfo.name()!
     */
    virtual std::string
    getName() const = 0;

    /**
     * Returns a default constructed variant, the implementation is responsible for.
     */
    virtual Variant
    create() const = 0;

    /**
     * Convert the variant to string
     *
     * @param [in]  value       The variant to convert
     *
     * @return String representation of the Variant
     */
    virtual std::string
    toString( const Variant& value ) const = 0;

    /**
     * Parse variant from string
     *
     * @param [in]  value       The string to parse
     *
     * @return Variant value
     */
    virtual Variant
    fromString( const std::string& value ) const = 0;

    /**
     * Serialize variant to data stream
     *
     * @param [in]  stream      The stream to write to
     * @param [in]  value       The value to write
     */
    virtual void
    serialize( DataStream& stream,
               const Variant& value ) const = 0;

    /**
     * Deserialize variant from data stream
     *
     * @param [in]  stream      The data stream
     * @param [out] value       The deserialized variant
     */
    virtual void
    deserialize( DataStream& stream,
                 Variant& value ) const = 0;

    SEQ_INTERFACE_DECL( IVariantMethods );
};

} // end namespace workflow::type