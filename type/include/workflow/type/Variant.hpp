#pragma once

#include <memory>
#include <typeindex>
#include <iosfwd>

#include <workflow/utils/Error.hpp>
#include <workflow/utils/Demangle.hpp>
#include <workflow/utils/OutputStreamHelpers.hpp>

namespace workflow::type {

/**
 * A simple variant class. Passing bare pointers to variants is not allowed.
 * All types must be comparable and default constructable.
 */
class Variant
{
public:
    /**
     * Create an empty variant
     */
    Variant() = default;

    /**
     * Copy construct a variant
     *
     * @param [in]  other       Variant to copy
     */
    Variant( const Variant& other );

    /**
     * Move construct a variant
     *
     * @param [in]  other       Variant to take from
     */
    Variant( Variant&& other ) noexcept;

    /**
     * Create variant from value
     * @tparam T                The variants value type
     * @param [in]  value       The value
     */
    template<typename T>
    explicit
    Variant( const T& value );

    /**
     * Default destructor
     */
    ~Variant() = default;

    /**
     * Assignment operator
     *
     * @param [in]  other       Variant to copy from
     *
     * @return Reference to variant.
     */
    Variant&
    operator=( const Variant& other );

    /**
     * Assignment operator
     *
     * @param [in]  other       Variant to take from
     *
     * @return Reference to variant.
     */
    Variant&
    operator=( Variant&& other ) noexcept;

    /**
     * Test if variant is empty
     */
    bool
    empty() const;

    /**
     * Clear variant
     */
    void
    clear();

    /**
     * Get value stored along with the variant
     *
     * @tparam T    The expected type
     */
    template<typename T>
    T
    get() const;

    /**
     * Set variant value. It must match the variants type.
     *
     * @tparam T    The values type
     * @param [in]  value       The value
     */
    template<typename T>
    void
    set( const T& value );

    /**
     * Get variants type index. It is the type of value stored inside.
     */
    std::type_index
    getTypeIndex() const;

    /**
     * Get the demangled type name of the stored value
     */
    std::string
    getTypeName() const;

    /**
     * Test for equality
     *
     * @param [in]  lhs     The left operand
     * @param [in]  rhs     The right operand
     *
     * @return True if equal, else false.
     */
    friend bool
    operator==( const Variant& lhs,
                const Variant& rhs );

    /**
     * Test for inequality
     *
     * @param [in]  lhs     The left operand
     * @param [in]  rhs     The right operand
     *
     * @return True if not equal, else false.
     */
    friend bool
    operator!=( const Variant& lhs,
                const Variant& rhs );

    /**
     * Write to output stream for logging purpose
     *
     * @param [in]  os      The stream
     * @param [in]  value   The value
     *
     * @return The stream
     */
    friend std::ostream&
    operator<<( std::ostream& os,
                const Variant& value );

private:
    class IValue;
    using IValuePtr = std::unique_ptr<IValue>;

    /**
     * The interface class to the internal values
     */
    struct IValue
    {
        /**
         * Get type index
         */
        virtual std::type_index
        getTypeIndex() const = 0;

        /**
         * Clone value
         */
        virtual IValuePtr
        clone() const = 0;

        /**
         * Test for equality
         *
         * @param [in]  other       The value
         *
         * @return True if equal, else false.
         */
        virtual bool
        equals( const IValue& other ) const = 0;

        /**
         * Write to output stream
         *
         * @param [in]  os          The stream
         */
        virtual void
        output( std::ostream& os ) const = 0;
    };

    /**
     * The concrete holder of the internal value
     * @tparam T        The internal values type
     */
    template<typename T>
    struct Value : public IValue
    {
        /**
         * Construct from a value
         *
         * @param [in]  value       The value.
         */
        explicit
        Value( const T& value );

        virtual std::type_index
        getTypeIndex() const override;

        virtual IValuePtr
        clone() const override;

        virtual bool
        equals( const IValue& other ) const override;

        virtual void
        output( std::ostream& os ) const override;

        T mValue;
    };
    IValuePtr mValue;
};

/******************************************************************************
 * Inlined implementations
 *****************************************************************************/
template<typename T>
Variant::Variant( const T& value )
    : mValue( std::make_unique<Value<std::decay_t<T>>>( value ) )
{
}

template<typename T>
T
Variant::get() const
{
    SEQ_ASSERT_INVARIANT( mValue, "Cannot access empty variant" );
    SEQ_ASSERT_ARGUMENT( mValue->getTypeIndex() == typeid(std::decay_t<T>),
                         "cannot convert variant of '"
                         << utils::demangle(mValue->getTypeIndex().name() )
                         << "' to type '" << utils::demangle(typeid(T).name())
                         << "'" );
    const auto& value = dynamic_cast<const Value<std::decay_t<T>>&>( *mValue );
    return value.mValue;
}

template<typename T>
void
Variant::set( const T& value )
{
    SEQ_ASSERT_INVARIANT( mValue, "Cannot access empty variant" );
    SEQ_ASSERT_ARGUMENT( mValue->getTypeIndex() == typeid(std::decay_t<T>),
                         "cannot convert variant of '"
                                 << utils::demangle(mValue->getTypeIndex().name() )
                                 << "' to type '" << utils::demangle(typeid(T).name())
                                 << "'" );
    auto& ref = dynamic_cast<Value<std::decay_t<T>>&>( *mValue );
    ref.mValue = value;
}

/*****************************************************************************/
template<typename T>
Variant::Value<T>::Value( const T& value )
    : mValue( value )
{
    static_assert( !std::is_pointer_v<T>, "Pointer are not allowed");
}

template<typename T>
std::type_index
Variant::Value<T>::getTypeIndex() const
{
    return typeid(T);
}

template<typename T>
Variant::IValuePtr
Variant::Value<T>::clone() const
{
    return std::make_unique<Value<T>>( mValue );
}

template<typename T>
bool
Variant::Value<T>::equals( const IValue& other ) const
{
    auto otherPtr = dynamic_cast<const Value<T>*>( &other );
    return otherPtr ? mValue == otherPtr->mValue : false;
}

template<typename T>
void
Variant::Value<T>::output( std::ostream& os ) const
{
    os << "<" << utils::demangle(typeid(T).name()) << ">("
       << utils::Printer<T>(mValue) << ")";
}

} // end namespace workflow::type