#include "ThemeManager.h"
#include <QApplication>
#include <QDir>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QStandardPaths>
#include <QDebug>
#include <QStyleHints>
#include <QPalette>

#ifdef Q_OS_WIN
#include <QSettings>
#endif

namespace rudebase3d {
namespace ui {

ThemeManager* ThemeManager::s_instance = nullptr;

ThemeManager::ThemeManager(QObject* parent)
    : QObject(parent)
    , m_currentTheme("default")
{
    loadThemes();
    applyTheme(m_currentTheme);
}

ThemeManager* ThemeManager::instance()
{
    if (!s_instance) {
        s_instance = new ThemeManager();
    }
    return s_instance;
}

void ThemeManager::loadThemes()
{
    // Load built-in themes
    loadBuiltInThemes();
    
    // Load user themes from config directory
    QString configDir = QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation);
    QString themesDir = configDir + "/themes";
    
    if (QDir(themesDir).exists()) {
        loadThemesFromDirectory(themesDir);
    }
}

void ThemeManager::loadBuiltInThemes()
{
    // Default Light Theme
    Theme lightTheme;
    lightTheme.name = "Light";
    lightTheme.id = "light";
    lightTheme.colors[ColorRole::Background] = QColor(240, 240, 240);
    lightTheme.colors[ColorRole::BackgroundAlternate] = QColor(250, 250, 250);
    lightTheme.colors[ColorRole::Foreground] = QColor(40, 40, 40);
    lightTheme.colors[ColorRole::Primary] = QColor(0, 122, 255);
    lightTheme.colors[ColorRole::Secondary] = QColor(108, 117, 125);
    lightTheme.colors[ColorRole::Success] = QColor(40, 167, 69);
    lightTheme.colors[ColorRole::Warning] = QColor(255, 193, 7);
    lightTheme.colors[ColorRole::Error] = QColor(220, 53, 69);
    lightTheme.colors[ColorRole::Border] = QColor(206, 212, 218);
    lightTheme.colors[ColorRole::Shadow] = QColor(0, 0, 0, 25);
    lightTheme.colors[ColorRole::Highlight] = QColor(0, 122, 255, 50);
    lightTheme.colors[ColorRole::Selection] = QColor(0, 122, 255, 100);
    
    m_themes[lightTheme.id] = lightTheme;
    
    // Default Dark Theme
    Theme darkTheme;
    darkTheme.name = "Dark";
    darkTheme.id = "dark";
    darkTheme.colors[ColorRole::Background] = QColor(45, 45, 48);
    darkTheme.colors[ColorRole::BackgroundAlternate] = QColor(37, 37, 38);
    darkTheme.colors[ColorRole::Foreground] = QColor(241, 241, 241);
    darkTheme.colors[ColorRole::Primary] = QColor(10, 132, 255);
    darkTheme.colors[ColorRole::Secondary] = QColor(108, 117, 125);
    darkTheme.colors[ColorRole::Success] = QColor(48, 209, 88);
    darkTheme.colors[ColorRole::Warning] = QColor(255, 214, 10);
    darkTheme.colors[ColorRole::Error] = QColor(255, 69, 58);
    darkTheme.colors[ColorRole::Border] = QColor(83, 83, 85);
    darkTheme.colors[ColorRole::Shadow] = QColor(0, 0, 0, 100);
    darkTheme.colors[ColorRole::Highlight] = QColor(10, 132, 255, 50);
    darkTheme.colors[ColorRole::Selection] = QColor(10, 132, 255, 100);
    
    m_themes[darkTheme.id] = darkTheme;
    
    // Professional Blue Theme
    Theme blueTheme;
    blueTheme.name = "Professional Blue";
    blueTheme.id = "professional-blue";
    blueTheme.colors[ColorRole::Background] = QColor(32, 39, 51);
    blueTheme.colors[ColorRole::BackgroundAlternate] = QColor(41, 50, 65);
    blueTheme.colors[ColorRole::Foreground] = QColor(220, 221, 222);
    blueTheme.colors[ColorRole::Primary] = QColor(52, 152, 219);
    blueTheme.colors[ColorRole::Secondary] = QColor(127, 140, 141);
    blueTheme.colors[ColorRole::Success] = QColor(46, 204, 113);
    blueTheme.colors[ColorRole::Warning] = QColor(241, 196, 15);
    blueTheme.colors[ColorRole::Error] = QColor(231, 76, 60);
    blueTheme.colors[ColorRole::Border] = QColor(69, 90, 120);
    blueTheme.colors[ColorRole::Shadow] = QColor(0, 0, 0, 150);
    blueTheme.colors[ColorRole::Highlight] = QColor(52, 152, 219, 80);
    blueTheme.colors[ColorRole::Selection] = QColor(52, 152, 219, 120);
    
    m_themes[blueTheme.id] = blueTheme;
    
    // High Contrast Dark Theme (for accessibility)
    Theme highContrastDark;
    highContrastDark.name = "High Contrast Dark";
    highContrastDark.id = "high-contrast-dark";
    highContrastDark.colors[ColorRole::Background] = QColor(0, 0, 0);
    highContrastDark.colors[ColorRole::BackgroundAlternate] = QColor(20, 20, 20);
    highContrastDark.colors[ColorRole::Foreground] = QColor(255, 255, 255);
    highContrastDark.colors[ColorRole::Primary] = QColor(0, 150, 255);
    highContrastDark.colors[ColorRole::Secondary] = QColor(150, 150, 150);
    highContrastDark.colors[ColorRole::Success] = QColor(0, 255, 0);
    highContrastDark.colors[ColorRole::Warning] = QColor(255, 255, 0);
    highContrastDark.colors[ColorRole::Error] = QColor(255, 0, 0);
    highContrastDark.colors[ColorRole::Border] = QColor(100, 100, 100);
    highContrastDark.colors[ColorRole::Shadow] = QColor(0, 0, 0, 200);
    highContrastDark.colors[ColorRole::Highlight] = QColor(0, 150, 255, 80);
    highContrastDark.colors[ColorRole::Selection] = QColor(0, 150, 255, 150);
    
    m_themes[highContrastDark.id] = highContrastDark;
    
    // Modern Dark Theme (popular GitHub/VS Code style)
    Theme modernDark;
    modernDark.name = "Modern Dark";
    modernDark.id = "modern-dark";
    modernDark.colors[ColorRole::Background] = QColor(30, 30, 30);
    modernDark.colors[ColorRole::BackgroundAlternate] = QColor(40, 40, 40);
    modernDark.colors[ColorRole::Foreground] = QColor(212, 212, 212);
    modernDark.colors[ColorRole::Primary] = QColor(0, 122, 204);
    modernDark.colors[ColorRole::Secondary] = QColor(150, 150, 150);
    modernDark.colors[ColorRole::Success] = QColor(22, 163, 74);
    modernDark.colors[ColorRole::Warning] = QColor(245, 158, 11);
    modernDark.colors[ColorRole::Error] = QColor(239, 68, 68);
    modernDark.colors[ColorRole::Border] = QColor(60, 60, 60);
    modernDark.colors[ColorRole::Shadow] = QColor(0, 0, 0, 120);
    modernDark.colors[ColorRole::Highlight] = QColor(0, 122, 204, 60);
    modernDark.colors[ColorRole::Selection] = QColor(0, 122, 204, 120);
    
    m_themes[modernDark.id] = modernDark;
    
    // Warm Dark Theme (easier on eyes for long sessions)
    Theme warmDark;
    warmDark.name = "Warm Dark";
    warmDark.id = "warm-dark";
    warmDark.colors[ColorRole::Background] = QColor(42, 39, 37);
    warmDark.colors[ColorRole::BackgroundAlternate] = QColor(52, 48, 45);
    warmDark.colors[ColorRole::Foreground] = QColor(235, 219, 178);
    warmDark.colors[ColorRole::Primary] = QColor(131, 165, 152);
    warmDark.colors[ColorRole::Secondary] = QColor(168, 153, 132);
    warmDark.colors[ColorRole::Success] = QColor(142, 192, 124);
    warmDark.colors[ColorRole::Warning] = QColor(250, 189, 47);
    warmDark.colors[ColorRole::Error] = QColor(251, 73, 52);
    warmDark.colors[ColorRole::Border] = QColor(80, 73, 69);
    warmDark.colors[ColorRole::Shadow] = QColor(0, 0, 0, 100);
    warmDark.colors[ColorRole::Highlight] = QColor(131, 165, 152, 50);
    warmDark.colors[ColorRole::Selection] = QColor(131, 165, 152, 100);
    
    m_themes[warmDark.id] = warmDark;
    
    // Purple Dark Theme (creative/modern feel)
    Theme purpleDark;
    purpleDark.name = "Purple Dark";
    purpleDark.id = "purple-dark";
    purpleDark.colors[ColorRole::Background] = QColor(35, 31, 49);
    purpleDark.colors[ColorRole::BackgroundAlternate] = QColor(45, 40, 60);
    purpleDark.colors[ColorRole::Foreground] = QColor(230, 225, 240);
    purpleDark.colors[ColorRole::Primary] = QColor(147, 112, 219);
    purpleDark.colors[ColorRole::Secondary] = QColor(150, 140, 160);
    purpleDark.colors[ColorRole::Success] = QColor(102, 187, 106);
    purpleDark.colors[ColorRole::Warning] = QColor(255, 193, 7);
    purpleDark.colors[ColorRole::Error] = QColor(244, 67, 54);
    purpleDark.colors[ColorRole::Border] = QColor(80, 70, 100);
    purpleDark.colors[ColorRole::Shadow] = QColor(0, 0, 0, 120);
    purpleDark.colors[ColorRole::Highlight] = QColor(147, 112, 219, 60);
    purpleDark.colors[ColorRole::Selection] = QColor(147, 112, 219, 120);
    
    m_themes[purpleDark.id] = purpleDark;
}

void ThemeManager::loadThemesFromDirectory(const QString& directory)
{
    QDir dir(directory);
    QStringList filters;
    filters << "*.json";
    
    QStringList themeFiles = dir.entryList(filters, QDir::Files);
    
    for (const QString& fileName : themeFiles) {
        QString filePath = dir.absoluteFilePath(fileName);
        loadThemeFromFile(filePath);
    }
}

void ThemeManager::loadThemeFromFile(const QString& filePath)
{
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly)) {
        qWarning() << "Failed to open theme file:" << filePath;
        return;
    }
    
    QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
    if (!doc.isObject()) {
        qWarning() << "Invalid theme file format:" << filePath;
        return;
    }
    
    QJsonObject themeObj = doc.object();
    Theme theme = themeFromJson(themeObj);
    
    if (!theme.id.isEmpty()) {
        m_themes[theme.id] = theme;
    }
}

ThemeManager::Theme ThemeManager::themeFromJson(const QJsonObject& json)
{
    Theme theme;
    theme.name = json["name"].toString();
    theme.id = json["id"].toString();
    
    QJsonObject colorsObj = json["colors"].toObject();
    
    // Map color role names to enum values
    QMap<QString, ColorRole> roleMap = {
        {"background", ColorRole::Background},
        {"backgroundAlternate", ColorRole::BackgroundAlternate},
        {"foreground", ColorRole::Foreground},
        {"primary", ColorRole::Primary},
        {"secondary", ColorRole::Secondary},
        {"success", ColorRole::Success},
        {"warning", ColorRole::Warning},
        {"error", ColorRole::Error},
        {"border", ColorRole::Border},
        {"shadow", ColorRole::Shadow},
        {"highlight", ColorRole::Highlight},
        {"selection", ColorRole::Selection}
    };
    
    for (auto it = roleMap.constBegin(); it != roleMap.constEnd(); ++it) {
        if (colorsObj.contains(it.key())) {
            QString colorStr = colorsObj[it.key()].toString();
            QColor color(colorStr);
            if (color.isValid()) {
                theme.colors[it.value()] = color;
            }
        }
    }
    
    return theme;
}

QStringList ThemeManager::availableThemes() const
{
    QStringList themes;
    for (auto it = m_themes.constBegin(); it != m_themes.constEnd(); ++it) {
        themes << it.value().name;
    }
    return themes;
}

QString ThemeManager::currentTheme() const
{
    return m_currentTheme;
}

void ThemeManager::applyTheme(const QString& themeId)
{
    if (!m_themes.contains(themeId)) {
        qWarning() << "Theme not found:" << themeId;
        return;
    }
    
    m_currentTheme = themeId;
    const Theme& theme = m_themes[themeId];
    
    // Generate and apply Qt stylesheet
    QString styleSheet = generateStyleSheet(theme);
    if (QApplication* app = qobject_cast<QApplication*>(QApplication::instance())) {
        app->setStyleSheet(styleSheet);
    }
    
    emit themeChanged(themeId);
}

QColor ThemeManager::getColor(ColorRole role) const
{
    if (m_themes.contains(m_currentTheme)) {
        const Theme& theme = m_themes[m_currentTheme];
        if (theme.colors.contains(role)) {
            return theme.colors[role];
        }
    }
    
    // Fallback colors
    switch (role) {
        case ColorRole::Background: return QColor(240, 240, 240);
        case ColorRole::Foreground: return QColor(40, 40, 40);
        case ColorRole::Primary: return QColor(0, 122, 255);
        default: return QColor();
    }
}

QString ThemeManager::generateStyleSheet(const Theme& theme)
{
    QString styleSheet;
    
    // Main application styling
    styleSheet += QString(R"(
        QMainWindow {
            background-color: %1;
            color: %2;
        }
        
        QWidget {
            background-color: %1;
            color: %2;
        }
        
        QMenuBar {
            background-color: %3;
            border-bottom: 1px solid %4;
            padding: 2px;
        }
        
        QMenuBar::item {
            background-color: transparent;
            padding: 4px 8px;
            border-radius: 4px;
        }
        
        QMenuBar::item:selected {
            background-color: %5;
        }
        
        QMenu {
            background-color: %3;
            border: 1px solid %4;
            border-radius: 4px;
            padding: 4px;
        }
        
        QMenu::item {
            padding: 6px 20px;
            border-radius: 4px;
        }
        
        QMenu::item:selected {
            background-color: %5;
        }
        
        QToolBar {
            background-color: %3;
            border: none;
            spacing: 2px;
            padding: 4px;
        }
        
        QToolButton {
            background-color: transparent;
            border: none;
            padding: 6px;
            border-radius: 4px;
        }
        
        QToolButton:hover {
            background-color: %5;
        }
        
        QToolButton:pressed {
            background-color: %6;
        }
        
        QPushButton {
            background-color: %7;
            color: white;
            border: none;
            padding: 8px 16px;
            border-radius: 4px;
            font-weight: bold;
        }
        
        QPushButton:hover {
            background-color: %8;
        }
        
        QPushButton:pressed {
            background-color: %9;
        }
        
        QLineEdit {
            background-color: %1;
            border: 1px solid %4;
            border-radius: 4px;
            padding: 6px;
        }
        
        QLineEdit:focus {
            border-color: %7;
        }
        
        QTextEdit {
            background-color: %1;
            border: 1px solid %4;
            border-radius: 4px;
        }
        
        QScrollBar:vertical {
            background-color: %3;
            width: 12px;
            border-radius: 6px;
        }
        
        QScrollBar::handle:vertical {
            background-color: %4;
            border-radius: 6px;
            min-height: 20px;
        }
        
        QScrollBar::handle:vertical:hover {
            background-color: %5;
        }
        
        QTabWidget::pane {
            border: 1px solid %4;
            background-color: %1;
        }
        
        QTabBar::tab {
            background-color: %3;
            padding: 8px 16px;
            margin-right: 2px;
        }
        
        QTabBar::tab:selected {
            background-color: %1;
            border-bottom: 2px solid %7;
        }
        
        QDockWidget {
            background-color: %3;
            titlebar-close-icon: none;
            titlebar-normal-icon: none;
        }
        
        QDockWidget::title {
            background-color: %3;
            padding: 8px;
            border-bottom: 1px solid %4;
        }
    )")
    .arg(theme.colors.value(ColorRole::Background).name())           // %1
    .arg(theme.colors.value(ColorRole::Foreground).name())           // %2
    .arg(theme.colors.value(ColorRole::BackgroundAlternate).name())  // %3
    .arg(theme.colors.value(ColorRole::Border).name())               // %4
    .arg(theme.colors.value(ColorRole::Highlight).name())            // %5
    .arg(theme.colors.value(ColorRole::Selection).name())            // %6
    .arg(theme.colors.value(ColorRole::Primary).name())              // %7
    .arg(theme.colors.value(ColorRole::Primary).lighter(110).name()) // %8
    .arg(theme.colors.value(ColorRole::Primary).darker(110).name()); // %9
    
    return styleSheet;
}

void ThemeManager::applySystemTheme()
{
    if (isSystemDarkMode()) {
        applyTheme(getRecommendedDarkTheme());
    } else {
        applyTheme(getRecommendedLightTheme());
    }
}

bool ThemeManager::isSystemDarkMode() const
{
#ifdef Q_OS_WIN
    // Windows 10/11 dark mode detection
    QSettings settings("HKEY_CURRENT_USER\\Software\\Microsoft\\Windows\\CurrentVersion\\Themes\\Personalize", QSettings::NativeFormat);
    return settings.value("AppsUseLightTheme", 1).toInt() == 0;
#elif defined(Q_OS_MACOS)
    // macOS dark mode detection
    QStyleHints* hints = QApplication::styleHints();
    return hints->colorScheme() == Qt::ColorScheme::Dark;
#else
    // Linux/Unix - fallback to Qt's detection
    QStyleHints* hints = QApplication::styleHints();
    return hints->colorScheme() == Qt::ColorScheme::Dark;
#endif
}

QString ThemeManager::getRecommendedLightTheme() const
{
    return "light"; // Default light theme
}

QString ThemeManager::getRecommendedDarkTheme() const
{
    return "modern-dark"; // Modern dark is most popular
}

QStringList ThemeManager::getDarkThemes() const
{
    QStringList darkThemes;
    darkThemes << "dark" << "modern-dark" << "warm-dark" << "purple-dark" 
               << "high-contrast-dark" << "professional-blue";
    return darkThemes;
}

QStringList ThemeManager::getLightThemes() const
{
    QStringList lightThemes;
    lightThemes << "light";
    // Add more light themes here as they're created
    return lightThemes;
}

} // namespace ui
} // namespace rudebase3d
