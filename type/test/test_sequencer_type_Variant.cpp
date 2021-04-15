#include <gtest/gtest.h>

#include <sstream>

#include <workflow/type/Variant.hpp>

TEST( test_sequencer_type_Variant, Construct )
{
    const int VALUE = 10;
    workflow::type::Variant variant(VALUE);
    ASSERT_EQ( VALUE, variant.get<decltype(VALUE)>());
}

TEST( test_sequencer_type_Variant, CopyConstruct )
{
    const int VALUE = 10;
    workflow::type::Variant variant(VALUE);

    workflow::type::Variant copy = variant;
    ASSERT_EQ( VALUE, copy.get<decltype(VALUE)>());
}

TEST( test_sequencer_type_Variant, MoveConstruct )
{
    const int VALUE = 10;
    workflow::type::Variant variant(VALUE);

    workflow::type::Variant copy = std::move(variant);
    ASSERT_EQ( VALUE, copy.get<decltype(VALUE)>());
}

TEST( test_sequencer_type_Variant, CopyAssign )
{
    const int VALUE = 10;
    workflow::type::Variant variant(VALUE);

    workflow::type::Variant copy;
    copy = variant;
    ASSERT_EQ( VALUE, copy.get<decltype(VALUE)>());
}

TEST( test_sequencer_type_Variant, MoveAssign )
{
    const int VALUE = 10;
    workflow::type::Variant variant(VALUE);

    workflow::type::Variant copy;
    copy = std::move(variant);
    ASSERT_EQ( VALUE, copy.get<decltype(VALUE)>());
}

TEST( test_sequencer_type_Variant, EmptyGet )
{
    workflow::type::Variant variant;
    ASSERT_THROW(variant.get<int>(), workflow::utils::Error );
}

TEST( test_sequencer_type_Variant, EmptySet )
{
    workflow::type::Variant variant;
    ASSERT_THROW(variant.set<int>(10), workflow::utils::Error );
}

TEST( test_sequencer_type_Variant, InvalidGet )
{
    workflow::type::Variant variant(1.0 );
    ASSERT_THROW(variant.get<int>(), workflow::utils::Error );
}

TEST( test_sequencer_type_Variant, InvalidSet )
{
    workflow::type::Variant variant(1.0 );
    ASSERT_THROW(variant.set<int>( 10 ), workflow::utils::Error );
}

TEST( test_sequencer_type_Variant, EqualsEmptyEmpty )
{
    workflow::type::Variant variantA, variantB;

    ASSERT_TRUE( variantA == variantB );
    ASSERT_FALSE( variantA != variantB );
}

TEST( test_sequencer_type_Variant, EqualsEmptyValid )
{
    workflow::type::Variant variantA, variantB(10);

    ASSERT_FALSE( variantA == variantB );
    ASSERT_TRUE( variantA != variantB );
}

TEST( test_sequencer_type_Variant, EqualsValidEmpty )
{
    workflow::type::Variant variantA(10), variantB;

    ASSERT_FALSE( variantA == variantB );
    ASSERT_TRUE( variantA != variantB );
}

TEST( test_sequencer_type_Variant, EqualsValidValid )
{
    workflow::type::Variant variantA(10), variantB(10), variantC(5);

    ASSERT_TRUE( variantA == variantB );
    ASSERT_FALSE( variantA != variantB );

    ASSERT_FALSE( variantA == variantC );
    ASSERT_TRUE( variantA != variantC );
}

TEST( test_sequencer_type_Variant, Empty )
{
    workflow::type::Variant variantA, variantB(10);

    ASSERT_TRUE( variantA.empty() );
    ASSERT_FALSE( variantB.empty() );
}

TEST( test_sequencer_type_Variant, Clear )
{
    workflow::type::Variant variant(10 );

    ASSERT_FALSE( variant.empty() );
    variant.clear();
    ASSERT_TRUE( variant.empty() );
}

TEST( test_sequencer_type_Variant, GetTypeIndex )
{
    workflow::type::Variant variant(10 );

    ASSERT_EQ( std::type_index(typeid(int)), variant.getTypeIndex() );
}

TEST( test_sequencer_type_Variant, GetTypeName )
{
    workflow::type::Variant variantA, variantB(10);

    ASSERT_EQ( "void", variantA.getTypeName() );
    ASSERT_EQ( "int", variantB.getTypeName() );
}

TEST( test_sequencer_type_Variant, OutputEmpty )
{
    workflow::type::Variant variant;
    std::stringstream ss;
    ss << variant;

    ASSERT_EQ( "<void>()", ss.str() );
}

TEST( test_sequencer_type_Variant, OutputPrintable )
{
    workflow::type::Variant variant(10);
    std::stringstream ss;
    ss << variant;

    ASSERT_EQ( "<int>(10)", ss.str() );
}

struct Foo
{
    int value;

    bool
    operator==( const Foo& other ) const
    {
        return value == other.value;
    }
};

TEST( test_sequencer_type_Variant, OutputUnprintable )
{
    Foo foo{ 10 };

    workflow::type::Variant variant(foo);
    std::stringstream ss;
    ss << variant;

    ASSERT_EQ( "<Foo>(...)", ss.str() );
}