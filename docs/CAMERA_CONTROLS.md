# RudeBase3D Camera Controls Documentation

This document provides a comprehensive guide to the camera navigation system in RudeBase3D, designed to provide professional-grade 3D modeling controls similar to industry-standard applications like Maya, Blender, and CAD software.

## Quick Start

- **Default Mode**: Maya-style navigation
- **Basic Orbit**: Hold `Alt` + drag with `Left Mouse Button`
- **Basic Pan**: Hold `Alt` + drag with `Middle Mouse Button`
- **Basic Zoom**: Hold `Alt` + drag with `Right Mouse Button` or use `Mouse Wheel`
- **Frame Scene**: Press `F` to fit all objects in view

---

## Navigation Modes

RudeBase3D supports four different navigation styles to match your preferred workflow:

### 🎯 Maya Style (Default)
Professional 3D animation software style controls.

| Action | Controls |
|--------|----------|
| **Orbit** | `Alt` + `Left Mouse Button` + drag |
| **Pan** | `Alt` + `Middle Mouse Button` + drag |
| **Dolly** | `Alt` + `Right Mouse Button` + drag |
| **Zoom** | `Mouse Wheel` |

### 🔷 Blender Style
Open-source 3D creation suite style controls.

| Action | Controls |
|--------|----------|
| **Orbit** | `Middle Mouse Button` + drag |
| **Pan** | `Shift` + `Middle Mouse Button` + drag |
| **Dolly** | `Ctrl` + `Middle Mouse Button` + drag |
| **Zoom** | `Mouse Wheel` |

### 📐 CAD Style
Computer-Aided Design software style controls.

| Action | Controls |
|--------|----------|
| **Orbit** | `Right Mouse Button` + drag |
| **Pan** | `Middle Mouse Button` + drag |
| **Zoom** | `Mouse Wheel` |

### 🎮 FPS Style
First-Person Shooter game style controls for immersive navigation.

| Action | Controls |
|--------|----------|
| **Mouse Look** | Hold `Right Mouse Button` + move mouse |
| **Move Forward** | `W` |
| **Move Backward** | `S` |
| **Move Left** | `A` |
| **Move Right** | `D` |
| **Move Up** | `Shift` + `Q` |
| **Move Down** | `Shift` + `E` |
| **Fast Movement** | Hold `Shift` + movement keys |

---

## Camera Modes

### 🌐 Orbit Mode
*Default mode for 3D modeling*

The camera orbits around a fixed pivot point, ideal for inspecting and modeling objects. The pivot point can be:
- World center (0,0,0)
- Scene bounding box center
- Selected object center
- Custom user-defined point

### ✈️ Fly Mode
*Free-flight camera for exploration*

Move the camera freely through 3D space like flying a drone. Perfect for architectural walkthroughs or exploring large scenes.

### 🎯 Focus Mode
*Automated camera positioning*

Smoothly animates the camera to frame objects or scenes with optimal viewing angles and distances.

---

## Keyboard Shortcuts

### Scene Navigation
| Key | Action |
|-----|--------|
| `F` | Frame entire scene |
| `Shift` + `F` | Frame selected object |
| `G` | Toggle grid visibility |

### Predefined Views
| Key | View |
|-----|------|
| `7` | Top view |
| `Ctrl` + `1` | Front view |
| `Ctrl` + `3` | Right view |
| `9` | Isometric view |

### Render Modes
| Key | Mode |
|-----|------|
| `1` | Wireframe |
| `2` | Solid |
| `3` | Solid + Wireframe |

### Transform Modes
| Key | Mode |
|-----|------|
| `Q` | Select |
| `W` | Translate (Move) |
| `E` | Rotate |
| `R` | Scale |

### Camera Controls
| Key | Action |
|-----|--------|
| `Ctrl` + `Tab` | Toggle Orbit ↔ Fly camera mode |

---

## Professional Features

### ✨ Smooth Animations
- Automated camera transitions when framing objects
- Configurable animation speed
- Smooth ease-in/ease-out motion curves

### 🛡️ Gimbal Lock Prevention
- Smart orbit controls prevent camera flipping
- Pitch angle clamping to avoid disorientation
- Maintains proper camera orientation

### ⚙️ Configurable Sensitivity
Independent sensitivity controls for:
- **Rotation Speed**: How fast the camera rotates during orbit
- **Pan Speed**: Movement sensitivity when panning
- **Zoom Speed**: Zoom in/out sensitivity
- **Movement Speed**: Flight speed in FPS mode
- **Animation Speed**: Speed of automated camera transitions

### 🔄 Y-Axis Inversion
- Optional inverted mouse Y-axis
- Matches user preferences from other 3D software
- Applies to both orbit and FPS modes

### 🖱️ Mouse Capture
- FPS-style mouse capture for immersive navigation
- Automatically hides cursor during mouse look
- Returns cursor to center for continuous rotation

---

## Settings & Customization

### Navigation Preferences
```cpp
// Example settings (configurable in preferences)
cameraController->setMovementSpeed(5.0f);        // Units per second
cameraController->setRotationSpeed(1.0f);        // Degrees per pixel
cameraController->setPanSpeed(0.01f);            // Pan sensitivity
cameraController->setZoomSpeed(0.1f);            // Zoom sensitivity
cameraController->setAnimationSpeed(2.0f);       // Animation speed multiplier
cameraController->setInvertY(false);             // Y-axis inversion
```

### Orbit Pivot Options
- **World Center**: Always orbit around origin (0,0,0)
- **Scene Center**: Orbit around the center of all objects
- **Selection**: Orbit around the currently selected object
- **Custom Pivot**: Set a specific point to orbit around

---

## Tips for Effective Navigation

### 🎯 For Precision Modeling
1. Use **Maya style** controls for familiar industry-standard navigation
2. Frame your object with `Shift` + `F` to focus on your work
3. Use predefined views (`7`, `Ctrl+1`, `Ctrl+3`) for orthographic precision
4. Set orbit pivot to **Selection** to always rotate around your current object

### 🏗️ For Architectural Visualization
1. Switch to **FPS mode** (`Ctrl` + `Tab`) for walkthrough navigation
2. Use `WASD` keys for smooth movement through your scene
3. Hold `Shift` for faster movement when traversing large spaces
4. Use `Right Mouse` for looking around while moving

### 🎨 For Artistic Exploration
1. Use **Blender style** if you're familiar with Blender workflows
2. Enable smooth animations for cinematic camera movements
3. Use `F` frequently to get overview shots of your entire scene
4. Experiment with different orbit pivot points for dynamic viewing angles

### ⚡ Performance Tips
- Lower animation speed on slower computers
- Disable mouse capture if experiencing input lag
- Use **CAD style** for simplified, efficient navigation on basic hardware

---

## Troubleshooting

### Camera Gets "Lost" or Disoriented
- Press `F` to frame the entire scene and reset your view
- Use predefined views (`7`, `9`) to get oriented
- Switch to **World Center** orbit pivot for consistent rotation point

### Controls Feel Too Sensitive/Slow
- Adjust sensitivity settings in preferences
- Different navigation modes have different feel - try switching modes
- Use `Shift` modifier for faster movement in FPS mode

### Mouse Capture Issues
- Press `Escape` or release `Right Mouse Button` to exit mouse capture
- Disable mouse capture in settings if problematic
- Ensure the viewport window has focus when using FPS controls

---

## Advanced Usage

### Custom Pivot Points
```cpp
// Set a custom orbit pivot point
cameraController->setOrbitMode(OrbitMode::CustomPivot);
cameraController->setCustomPivot(QVector3D(10.0f, 5.0f, 0.0f));
```

### Programmatic Camera Control
```cpp
// Animate to specific views
cameraController->setTopView(true);              // Animated
cameraController->focusOnObject(myObject, true); // Smooth transition
cameraController->frameScene(false);             // Instant
```

### Navigation Mode Switching
```cpp
// Change navigation style programmatically
inputController->setNavigationMode(NavigationMode::Blender);
```

---

*This camera system provides the flexibility and precision needed for professional 3D modeling work, allowing you to navigate around your models with the same fluidity and control found in industry-standard 3D software.*
