#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <optional>

#include <workflow/type/VariantMethodsManager.hpp>
#include <workflow/type/IVariantMethods.hpp>
#include <workflow/type/DataStream.hpp>

#include "VectorDataStream.h"

using ::testing::Return;
using namespace workflow::type;

class IVariantMethodsMock : public IVariantMethods
{
public:
    MOCK_CONST_METHOD0( getName, std::string() );
    MOCK_CONST_METHOD0( create, Variant() );
    MOCK_CONST_METHOD1( toString, std::string(const Variant&) );
    MOCK_CONST_METHOD1( fromString, Variant(const std::string&) );
    MOCK_CONST_METHOD2( serialize, void(DataStream&, const Variant&) );
    MOCK_CONST_METHOD2( deserialize, void(DataStream&, Variant&) );
};

TEST( test_sequencer_type_VariantMethodsManager, Instance )
{
    ASSERT_NO_THROW( auto& instance = VariantMethodsManager::instance() );
}

TEST( test_sequencer_type_VariantMethodsManager, CalculateHash )
{
    auto& instance = VariantMethodsManager::instance();
    auto hashA = instance.calculateHash( Variant(10).getTypeIndex() );
    ASSERT_NE( 0, hashA.value );

    auto hashB = instance.calculateHash( Variant(10.0).getTypeIndex() );
    ASSERT_NE( 0, hashB.value );

    ASSERT_NE( hashA.value, hashB.value );
}

TEST( test_sequencer_type_VariantMethodsManager, InsertInvalid )
{
    auto& instance = VariantMethodsManager::instance();
    ASSERT_THROW(instance.insert(nullptr), workflow::utils::Error );
}

struct Insert
{
    size_t value;

    friend bool
    operator==( const Insert& lhs,
                const Insert& rhs )
    {
        return lhs.value == rhs.value;
    }
};

TEST( test_sequencer_type_VariantMethodsManager, Insert )
{
    auto method = std::make_unique<IVariantMethodsMock>();

    EXPECT_CALL( *method, create() )
        .WillOnce( Return( Variant(Insert{10}) ) );
    EXPECT_CALL( *method, getName() )
            .WillRepeatedly( Return( std::string("Insert") ) );

    auto& instance = VariantMethodsManager::instance();
    instance.insert( std::move(method) );

    instance.remove<Insert>();
}

TEST( test_sequencer_type_VariantMethodsManager, Has )
{
    auto method = std::make_unique<IVariantMethodsMock>();

    EXPECT_CALL( *method, create() )
            .WillOnce( Return( Variant(Insert{10}) ) );
    EXPECT_CALL( *method, getName() )
            .WillRepeatedly( Return( std::string("Insert") ) );
    auto& instance = VariantMethodsManager::instance();

    ASSERT_FALSE( instance.has<Insert>() );
    instance.insert( std::move(method) );
    ASSERT_TRUE( instance.has<Insert>() );

    instance.remove<Insert>();
}

TEST( test_sequencer_type_VariantMethodsManager, RemoveInvalid )
{
    auto& instance = VariantMethodsManager::instance();

    ASSERT_THROW(instance.remove<Insert>(),
                 workflow::utils::Error );
}

TEST( test_sequencer_type_VariantMethodsManager, Remove )
{
    auto method = std::make_unique<IVariantMethodsMock>();

    EXPECT_CALL( *method, create() )
            .WillOnce( Return( Variant(Insert{10}) ) );
    EXPECT_CALL( *method, getName() )
            .WillRepeatedly( Return( std::string("Insert") ) );
    auto& instance = VariantMethodsManager::instance();
    instance.insert( std::move(method) );

    ASSERT_TRUE( instance.has<Insert>() );
    instance.remove<Insert>();
    ASSERT_FALSE( instance.has<Insert>() );
}

TEST( test_sequencer_type_VariantMethodsManager, GetInvalid )
{
    auto& instance = VariantMethodsManager::instance();

    ASSERT_THROW(instance.get<Insert>(),
                 workflow::utils::Error );
}

TEST( test_sequencer_type_VariantMethodsManager, Get )
{
    auto method = std::make_unique<IVariantMethodsMock>();

    EXPECT_CALL(*method, create())
            .WillOnce(Return(Variant(Insert{10})));
    EXPECT_CALL( *method, getName() )
            .WillRepeatedly( Return( std::string("Insert") ) );
    auto &instance = VariantMethodsManager::instance();
    instance.insert(std::move(method));

    ASSERT_NO_THROW( auto& methods = instance.get<Insert>() );

    instance.remove<Insert>();
}

TEST( test_sequencer_type_VariantMethodsManager, DefaultTypeBool )
{
    auto &instance = VariantMethodsManager::instance();
    auto& method = instance.get<bool>();

    ASSERT_EQ( "true", method.toString(Variant(true)) );
    ASSERT_EQ( "false", method.toString(Variant(false)) );
    ASSERT_EQ( true, method.fromString("true").get<bool>() );
    ASSERT_EQ( false, method.fromString("false").get<bool>() );
    ASSERT_THROW(method.fromString("hohoho"), workflow::utils::Error );

    DataStream dataStream( std::make_unique<VectorStream>() );

    const Variant input(true);
    method.serialize( dataStream, input );

    Variant output;
    method.deserialize( dataStream, output );

    ASSERT_EQ( input, output );
}

template<typename T>
void
TestDefaultType( const T value,
                 const std::string& stringValue,
                 std::optional<std::string> notConvertible,
                 std::optional<std::string> above,
                 std::optional<std::string> below )
{
    auto &instance = VariantMethodsManager::instance();
    auto& method = instance.get<T>();

    ASSERT_EQ( stringValue, method.toString(Variant(value)) );
    ASSERT_EQ( value, method.fromString(stringValue).template get<T>() );
    if ( notConvertible )
    {
        ASSERT_THROW(method.fromString(*notConvertible), workflow::utils::Error);
    }
    if ( above )
    {
        ASSERT_THROW(method.fromString(*above), workflow::utils::Error );
    }
    if ( below )
    {
        ASSERT_THROW(method.fromString(*below), workflow::utils::Error);
    }

    DataStream dataStream( std::make_unique<VectorStream>() );

    const Variant input(value);
    method.serialize( dataStream, input );

    Variant output;
    method.deserialize( dataStream, output );

    ASSERT_EQ( input, output );
}

TEST( test_sequencer_type_VariantMethodsManager, DefaultTypeUInt8  )
{
    TestDefaultType<uint8_t>( 10, "10", "not convertible", "256", "-1" );
}

TEST( test_sequencer_type_VariantMethodsManager, DefaultTypeUInt16  )
{
    TestDefaultType<uint16_t>( 10, "10", "not convertible", "65536", "-1" );
}

TEST( test_sequencer_type_VariantMethodsManager, DefaultTypeUInt32  )
{
    TestDefaultType<uint32_t>( 10, "10", "not convertible", "4294967296", "-1" );
}

TEST( test_sequencer_type_VariantMethodsManager, DefaultTypeUInt64  )
{
    TestDefaultType<uint32_t>( 10, "10", "not convertible", "18446744073709551616", "-1" );
}

TEST( test_sequencer_type_VariantMethodsManager, DefaultTypeSInt8  )
{
    TestDefaultType<int8_t>( 10, "10", "not convertible", "128", "-129" );
}

TEST( test_sequencer_type_VariantMethodsManager, DefaultTypeSInt16  )
{
    TestDefaultType<int16_t>( 10, "10", "not convertible", "32768", "-32769" );
}

TEST( test_sequencer_type_VariantMethodsManager, DefaultTypeSInt32  )
{
    TestDefaultType<int32_t>( 10, "10", "not convertible", "2147483648", "-2147483649" );
}

TEST( test_sequencer_type_VariantMethodsManager, DefaultTypeSInt64  )
{
    TestDefaultType<int64_t>( 10, "10", "not convertible", "9223372036854775808", "-9.223.372.036.854.775.809" );
}

TEST( test_sequencer_type_VariantMethodsManager, DefaultTypeFloat  )
{
    TestDefaultType<float>( 10.25f, "10.25", "not convertible", {}, {} );
}

TEST( test_sequencer_type_VariantMethodsManager, DefaultTypeDouble  )
{
    TestDefaultType<double>( 10.25f, "10.25", "not convertible", {}, {} );
}

TEST( test_sequencer_type_VariantMethodsManager, DefaultTypeString  )
{
    TestDefaultType<std::string>( "Test", "Test", {}, {}, {} );
}