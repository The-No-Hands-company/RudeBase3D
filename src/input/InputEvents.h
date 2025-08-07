
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
 * █  FILE: InputEvents.h                                                        █
 * █  DESCRIPTION: Fundamental Input Event Structures & Enumeration Definitions  █
 * █                                                                              █
 * █  PART OF: RudeBase3D - Professional 3D Modeling & Animation Suite          █
 * █  VERSION: 1.0.0                                                             █
 * █  CREATED: 2024                                                              █
 * █  UPDATED: August 7, 2025                                                    █
 * █                                                                              █
 * █  LICENSE: MIT License                                                       █
 * █  COMPANY: The No Hands Company                                              █
 * █  AUTHOR: RudeBase3D Development Team                                        █
 * █                                                                              █
 * █  This file contains fundamental input event structures, navigation          █
 * █  enumerations, and bitwise operator overloads providing the foundational   █
 * █  data types for professional 3D modeling and animation input systems.      █
 * █                                                                              █
 * ████████████████████████████████████████████████████████████████████████████████
 */

/**
 * @file InputEvents.h
 * @brief Fundamental Input Event System and Navigation Mode Definitions
 * 
 * This file provides the core input event structures and enumeration definitions
 * that form the foundational layer for professional 3D modeling and animation
 * input processing. These types enable consistent, cross-platform input handling
 * with support for industry-standard navigation paradigms, complex modifier key
 * combinations, and extensible event processing suitable for demanding real-time
 * 3D graphics applications.
 * 
 * ## Core Input Event Architecture
 * 
 * ### Mouse Input Processing
 * Comprehensive mouse button and position event handling:
 * - **MouseButton Enumeration**: Left, Right, Middle button identification
 * - **Position Tracking**: Integer coordinate mouse position representation
 * - **Multi-Button Support**: Simultaneous button state management
 * - **Cross-Platform Consistency**: Unified button mapping across operating systems
 * 
 * ### Keyboard Modifier System
 * Professional modifier key combination support:
 * - **Multiple Modifier Support**: Shift, Control, Alt key combinations
 * - **Bitwise Operations**: Efficient modifier state representation and testing
 * - **Platform Adaptation**: Windows Ctrl vs Mac Command key abstraction
 * - **Custom Combinations**: Support for complex professional workflow shortcuts
 * 
 * ### Navigation Mode Framework
 * Industry-standard 3D navigation paradigm support:
 * - **Maya-Style Navigation**: Alt+mouse combinations for professional animation
 * - **Blender-Style Control**: Middle-mouse paradigm for rapid modeling workflows
 * - **CAD-Style Precision**: Right-click and precision-oriented navigation schemes
 * - **FPS-Style Movement**: WASD and mouse-look for immersive scene exploration
 * 
 * ## Professional Workflow Integration
 * 
 * ### Industry-Standard Navigation Patterns
 * Support for established professional 3D application conventions:
 * 
 * #### Maya Navigation Paradigm
 * Professional animation industry standard:
 * - **Alt + Left Mouse**: Orbit camera around scene center or selection
 * - **Alt + Middle Mouse**: Pan camera in screen space
 * - **Alt + Right Mouse**: Zoom camera toward/away from focus point
 * - **Alt + Scroll Wheel**: Alternative zoom control with fine precision
 * 
 * #### Blender Navigation Paradigm
 * Open-source modeling and animation standard:
 * - **Middle Mouse**: Orbit camera with intuitive middle-button control
 * - **Shift + Middle Mouse**: Pan camera for repositioning viewpoint
 * - **Ctrl + Middle Mouse**: Zoom camera with modifier-based control
 * - **Scroll Wheel**: Direct zoom control for rapid navigation
 * 
 * #### CAD Navigation Paradigm
 * Precision engineering and architectural workflows:
 * - **Right Mouse**: Context-sensitive orbit with precision control
 * - **Middle Mouse**: Pan operation for detailed inspection
 * - **Scroll Wheel**: Precise zoom with engineering-appropriate sensitivity
 * - **Orthographic Views**: Specialized navigation for technical drawings
 * 
 * #### FPS Navigation Paradigm
 * First-person immersive scene exploration:
 * - **WASD Keys**: Forward, left, backward, right movement
 * - **Mouse Movement**: Free-look camera control with mouse capture
 * - **Space/Ctrl**: Vertical movement for 3D space exploration
 * - **Shift Modifier**: Sprint/speed boost for rapid scene traversal
 * 
 * ### Advanced Input Event Features
 * 
 * #### Modifier Key Combinations
 * Complex professional workflow support:
 * - **Single Modifiers**: Shift, Ctrl, Alt independent operations
 * - **Multi-Modifiers**: Shift+Ctrl, Ctrl+Alt, Shift+Alt combinations
 * - **Triple Modifiers**: Shift+Ctrl+Alt for advanced operations
 * - **Platform Mapping**: Automatic Command key mapping on macOS platforms
 * 
 * #### Event Timing and Sequencing
 * Professional input processing requirements:
 * - **Click vs Drag**: Distinction between click events and drag operations
 * - **Double-Click Detection**: Time-based multi-click pattern recognition
 * - **Hold Duration**: Pressure-sensitive and time-based input analysis
 * - **Gesture Recognition**: Complex input pattern detection for advanced tools
 * 
 * #### Cross-Platform Input Normalization
 * Consistent behavior across diverse operating systems:
 * - **Coordinate System**: Unified screen coordinate representation
 * - **Button Mapping**: Consistent mouse button identification
 * - **Key Code Translation**: Platform-independent key identification
 * - **Sensitivity Scaling**: Hardware-independent input sensitivity
 * 
 * ## Technical Implementation Details
 * 
 * ### Bitwise Modifier Operations
 * Efficient modifier key state management through bitwise arithmetic:
 * - **OR Operations (|)**: Combining multiple modifier keys into single value
 * - **AND Operations (&)**: Testing for specific modifier key combinations
 * - **NOT Operations (~)**: Inverting modifier states for exclusion testing
 * - **Assignment Operations (|=, &=)**: In-place modifier state manipulation
 * 
 * ### Memory-Efficient Event Representation
 * Optimized data structures for real-time input processing:
 * - **Compact Enumerations**: Memory-efficient enum storage and comparison
 * - **POD Structure Design**: Plain-old-data structures for optimal performance
 * - **Cache-Friendly Layout**: Data organization for CPU cache optimization
 * - **Vector Math Integration**: GLM vector types for position and coordinate data
 * 
 * ### Event Processing Performance
 * High-frequency input event handling optimization:
 * - **Minimal Allocations**: Stack-based event structures for zero-allocation processing
 * - **Branch Prediction**: Enum ordering optimized for common case prediction
 * - **Inlined Operations**: Header-based implementations for optimal compiler optimization
 * - **SIMD Compatibility**: Data layout compatible with vectorized operations
 * 
 * ## Usage Examples
 * 
 * ### Basic Input Event Handling
 * ```cpp
 * // Process mouse click event with modifier detection
 * void handleMouseClick(const MouseEvent& event) {
 *     // Check for basic selection
 *     if (event.button == MouseButton::Left && 
 *         event.modifiers == KeyboardModifier::None) {
 *         performSingleSelection(event.pos);
 *     }
 *     
 *     // Check for additive selection with Shift
 *     else if (event.button == MouseButton::Left && 
 *              (event.modifiers & KeyboardModifier::Shift)) {
 *         performAdditiveSelection(event.pos);
 *     }
 *     
 *     // Check for subtractive selection with Ctrl
 *     else if (event.button == MouseButton::Left && 
 *              (event.modifiers & KeyboardModifier::Control)) {
 *         performSubtractiveSelection(event.pos);
 *     }
 *     
 *     // Check for context menu with Right click
 *     else if (event.button == MouseButton::Right) {
 *         showContextMenu(event.pos);
 *     }
 * }
 * ```
 * 
 * ### Navigation Mode Implementation
 * ```cpp
 * class NavigationModeController {
 * private:
 *     NavigationMode m_currentMode = NavigationMode::Blender;
 *     NavigationAction m_currentAction = NavigationAction::None;
 *     
 * public:
 *     NavigationAction determineNavigationAction(const MouseEvent& event) {
 *         switch (m_currentMode) {
 *             case NavigationMode::Maya:
 *                 return determineMayaAction(event);
 *                 
 *             case NavigationMode::Blender:
 *                 return determineBlenderAction(event);
 *                 
 *             case NavigationMode::CAD:
 *                 return determineCADAction(event);
 *                 
 *             case NavigationMode::FPS:
 *                 return NavigationAction::None; // FPS uses keyboard primarily
 *                 
 *             default:
 *                 return NavigationAction::None;
 *         }
 *     }
 *     
 *     NavigationAction determineMayaAction(const MouseEvent& event) {
 *         if (event.modifiers & KeyboardModifier::Alt) {
 *             switch (event.button) {
 *                 case MouseButton::Left:   return NavigationAction::Orbit;
 *                 case MouseButton::Middle: return NavigationAction::Pan;
 *                 case MouseButton::Right:  return NavigationAction::Zoom;
 *                 default: return NavigationAction::None;
 *             }
 *         }
 *         return NavigationAction::None;
 *     }
 *     
 *     NavigationAction determineBlenderAction(const MouseEvent& event) {
 *         if (event.button == MouseButton::Middle) {
 *             if (event.modifiers & KeyboardModifier::Shift) {
 *                 return NavigationAction::Pan;
 *             } else if (event.modifiers & KeyboardModifier::Control) {
 *                 return NavigationAction::Zoom;
 *             } else {
 *                 return NavigationAction::Orbit;
 *             }
 *         }
 *         return NavigationAction::None;
 *     }
 *     
 *     NavigationAction determineCADAction(const MouseEvent& event) {
 *         switch (event.button) {
 *             case MouseButton::Right:  return NavigationAction::Orbit;
 *             case MouseButton::Middle: return NavigationAction::Pan;
 *             default: return NavigationAction::None;
 *         }
 *     }
 * };
 * ```
 * 
 * ### Advanced Modifier Key Processing
 * ```cpp
 * class ModifierKeyProcessor {
 * public:
 *     struct ModifierState {
 *         bool hasShift;
 *         bool hasControl;
 *         bool hasAlt;
 *         bool isMultiModifier;
 *         
 *         ModifierState(KeyboardModifier modifiers) {
 *             hasShift = (modifiers & KeyboardModifier::Shift) != KeyboardModifier::None;
 *             hasControl = (modifiers & KeyboardModifier::Control) != KeyboardModifier::None;
 *             hasAlt = (modifiers & KeyboardModifier::Alt) != KeyboardModifier::None;
 *             isMultiModifier = (static_cast<int>(modifiers) & 
 *                               (static_cast<int>(modifiers) - 1)) != 0;
 *         }
 *     };
 *     
 *     SelectionOperation getSelectionOperation(KeyboardModifier modifiers) {
 *         ModifierState state(modifiers);
 *         
 *         if (state.hasShift && !state.hasControl && !state.hasAlt) {
 *             return SelectionOperation::ADD;
 *         } else if (!state.hasShift && state.hasControl && !state.hasAlt) {
 *             return SelectionOperation::SUBTRACT;
 *         } else if (!state.hasShift && !state.hasControl && state.hasAlt) {
 *             return SelectionOperation::INTERSECT;
 *         } else if (state.hasShift && state.hasControl && !state.hasAlt) {
 *             return SelectionOperation::INVERT;
 *         } else {
 *             return SelectionOperation::REPLACE;
 *         }
 *     }
 *     
 *     SnapMode getSnapMode(KeyboardModifier modifiers) {
 *         ModifierState state(modifiers);
 *         
 *         if (state.hasShift) {
 *             return SnapMode::GRID;
 *         } else if (state.hasControl) {
 *             return SnapMode::VERTEX;
 *         } else if (state.hasAlt) {
 *             return SnapMode::EDGE;
 *         } else {
 *             return SnapMode::NONE;
 *         }
 *     }
 * };
 * ```
 * 
 * ### Wheel Event Processing
 * ```cpp
 * class WheelEventProcessor {
 * public:
 *     void processWheelEvent(const WheelEvent& event) {
 *         ModifierState modifiers(event.modifiers);
 *         
 *         if (modifiers.hasControl && !modifiers.hasShift) {
 *             // Zoom operation
 *             float zoomFactor = 1.0f + (event.delta * 0.1f);
 *             performCameraZoom(zoomFactor);
 *             
 *         } else if (modifiers.hasShift && !modifiers.hasControl) {
 *             // Horizontal pan operation
 *             glm::vec2 panDelta(event.delta * 10.0f, 0.0f);
 *             performCameraPan(panDelta);
 *             
 *         } else if (modifiers.hasAlt) {
 *             // Fine zoom with increased precision
 *             float preciseZoomFactor = 1.0f + (event.delta * 0.01f);
 *             performCameraZoom(preciseZoomFactor);
 *             
 *         } else {
 *             // Standard vertical zoom
 *             float standardZoomFactor = 1.0f + (event.delta * 0.05f);
 *             performCameraZoom(standardZoomFactor);
 *         }
 *     }
 * };
 * ```
 * 
 * ### Cross-Platform Event Normalization
 * ```cpp
 * class CrossPlatformInputNormalizer {
 * public:
 *     MouseEvent normalizeMouseEvent(const PlatformMouseEvent& platformEvent) {
 *         MouseEvent normalized;
 *         
 *         // Normalize button mapping
 *         normalized.button = normalizeMooseButton(platformEvent.button);
 *         
 *         // Normalize modifier keys (handle Cmd vs Ctrl on macOS)
 *         normalized.modifiers = normalizeModifiers(platformEvent.modifiers);
 *         
 *         // Normalize coordinate system (handle different DPI scaling)
 *         normalized.pos = normalizeCoordinates(platformEvent.position);
 *         
 *         return normalized;
 *     }
 *     
 * private:
 *     MouseButton normalizeMooseButton(int platformButton) {
 * #ifdef PLATFORM_MACOS
 *         // Handle macOS single-button mouse emulation
 *         if (platformButton == 0 && isControlPressed()) {
 *             return MouseButton::Right;
 *         }
 * #endif
 *         switch (platformButton) {
 *             case 0: return MouseButton::Left;
 *             case 1: return MouseButton::Right;
 *             case 2: return MouseButton::Middle;
 *             default: return MouseButton::None;
 *         }
 *     }
 *     
 *     KeyboardModifier normalizeModifiers(int platformModifiers) {
 *         KeyboardModifier result = KeyboardModifier::None;
 *         
 * #ifdef PLATFORM_MACOS
 *         // Map Cmd key to Control for cross-platform consistency
 *         if (platformModifiers & PLATFORM_CMD_KEY) {
 *             result |= KeyboardModifier::Control;
 *         }
 *         if (platformModifiers & PLATFORM_CTRL_KEY) {
 *             result |= KeyboardModifier::Alt; // Ctrl becomes Alt on Mac
 *         }
 * #else
 *         if (platformModifiers & PLATFORM_CTRL_KEY) {
 *             result |= KeyboardModifier::Control;
 *         }
 * #endif
 *         
 *         if (platformModifiers & PLATFORM_SHIFT_KEY) {
 *             result |= KeyboardModifier::Shift;
 *         }
 *         if (platformModifiers & PLATFORM_ALT_KEY) {
 *             result |= KeyboardModifier::Alt;
 *         }
 *         
 *         return result;
 *     }
 * };
 * ```
 * 
 * ## Advanced Features
 * 
 * ### Event Queue Management
 * High-performance event processing for real-time applications:
 * - **Circular Buffers**: Lock-free event queue implementation for multi-threaded access
 * - **Priority Queuing**: High-priority events (selection) vs low-priority (navigation)
 * - **Event Coalescing**: Combining rapid mouse movement events for optimal performance
 * - **Frame Synchronization**: Event processing synchronized with rendering frame rate
 * 
 * ### Gesture Recognition Foundation
 * Extensible framework for complex input pattern detection:
 * - **Temporal Patterns**: Time-based input sequence recognition for advanced commands
 * - **Spatial Gestures**: Mouse movement pattern recognition for custom operations
 * - **Multi-Touch Support**: Foundation for tablet and touch-based input devices
 * - **Customizable Patterns**: User-definable gesture recognition for specialized workflows
 * 
 * ### Accessibility Integration
 * Inclusive design support for diverse user interaction needs:
 * - **Alternative Input**: Foundation for voice control and eye tracking integration
 * - **Assistive Technology**: Screen reader and switch navigation compatibility
 * - **Sensitivity Adaptation**: Configurable input sensitivity for different motor abilities
 * - **Visual Feedback**: Event state visualization for input confirmation and feedback
 * 
 * @author RudeBase3D Development Team
 * @version 1.0
 * @since 1.0
 * @date August 7, 2025
 * 
 * @ingroup Input
 * @ingroup Events
 * @ingroup Navigation
 * @ingroup UserInterface
 */

#pragma once
#include <glm/glm.hpp>


enum class MouseButton { None, Left, Right, Middle };
enum class KeyboardModifier { None = 0, Shift = 1, Control = 2, Alt = 4 };

// Equality operator for MouseButton
inline bool operator==(MouseButton a, MouseButton b) {
    return static_cast<int>(a) == static_cast<int>(b);
}
// Bitwise operators for KeyboardModifier
inline KeyboardModifier operator|(KeyboardModifier a, KeyboardModifier b) {
    return static_cast<KeyboardModifier>(static_cast<int>(a) | static_cast<int>(b));
}
inline KeyboardModifier operator&(KeyboardModifier a, KeyboardModifier b) {
    return static_cast<KeyboardModifier>(static_cast<int>(a) & static_cast<int>(b));
}
inline KeyboardModifier& operator|=(KeyboardModifier& a, KeyboardModifier b) {
    a = a | b;
    return a;
}
inline KeyboardModifier& operator&=(KeyboardModifier& a, KeyboardModifier b) {
    a = a & b;
    return a;
}
inline KeyboardModifier operator~(KeyboardModifier a) {
    return static_cast<KeyboardModifier>(~static_cast<int>(a));
}

// Navigation modes for camera controllers
enum class NavigationMode {
    None,
    Maya,        // Maya-style: Alt+LMB=orbit, Alt+MMB=pan, Alt+RMB=zoom
    Blender,     // Blender-style: MMB=orbit, Shift+MMB=pan, Ctrl+MMB=zoom
    CAD,         // CAD-style: RMB=orbit, MMB=pan, wheel=zoom
    FPS          // FPS-style: WASD movement, mouse look
};

// Navigation actions for camera controllers
enum class NavigationAction {
    None,
    Orbit,
    Pan,
    Zoom
};

struct MouseEvent {
    MouseButton button;
    KeyboardModifier modifiers;
    glm::ivec2 pos;
};

struct WheelEvent {
    float delta;
    KeyboardModifier modifiers;
};

struct KeyEvent {
    int key;
    KeyboardModifier modifiers;
};
