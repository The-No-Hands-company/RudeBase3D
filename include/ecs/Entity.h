/*
 * ██████╗ ██╗   ██╗██████╗ ███████╗██████╗  █████╗ ███████╗███████╗██████╗ ██████╗ 
 * ██╔══██╗██║   ██║██╔══██╗██╔════╝██╔══██╗██╔══██╗██╔════╝██╔════╝╚════██╗██╔══██╗
 * ██████╔╝██║   ██║██║  ██║█████╗  ██████╔╝███████║███████╗█████╗   █████╔╝██║  ██║
 * ██╔══██╗██║   ██║██║  ██║██╔══╝  ██╔══██╗██╔══██║╚════██║██╔══╝   ╚═══██╗██║  ██║
 * ██║  ██║╚██████╔╝██████╔╝███████╗██████╔╝██║  ██║███████║███████╗██████╔╝██████╔╝
 * ╚═╝  ╚═╝ ╚═════╝ ╚═════╝ ╚══════╝╚═════╝ ╚═╝  ╚═╝╚══════╝╚══════╝╚═════╝ ╚═════╝ 
 * 
 * Professional 3D Modeling & Animation Software
 * 
 * ████████████████████████████████████████████████████████████████████████████████
 * █                                                                              █
 * █  FILE: Entity.h                                                              █
 * █  DESCRIPTION: ECS Entity Identifier & Management System                     █
 * █                                                                              █
 * █  PART OF: RudeBase3D - Professional 3D Modeling & Animation Suite            █
 * █  VERSION: 1.0.0                                                              █
 * █  CREATED: 2024                                                               █
 * █  UPDATED: August 7, 2025                                                     █
 * █                                                                              █
 * █  LICENSE: MIT License                                                        █
 * █  COMPANY: The No Hands Company                                               █
 * █  AUTHOR: RudeBase3D Development Team                                         █
 * █                                                                              █
 * █  This file contains the Entity identifier types and management              █
 * █  functionality for the Entity Component System architecture                 █
 * █  used throughout the RudeBase3D application.                                █
 * █                                                                              █
 * ████████████████████████████████████████████████████████████████████████████████
 */

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
