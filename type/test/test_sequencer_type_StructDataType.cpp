#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <workflow/type/VariantDataType.hpp>
#include <workflow/type/VectorDataType.hpp>
#include <workflow/type/StructDataType.hpp>
#include <workflow/type/IDataTypeVisitor.hpp>
#include <workflow/type/DataStream.hpp>

#include "VectorDataStream.h"

using namespace workflow::type;
using ::testing::Matcher;
using ::testing::_;

namespace {

struct VisitorMock : public IDataTypeVisitor
{
    MOCK_METHOD1( visit, void(VariantDataType&) );
    MOCK_METHOD1( visit, void(StructDataType&) );
    MOCK_METHOD1( visit, void(VectorDataType&) );
};

struct ConstVisitorMock : public IConstDataTypeVisitor
{
    MOCK_METHOD1( visit, void(const VariantDataType&) );
    MOCK_METHOD1( visit, void(const StructDataType&) );
    MOCK_METHOD1( visit, void(const VectorDataType&) );
};

struct NotRegistered
{
    friend bool
    operator==(const NotRegistered &lhs,
               const NotRegistered &rhs)
    {
        return false;
    }
};

}// end namespace

TEST( test_sequencer_type_StructDataType, ContructInvalid )
{
    ASSERT_THROW((StructDataType( "", {
            { "attr_A", std::make_shared<VariantDataType>(Variant(10)) }
    })), workflow::utils::Error );

    ASSERT_THROW((StructDataType( "Name", {}) ), workflow::utils::Error );
}

TEST( test_sequencer_type_StructDataType, Construct )
{
    const std::string NAME = "Name";
    const std::string ATTR_A = "attr_A";
    const std::string ATTR_B = "attr_B";
    const Variant VALUE_A( 10 );
    const Variant VALUE_B( 1.0 );

    StructDataType structType( NAME,
    {
        { ATTR_A, std::make_shared<VariantDataType>(Variant(10)) },
        { ATTR_B, std::make_shared<VariantDataType>(Variant(1.0)) }
    });
    ASSERT_EQ( VariantDataType::Type::Struct,structType.getType() );
    ASSERT_EQ( NAME, structType.getName() );
}

TEST( test_sequencer_type_StructDataType, GetAttributes )
{
    const std::string NAME = "Name";
    const std::string ATTR_A = "attr_A";
    const std::string ATTR_B = "attr_B";
    const Variant VALUE_A( 10 );
    const Variant VALUE_B( 1.0 );

    StructDataType structType( NAME,
    {
        { ATTR_A, std::make_shared<VariantDataType>(Variant(10)) },
        { ATTR_B, std::make_shared<VariantDataType>(Variant(1.0)) }
    });

    ASSERT_EQ( (std::vector<std::string>{ ATTR_A, ATTR_B }),
               structType.getAttributes() );
}

TEST( test_sequencer_type_StructDataType, Has )
{
    const std::string NAME = "Name";
    const std::string ATTR_A = "attr_A";
    const std::string ATTR_B = "attr_B";
    const Variant VALUE_A( 10 );
    const Variant VALUE_B( 1.0 );

    StructDataType structType( NAME,
    {
        { ATTR_A, std::make_shared<VariantDataType>(Variant(10)) },
        { ATTR_B, std::make_shared<VariantDataType>(Variant(1.0)) }
    });

    ASSERT_TRUE( structType.has( ATTR_A ) );
    ASSERT_TRUE( structType.has( ATTR_B ) );
    ASSERT_FALSE( structType.has( "does not exist" ) );
}

TEST( test_sequencer_type_StructDataType, Get )
{
    const std::string NAME = "Name";
    const std::string ATTR_A = "attr_A";
    const std::string ATTR_B = "attr_B";
    const Variant VALUE_A( 10 );
    const Variant VALUE_B( 1.0 );

    StructDataType structType( NAME,
    {
        { ATTR_A, std::make_shared<VariantDataType>(Variant(10)) },
        { ATTR_B, std::make_shared<VariantDataType>(Variant(1.0)) }
    });

    ASSERT_NO_THROW( structType.get(ATTR_A) );
    ASSERT_NO_THROW( const_cast<const StructDataType&>(structType).get(ATTR_A) );
    ASSERT_NO_THROW( structType.get(ATTR_B) );
    ASSERT_NO_THROW( const_cast<const StructDataType&>(structType).get(ATTR_B) );

    ASSERT_THROW(structType.get("does not exist"), workflow::utils::Error );
    ASSERT_THROW(const_cast<const StructDataType&>(structType).get("does not exist"),
                 workflow::utils::Error );
}

TEST( test_sequencer_type_StructDataType, Visitor )
{
    VisitorMock visitor;
    EXPECT_CALL( visitor, visit(Matcher<VariantDataType&>(_)) ). Times(0);
    EXPECT_CALL( visitor, visit(Matcher<StructDataType&>(_)) ). Times(1);
    EXPECT_CALL( visitor, visit(Matcher<VectorDataType&>(_)) ). Times(0);

    const std::string NAME = "Name";
    const std::string ATTR_A = "attr_A";
    const std::string ATTR_B = "attr_B";
    const Variant VALUE_A( 10 );
    const Variant VALUE_B( 1.0 );

    StructDataType structType( NAME,
    {
        { ATTR_A, std::make_shared<VariantDataType>(Variant(10)) },
        { ATTR_B, std::make_shared<VariantDataType>(Variant(1.0)) }
    });
    structType.visit(visitor );
}

TEST( test_sequencer_type_StructDataType, ConstVisitor )
{
    ConstVisitorMock visitor;
    EXPECT_CALL( visitor, visit(Matcher<const VariantDataType&>(_)) ). Times(0);
    EXPECT_CALL( visitor, visit(Matcher<const StructDataType&>(_)) ). Times(1);
    EXPECT_CALL( visitor, visit(Matcher<const VectorDataType&>(_)) ). Times(0);

    const std::string NAME = "Name";
    const std::string ATTR_A = "attr_A";
    const std::string ATTR_B = "attr_B";
    const Variant VALUE_A( 10 );
    const Variant VALUE_B( 1.0 );

    StructDataType structType( NAME,
    {
        { ATTR_A, std::make_shared<VariantDataType>(Variant(10)) },
        { ATTR_B, std::make_shared<VariantDataType>(Variant(1.0)) }
    });
    structType.visit(visitor );
}

TEST( test_sequencer_type_StructDataType, Streaming )
{
    DataStream stream( std::make_unique<VectorStream>() );

    const std::string NAME = "Name";
    const std::string ATTR_A = "attr_A";
    const std::string ATTR_B = "attr_B";
    const Variant VALUE_A( 10 );
    const Variant VALUE_B( 1.0 );
    StructDataType input( NAME,
    {
        { ATTR_A, std::make_shared<VariantDataType>(Variant(10)) },
        { ATTR_B, std::make_shared<VariantDataType>(Variant(1.0)) }
    });
    input.serialize( stream );

    auto output = StructDataType( stream );
    ASSERT_EQ( input, output );
}

TEST( test_sequencer_type_StructDataType, Equals )
{
    const std::string NAME_A = "NameA";
    const std::string NAME_B = "NameB";
    const std::string ATTR_A = "attr_A";
    const std::string ATTR_B = "attr_B";
    const Variant VALUE_A( 10 );
    const Variant VALUE_B( 1.0 );

    StructDataType valueA( NAME_A,
    {
        { ATTR_A, std::make_shared<VariantDataType>(Variant(10)) },
        { ATTR_B, std::make_shared<VariantDataType>(Variant(1.0)) }
    });
    StructDataType valueB( NAME_A,
    {
        { ATTR_A, std::make_shared<VariantDataType>(Variant(10)) },
        { ATTR_B, std::make_shared<VariantDataType>(Variant(1.0)) }
    });
    StructDataType valueC( NAME_B,
    {
        { ATTR_A, std::make_shared<VariantDataType>(Variant(10)) },
        { ATTR_B, std::make_shared<VariantDataType>(Variant(1.0)) }
    });
    StructDataType valueD( NAME_B,
    {
        { ATTR_A, std::make_shared<VariantDataType>(Variant(10.0)) },
        { ATTR_B, std::make_shared<VariantDataType>(Variant(1)) }
    });

    ASSERT_TRUE( valueA.equals(valueA) );
    ASSERT_TRUE( valueA.equals(valueB) );
    ASSERT_FALSE( valueA.equals(valueC) );
    ASSERT_FALSE( valueC.equals(valueD) );
}

TEST( test_sequencer_type_StructDataType, Output )
{
    const std::string NAME = "Name";
    const std::string ATTR_A = "attr_A";
    const std::string ATTR_B = "attr_B";
    const Variant VALUE_A( 10 );
    const Variant VALUE_B( 1.0 );
    StructDataType value( NAME,
    {
        { ATTR_A, std::make_shared<VariantDataType>(Variant(10)) },
        { ATTR_B, std::make_shared<VariantDataType>(Variant(1.0)) }
    });

    std::stringstream ss;
    ss << value;
    ASSERT_EQ( "Struct[Name](\n"
               "    attr_A=Variant<int>(10)\n"
               "    attr_B=Variant<double>(1)\n"
               ")", ss.str() );
}