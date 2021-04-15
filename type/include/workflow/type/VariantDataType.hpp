#pragma once

#include <workflow/type/Variant.hpp>
#include <workflow/type/IDataType.hpp>

namespace workflow::type {

/**
 * A primitive type encapsulates all types that can be created with a Variant.
 * The engine will not be able to inspect these kind of types, So they must
 * be known to the engine.
 */
class VariantDataType : public IDataType
{
public:
    /**
     * Construct value from variant value
     *
     * @param [in]  value       The variant value
     */
    explicit
    VariantDataType(Variant value);

    /**
     * Construct from data stream
     *
     * @param [in]  stream      The data stream
     */
    explicit
    VariantDataType(DataStream& stream);

    /**
     * Get variant value
     */
    Variant
    get() const;

    /**
     * Set variant value. The construction type of the variant is not allowed to
     * change.
     *
     * @param [in]  value       The variant value
     */
    void
    set( Variant value );

    /**
     * Test for equality
     *
     * @param [in]  lhs         Left operand
     * @param [in]  rhs         Right operand
     *
     * @return True if equal, else false
     */
    friend bool
    operator==( const VariantDataType& lhs,
                const VariantDataType& rhs );

    /**
     * Test for inequality
     *
     * @param [in]  lhs         Left operand
     * @param [in]  rhs         Right operand
     *
     * @return True if not equal, else false
     */
    friend bool
    operator!=( const VariantDataType& lhs,
                const VariantDataType& rhs );

    /**
     * Write to output stream
     *
     * @param [in]  os      The stream
     * @param [in]  value   The value
     *
     * @return The stream
     */
    friend std::ostream&
    operator<<( std::ostream& os,
                const VariantDataType& value );

    /**************************************************************************
     * IDataType pure virtual overrides
     *************************************************************************/
    virtual std::string
    getName() const override;

    virtual Type
    getType() const noexcept override;

    virtual void
    visit( IDataTypeVisitor& visitor ) override;

    virtual void
    visit( IConstDataTypeVisitor& visitor ) const override;

    virtual void
    serialize( DataStream& stream ) const override;

    virtual bool
    equals( const IDataType& other ) const override;

    virtual void
    output( std::ostream& os ) const override;

private:
    Variant mValue;
};

/******************************************************************************
 * Inlined implementations
 *****************************************************************************/

} // end namespace workflow::type