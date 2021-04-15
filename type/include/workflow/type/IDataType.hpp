#pragma once

#include <memory>
#include <string>
#include <iosfwd>

#include <workflow/utils/Macros.hpp>

namespace workflow::type {

class DataStream;
class IDataTypeVisitor;
class IConstDataTypeVisitor;

SEQ_POINTER_DECL( IDataType );

/**
 * Interface for all data types the workflow engine is able to use.
 */
class IDataType
{
public:
    /**
     * The enumeration to discriminate the implementations
     */
    enum class Type : uint32_t
    {
        Variant,    ///< Of type VariantDataType
        Struct,     ///< Of type StructDataType
        Vector,     ///< Of type VectorDataType
    };

    /**
     * Get the name of the data type.
     */
    virtual std::string
    getName() const = 0;

    /**
     * Get the type
     */
    virtual Type
    getType() const noexcept = 0;

    /**
     * Apply visitor to data type. Mutable version
     *
     * @param [in]  visitor     The visitor to accept
     */
    virtual void
    visit( IDataTypeVisitor& visitor ) = 0;

    /**
     * Apply visitor to data type. Const version
     *
     * @param [in]  visitor     The visitor to accept
     */
    virtual void
    visit( IConstDataTypeVisitor& visitor ) const = 0;

    /**
     * Serialize the data type
     *
     * @param [in]  stream      The stream to serialize to
     */
    virtual void
    serialize( DataStream& stream ) const = 0;

    /**
     * Compare for equality
     *
     * @param [in]  other       Type to compare with
     *
     * @return True if equal, else false
     */
    virtual bool
    equals( const IDataType& other ) const = 0;

    /**
     * Write to output stream
     *
     * @param [in]  os          The stream
     */
    virtual void
    output( std::ostream& os ) const = 0;

    /**
     * Serialization helper. It puts a discriminator to the stream
     *
     * @param [in]  stream      The stream
     * @param [in]  dataType    The data type
     */
    static void
    serialize( DataStream& stream,
               const IDataType& dataType );

    /**
     * Deserialization helper. It creates concrete instances depending on the type
     * discriminator.
     *
     * @param [in]  stream      The stream
     *
     * @return The data tyoe
     */
    static IDataTypeUniquePtr
    deserialize( DataStream& stream );

    /**
     * Test for equality
     *
     * @param [in]  lhs         Left operand
     * @param [in]  rhs         Right operand
     *
     * @return true if equal, else false
     */
    friend bool
    operator==( const IDataType& lhs,
                const IDataType& rhs );

    /**
     * Test for inequality
     *
     * @param [in]  lhs         Left operand
     * @param [in]  rhs         Right operand
     *
     * @return true if equal, else false
     */
    friend bool
    operator!=( const IDataType& lhs,
                const IDataType& rhs );

    /**
     * Write to output stream
     *
     * @param [in]  os          The stream
     * @param [in]  type        The type
     *
     * @return The stream
     */
    friend std::ostream&
    operator<<( std::ostream& os,
                const IDataType& type );

    SEQ_INTERFACE_DECL( IDataType );
};

} // end namespace namespace workflow::type