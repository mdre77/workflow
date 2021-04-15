#pragma once

#include <memory>
#include <string>
#include <vector>
#include <typeindex>

#include <workflow/utils/Macros.hpp>

namespace workflow::type {

class Variant;
SEQ_POINTER_DECL( IVariantMethods );

/**
 * Manger used to store IVariantMethods. All variants used by the engine must be
 * registered here. Teh class already provides the following basic variant types:
 * bool, uint8_t, uint16_t, uint32_t, uint64_t, int8_t, int16_t, int32_t,
 * int64_t, float, double, std::string
 */
class VariantMethodsManager
{
public:
    // Wrapped into a hash to indicate it is different than type_info's hash code
    struct Hash
    {
        std::uint64_t value;
    };

    /**
     * The destructor
     */
    ~VariantMethodsManager();

    /**
     * Method to access the singleton
     */
    static VariantMethodsManager&
    instance();

    /**
     * Calculate hash for a type. Does not works for unregistered Variant values.
     *
     * @param [in]  type       The type
     *
     * @return The has code
     */
    Hash
    calculateHash( const std::type_index& type ) const;

    /**
     * Calculate hash for type
     *
     * @tparam T    The type
     *
     * @return Type hash
     */
    template<typename T>
    Hash
    calculateHash() const;

    /**
     * Insert variant method implementation
     *
     * @param [in]  methods     Variant methods object
     */
    void
    insert( IVariantMethodsUniquePtr methods );

    /**
     * Remove method implementation
     *
     * @param [in]  type        Type index
     */
    void
    remove( const std::type_index& type );

    /**
     * Remove type
     *
     * @tparam T    ^The type to remove
     */
    template<typename T>
    void
    remove();

    /**
     * Test if type is registered
     *
     * @param [in]  type        Type index
     *
     * @return True if found, else false
     */
    bool
    has( const std::type_index& type ) const;

    /**
     * Test if type is available
     *
     * @tparam T        The type
     *
     * @return True if found, else false
     */
    template<typename T>
    bool
    has() const;

    /**
     * Get variant methods responsible for a Variant
     *
     * @param [in]  type        Type index
     *
     * @return Variant methods
     */
    const IVariantMethods&
    get( const std::type_index& type ) const;

    /**
     * Get variant methods responsible for a Variant
     *
     * @param [in]  hash        Type hash
     *
     * @return Variant methods
     */
    const IVariantMethods&
    get( const Hash& hash ) const;

    /**
     * Get methods to type
     *
     * @tparam T        The data type
     *
     * @return The methods
     */
    template<typename T>
    const IVariantMethods&
    get() const;

    /**
     * Get hash codes of all registered Variant types
     */
    std::vector<std::type_index>
    getTypes() const;

private:
    /**
     * Singleton. Constructor must be private
     */
    VariantMethodsManager();

    class Impl;
    std::unique_ptr<Impl> mImpl;
};

/******************************************************************************
 * Inlined implementations
 *****************************************************************************/
template<typename T>
VariantMethodsManager::Hash
VariantMethodsManager::calculateHash() const
{
    return calculateHash( typeid(T) );
}

template<typename T>
void
VariantMethodsManager::remove()
{
    remove( typeid(T) );
}

template<typename T>
bool
VariantMethodsManager::has() const
{
    return has( typeid(T) );
}

template<typename T>
const IVariantMethods&
VariantMethodsManager::get() const
{
    return get( typeid(T) );
}

} // end namespace workflow::type
