#pragma once

#include <vector>

#include <workflow/type/IDataType.hpp>
#include <workflow/type/Variant.hpp>

namespace workflow::type {

class VectorDataType : public IDataType
{
public:
    VectorDataType( const Variant& value );

    VectorDataType( DataStream& stream );

    using VariantVector = std::vector<Variant>;

    // WIP provide STL compatible interface. But: Do not expose in a way that
    // variants of a different type can be set

    /**
     * Test for equality
     *
     * @param [in]  lhs         Left operand
     * @param [in]  rhs         Right operand
     *
     * @return True if equal, else false
     */
    friend bool
    operator==( const VectorDataType& lhs,
                const VectorDataType& rhs );

    /**
     * Test for inequality
     *
     * @param [in]  lhs         Left operand
     * @param [in]  rhs         Right operand
     *
     * @return True if not equal, else false
     */
    friend bool
    operator!=( const VectorDataType& lhs,
                const VectorDataType& rhs );

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
    Variant         mType;
    VariantVector   mValues;
};

} // end namespace workflow::type