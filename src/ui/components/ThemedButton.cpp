#include "ThemedButton.h"
#include "../core/ThemeManager.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPainter>
#include <QStyleOption>
#include <QMouseEvent>

namespace rudebase3d {
namespace ui {

ThemedButton::ThemedButton(QWidget* parent)
    : BaseComponent(parent)
{
    setupButton();
}

ThemedButton::ThemedButton(const QString& text, QWidget* parent)
    : BaseComponent(parent)
    , m_text(text)
{
    setupButton();
    setText(text);
}

ThemedButton::ThemedButton(const QIcon& icon, const QString& text, QWidget* parent)
    : BaseComponent(parent)
    , m_text(text)
    , m_icon(icon)
{
    setupButton();
    setIcon(icon);
    setText(text);
}

void ThemedButton::setupButton()
{
    m_button = new QPushButton(this);
    m_button->setAttribute(Qt::WA_TransparentForMouseEvents, true);
    
    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->addWidget(m_button);
    
    // Connect signals
    connect(this, &ThemedButton::clicked, [this]() {
        // Button click logic handled by parent class events
    });
    
    // Set default properties
    setFocusPolicy(Qt::StrongFocus);
    setCursor(Qt::PointingHandCursor);
    
    applyTheme();
}

void ThemedButton::setText(const QString& text)
{
    m_text = text;
    m_button->setText(text);
    update();
}

QString ThemedButton::text() const
{
    return m_text;
}

void ThemedButton::setIcon(const QIcon& icon)
{
    m_icon = icon;
    m_button->setIcon(icon);
    m_button->setIconSize(m_iconSize);
    update();
}

QIcon ThemedButton::icon() const
{
    return m_icon;
}

void ThemedButton::setButtonStyle(ButtonStyle style)
{
    m_buttonStyle = style;
    applyTheme();
}

ThemedButton::ButtonStyle ThemedButton::buttonStyle() const
{
    return m_buttonStyle;
}

void ThemedButton::setIconSize(const QSize& size)
{
    m_iconSize = size;
    m_button->setIconSize(size);
    update();
}

QSize ThemedButton::iconSize() const
{
    return m_iconSize;
}

void ThemedButton::applyTheme()
{
    BaseComponent::applyTheme();
    
    if (m_button) {
        QString styleSheet = getStyleSheetForState();
        m_button->setStyleSheet(styleSheet);
    }
}

QString ThemedButton::generateBaseStyleSheet() const
{
    return QString(); // Custom painting, no base stylesheet needed
}

void ThemedButton::onStateChanged(ComponentState oldState, ComponentState newState)
{
    BaseComponent::onStateChanged(oldState, newState);
    
    // Update button appearance based on state
    if (m_button) {
        QString styleSheet = getStyleSheetForState();
        m_button->setStyleSheet(styleSheet);
    }
}

QString ThemedButton::getStyleSheetForState() const
{
    QColor backgroundColor = getBackgroundColorForState();
    QColor textColor = getTextColorForState();
    QColor borderColor = backgroundColor.darker(120);
    
    QString hoverColor = backgroundColor.lighter(110).name();
    QString pressedColor = backgroundColor.darker(110).name();
    
    return QString(R"(
        QPushButton {
            background-color: %1;
            color: %2;
            border: 1px solid %3;
            border-radius: 6px;
            padding: 8px 16px;
            font-weight: 500;
            font-size: 14px;
            text-align: center;
        }
        
        QPushButton:hover {
            background-color: %4;
        }
        
        QPushButton:pressed {
            background-color: %5;
        }
        
        QPushButton:disabled {
            background-color: #cccccc;
            color: #888888;
            border-color: #dddddd;
        }
    )")
    .arg(backgroundColor.name())
    .arg(textColor.name())
    .arg(borderColor.name())
    .arg(hoverColor)
    .arg(pressedColor);
}

QColor ThemedButton::getBackgroundColorForState() const
{
    ThemeManager* themeManager = ThemeManager::instance();
    
    switch (m_buttonStyle) {
        case ButtonStyle::Primary:
            return themeManager->getColor(ThemeManager::ColorRole::Primary);
        case ButtonStyle::Secondary:
            return themeManager->getColor(ThemeManager::ColorRole::Secondary);
        case ButtonStyle::Success:
            return themeManager->getColor(ThemeManager::ColorRole::Success);
        case ButtonStyle::Warning:
            return themeManager->getColor(ThemeManager::ColorRole::Warning);
        case ButtonStyle::Danger:
            return themeManager->getColor(ThemeManager::ColorRole::Error);
        case ButtonStyle::Ghost:
            return QColor(0, 0, 0, 0); // Transparent
        default:
            return themeManager->getColor(ThemeManager::ColorRole::Primary);
    }
}

QColor ThemedButton::getTextColorForState() const
{
    switch (m_buttonStyle) {
        case ButtonStyle::Ghost:
            return ThemeManager::instance()->getColor(ThemeManager::ColorRole::Primary);
        default:
            return QColor(255, 255, 255); // White text for colored buttons
    }
}

void ThemedButton::paintEvent(QPaintEvent* event)
{
    Q_UNUSED(event)
    
    // Custom painting if needed for ghost button or special effects
    if (m_buttonStyle == ButtonStyle::Ghost) {
        QPainter painter(this);
        painter.setRenderHint(QPainter::Antialiasing);
        
        QRect buttonRect = rect().adjusted(1, 1, -1, -1);
        
        // Draw border for ghost button
        QPen pen;
        pen.setColor(ThemeManager::instance()->getColor(ThemeManager::ColorRole::Primary));
        pen.setWidth(1);
        painter.setPen(pen);
        painter.setBrush(Qt::NoBrush);
        painter.drawRoundedRect(buttonRect, 6, 6);
        
        // Handle hover/pressed states
        if (componentState() == ComponentState::Hover) {
            QColor hoverColor = ThemeManager::instance()->getColor(ThemeManager::ColorRole::Primary);
            hoverColor.setAlpha(20);
            painter.setBrush(hoverColor);
            painter.drawRoundedRect(buttonRect, 6, 6);
        } else if (componentState() == ComponentState::Pressed) {
            QColor pressedColor = ThemeManager::instance()->getColor(ThemeManager::ColorRole::Primary);
            pressedColor.setAlpha(40);
            painter.setBrush(pressedColor);
            painter.drawRoundedRect(buttonRect, 6, 6);
        }
    }
    
    BaseComponent::paintEvent(event);
}

QSize ThemedButton::sizeHint() const
{
    QSize baseSize = m_button->sizeHint();
    
    // Add some padding
    baseSize.setWidth(baseSize.width() + 16);
    baseSize.setHeight(qMax(baseSize.height(), 36));
    
    return baseSize;
}

QSize ThemedButton::minimumSizeHint() const
{
    QSize baseSize = m_button->minimumSizeHint();
    
    // Ensure minimum size
    baseSize.setWidth(qMax(baseSize.width(), 80));
    baseSize.setHeight(qMax(baseSize.height(), 32));
    
    return baseSize;
}

} // namespace ui
} // namespace rudebase3d
