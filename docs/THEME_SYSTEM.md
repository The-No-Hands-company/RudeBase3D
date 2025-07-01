# Theme System Documentation

## 🎨 **Overview**

RudeBase3D features a comprehensive theme system that provides professional dark mode support, real-time theme switching, and system integration. The theme system is built around a flexible architecture that supports multiple theme variants and user customization.

## ✨ **Features**

### **Multiple Built-in Themes**
- **Light Theme** - Clean, professional light interface
- **Modern Dark** - Sleek dark theme with blue accents
- **High Contrast Dark** - Enhanced contrast for accessibility
- **Warm Dark** - Dark theme with warm color tones
- **Purple Dark** - Dark theme with purple accent colors
- **Cool Blue Dark** - Dark theme with cool blue styling

### **Smart System Integration**
- **Auto-Detection** - Automatically detects and follows system dark mode preference
- **Real-time Switching** - Theme changes apply immediately without restart
- **Theme Persistence** - Remembers user's theme choice across sessions
- **Theme Recommendations** - Suggests appropriate themes based on system settings

### **User Interface**
- **Theme Selector** - Integrated dropdown in main application toolbar
- **Auto-Detect Toggle** - One-click option to follow system theme
- **Visual Preview** - Real-time preview of theme changes
- **Professional Styling** - Theme selector itself follows current theme

## 🏗️ **Architecture**

### **Core Components**

#### **ThemeManager** (`src/ui/core/ThemeManager.h/.cpp`)
The central theme management system that handles:
- Theme registration and storage
- Theme switching and application
- System theme detection
- Color palette management
- Stylesheet generation

```cpp
class ThemeManager : public QObject {
    Q_OBJECT
public:
    // Theme management
    void setTheme(const QString& themeName);
    QStringList getAvailableThemes() const;
    QString getCurrentTheme() const;
    
    // System integration
    void setAutoDetectSystemTheme(bool enabled);
    bool isAutoDetectEnabled() const;
    
    // Theme discovery
    QStringList getDarkThemes() const;
    QString getRecommendedTheme() const;
    
signals:
    void themeChanged(const QString& themeName);
    void autoDetectChanged(bool enabled);
};
```

#### **ThemeSelector** (`src/ui/components/ThemeSelector.h/.cpp`)
User interface component for theme selection:
- Dropdown list of available themes
- Auto-detect system theme toggle
- Real-time theme preview
- Professional styling integration

#### **BaseComponent** (`src/ui/components/BaseComponent.h/.cpp`)
Foundation class for all themed UI components:
- Automatic theme integration
- State management (Normal, Hover, Focus, etc.)
- Theme-aware styling
- Consistent behavior across components

## 🎨 **Theme Structure**

Each theme is defined with semantic color roles:

```cpp
struct Theme {
    QString name;
    QColor background;           // Main application background
    QColor backgroundAlternate;  // Secondary background for panels
    QColor foreground;           // Primary text color
    QColor primary;              // Brand/accent color
    QColor secondary;            // Secondary accent color
    QColor success;              // Success state color
    QColor warning;              // Warning state color
    QColor danger;               // Error/danger state color
    QColor border;               // Border and separator color
    QColor hover;                // Hover state overlay
    QColor selection;            // Selection highlight color
    QColor disabled;             // Disabled element color
};
```

## 🔧 **Usage**

### **For Developers**

#### **Applying Themes to Components**
```cpp
class MyComponent : public BaseComponent {
public:
    MyComponent(QWidget* parent = nullptr) : BaseComponent(parent) {
        // Component automatically receives theme updates
        connect(ThemeManager::instance(), &ThemeManager::themeChanged,
                this, &MyComponent::updateTheme);
    }
    
private slots:
    void updateTheme() {
        // Custom theme application logic
        setStyleSheet(ThemeManager::instance()->getStyleSheet("MyComponent"));
    }
};
```

#### **Creating Custom Themes**
```cpp
// Register a new theme
Theme customTheme;
customTheme.name = "Custom Dark";
customTheme.background = QColor(30, 30, 30);
customTheme.foreground = QColor(255, 255, 255);
// ... set other colors ...

ThemeManager::instance()->registerTheme(customTheme);
```

### **For Users**

#### **Changing Themes**
1. Click the theme selector dropdown in the main toolbar
2. Choose from available themes
3. Theme applies immediately

#### **Auto-Detect System Theme**
1. Toggle the "Auto-detect" checkbox in the theme selector
2. Application will follow your system's light/dark mode setting
3. Manual theme selection is disabled when auto-detect is enabled

## 🔄 **Integration Points**

### **Main Application**
- Theme selector integrated into main toolbar via `UIManager`
- Main window applies theme styling automatically
- All panels and components receive theme updates

### **Component System**
- All UI components inherit from `BaseComponent` for automatic theme support
- Custom components can override `updateTheme()` for specific styling
- Theme changes propagate through Qt's signal/slot system

### **Styling System**
- Automatic CSS stylesheet generation based on theme colors
- Component-specific stylesheets for detailed control
- Hover, focus, and state-specific styling

## 🔮 **Future Enhancements**

### **Planned Features**
- **Custom Theme Creator** - GUI for creating and editing themes
- **Theme Import/Export** - Share themes between installations
- **Industry Themes** - Maya-like, Blender-like, 3ds Max-like themes
- **Accessibility Themes** - High contrast, colorblind-friendly options
- **Icon Theming** - Theme-aware icon system
- **Animation Support** - Smooth transitions between themes

### **Technical Improvements**
- **Performance Optimization** - Faster theme switching for complex UIs
- **Memory Efficiency** - Reduced memory usage for theme storage
- **Plugin Support** - Allow plugins to register custom themes
- **Advanced Customization** - Per-component theme overrides

## 📝 **Implementation Notes**

### **Build Requirements**
- Qt 6.5+ with full widget support
- C++23 compatible compiler
- No additional dependencies required

### **File Structure**
```
src/ui/
├── core/
│   ├── ThemeManager.h/.cpp     # Core theme management
│   └── UIManager.h/.cpp        # UI integration
└── components/
    ├── BaseComponent.h/.cpp    # Foundation class
    ├── ThemeSelector.h/.cpp    # Theme selection UI
    └── ThemedButton.h/.cpp     # Example themed component
```

### **Testing**
- All themes tested on Windows 10/11
- System dark mode integration verified
- Real-time switching performance validated
- Memory leak testing completed

## 🎯 **Best Practices**

### **For Component Development**
1. Always inherit from `BaseComponent` for automatic theme support
2. Use semantic color roles rather than hardcoded colors
3. Test components with all available themes
4. Implement proper hover and focus states

### **For Theme Creation**
1. Ensure sufficient contrast ratios for accessibility
2. Test with various UI components and states
3. Consider colorblind users when choosing colors
4. Maintain consistency with application's design language

## 📊 **Current Status**

- ✅ **Core Implementation** - Complete and stable
- ✅ **Multiple Dark Themes** - 5+ themes available
- ✅ **System Integration** - Auto-detection working
- ✅ **User Interface** - Theme selector integrated
- ✅ **Build Integration** - No build issues
- ✅ **Testing** - Functionality verified

**The theme system is production-ready and actively used in the main application.**

---

*Last Updated: July 1, 2025*  
*Status: ✅ Complete and Integrated*
