#pragma once

#include <map>
#include <vector>
#include <string>

#include <workflow/type/IDataType.hpp>

namespace workflow::type {

/**
 * A composite data type, that contains other data types identified by its name.
 */
class StructDataType : public IDataType
{
public:
    using NamedTypes = std::map<std::string,IDataTypeSharedPtr>;

    /**
     * Create struct data type
     *
     * @param [in]  name            The name of the type
     * @param [in]  namedTypes      The child data types
     */
    StructDataType( const std::string& name,
                    const NamedTypes& namedTypes );

    /**
     * Create struct type from data stream.
     *
     * @param [in]  stream          The stream to read from.
     */
    explicit
    StructDataType( DataStream& stream );

    /**
     * Get list of all attribute names
     */
    std::vector<std::string>
    getAttributes() const;

    /**
     * Test if a child type is available
     *
     * @param [in]  name            The attribute name to test
     *
     * @return True if found, else false
     */
    bool
    has( const std::string& name ) const;

    /**
     * Get attribute.
     *
     * @param [in]  name            The attributes name.
     *
     * @return Reference to attribute
     */
    const IDataType&
    get( const std::string& name ) const;

    /**
     * Get attribute.
     *
     * @param [in]  name            The attributes name.
     *
     * @return Reference to attribute
     */
    IDataType&
    get( const std::string& name );

    /**
     * Test for equality
     *
     * @param [in]  lhs         Left operand
     * @param [in]  rhs         Right operand
     *
     * @return True if equal, else false
     */
    friend bool
    operator==( const StructDataType& lhs,
                const StructDataType& rhs );

    /**
     * Test for inequality
     *
     * @param [in]  lhs         Left operand
     * @param [in]  rhs         Right operand
     *
     * @return True if not equal, else false
     */
    friend bool
    operator!=( const StructDataType& lhs,
                const StructDataType& rhs );

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
                const StructDataType& value );

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
    std::string     mName;
    NamedTypes      mAttributes;
};

} // end namespace workflow::type
