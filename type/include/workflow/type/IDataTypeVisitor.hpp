#pragma once

#include <workflow/utils/Macros.hpp>

namespace workflow::type {

class VariantDataType;
class StructDataType;
class VectorDataType;

/**
 * Visitor interface to visit data types
 */
class IDataTypeVisitor
{
public:
    /**
     * Visit variant data type
     *
     * @param [in]  type        Visited type
     */
    virtual void
    visit( VariantDataType& type ) = 0;

    /**
     * Visit struct data type
     *
     * @param [in]  type        Visited type
     */
    virtual void
    visit( StructDataType& type ) = 0;

    /**
     * Visit vector data type
     *
     * @param [in]  type        Visited type
     */
    virtual void
    visit( VectorDataType& type ) = 0;


    SEQ_INTERFACE_DECL( IDataTypeVisitor );
};

/**
 * Visitor interface to visit data types
 */
class IConstDataTypeVisitor
{
public:
    /**
     * Visit variant data type
     *
     * @param [in]  type        Visited type
     */
    virtual void
    visit( const VariantDataType& type ) = 0;

    /**
     * Visit struct data type
     *
     * @param [in]  type        Visited type
     */
    virtual void
    visit( const StructDataType& type ) = 0;

    /**
     * Visit vector data type
     *
     * @param [in]  type        Visited type
     */
    virtual void
    visit( const VectorDataType& type ) = 0;


    SEQ_INTERFACE_DECL( IConstDataTypeVisitor );
};

} // end namespace workflow::type
