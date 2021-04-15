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

TEST( test_sequencer_type_VariantDataType, ContructInvalid )
{
    ASSERT_THROW(VariantDataType( Variant(NotRegistered{})), workflow::utils::Error );
}

TEST( test_sequencer_type_VariantDataType, Construct )
{
    std::string string = "Hi";

    VariantDataType variant( (Variant(string)) );

    ASSERT_EQ( VariantDataType::Type::Variant,variant.getType() );
    ASSERT_EQ( "std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> >",
               variant.getName() );
}

TEST( test_sequencer_type_VariantDataType, Visitor )
{
    VisitorMock visitor;
    EXPECT_CALL( visitor, visit(Matcher<VariantDataType&>(_)) ). Times(1);
    EXPECT_CALL( visitor, visit(Matcher<StructDataType&>(_)) ). Times(0);
    EXPECT_CALL( visitor, visit(Matcher<VectorDataType&>(_)) ). Times(0);

    VariantDataType variant( (Variant(10)) );
    variant.visit(visitor );
}

TEST( test_sequencer_type_VariantDataType, ConstVisitor )
{
    ConstVisitorMock visitor;
    EXPECT_CALL( visitor, visit(Matcher<const VariantDataType&>(_)) ). Times(1);
    EXPECT_CALL( visitor, visit(Matcher<const StructDataType&>(_)) ). Times(0);
    EXPECT_CALL( visitor, visit(Matcher<const VectorDataType&>(_)) ). Times(0);

    VariantDataType variant( (Variant(10)) );
    variant.visit(visitor );
}

TEST( test_sequencer_type_VariantDataType, Streaming )
{
    DataStream stream( std::make_unique<VectorStream>() );

    VariantDataType input( (Variant(10)) );
    input.serialize( stream );

    auto output = VariantDataType( stream );
    ASSERT_EQ( input, output );
}

TEST( test_sequencer_type_VariantDataType, Equals )
{
    VariantDataType V1( (Variant(10)) );
    VariantDataType V2( (Variant(10)) );
    VariantDataType V3( (Variant(20.25)) );

    ASSERT_TRUE( V1.equals( V1 ) );
    ASSERT_TRUE( V1.equals( V2 ) );
    ASSERT_FALSE( V1.equals( V3 ) );
}

TEST( test_sequencer_type_VariantDataType, Output )
{
    VariantDataType V( (Variant(10)) );
    std::stringstream ss;
    ss << V;

    ASSERT_EQ( "Variant<int>(10)", ss.str() );
}