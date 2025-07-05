#pragma once

#include <cstdint>

namespace rude::ecs {

/**
 * @brief Entity represents a unique identifier in the ECS system
 * 
 * An Entity is simply an ID that groups components together.
 * It's the "E" in ECS - a lightweight identifier that doesn't contain
 * any data itself, but serves as a key to look up components.
 */
using Entity = std::uint32_t;

/**
 * @brief Special constant representing an invalid/null entity
 */
constexpr Entity NULL_ENTITY = 0;

/**
 * @brief Maximum number of entities that can exist simultaneously
 */
constexpr std::uint32_t MAX_ENTITIES = 5000;

} // namespace rude::ecs
