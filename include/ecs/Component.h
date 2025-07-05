#pragma once

#include <typeinfo>
#include <cstdint>

namespace rude::ecs {

/**
 * @brief Base class for all components
 * 
 * Components in ECS are pure data containers. They should not contain
 * any logic - that belongs in Systems. This base class provides a
 * way to identify component types at runtime.
 */
class Component {
public:
    virtual ~Component() = default;
    
    /**
     * @brief Get a unique type ID for this component type
     * @return Type ID that's unique for each component type
     */
    template<typename T>
    static std::uint32_t getTypeId() {
        static std::uint32_t typeId = s_nextTypeId++;
        return typeId;
    }

private:
    static std::uint32_t s_nextTypeId;
};

/**
 * @brief Maximum number of component types supported
 */
constexpr std::uint32_t MAX_COMPONENTS = 32;

} // namespace rude::ecs
