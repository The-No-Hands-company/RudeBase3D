#include "BaseComponent.h"
#include "../core/ThemeManager.h"
#include <QEvent>
#include <QApplication>
#include <QStyle>

namespace rudebase3d {
namespace ui {

BaseComponent::BaseComponent(QWidget* parent)
    : QWidget(parent)
    , m_themeManager(ThemeManager::instance())
{
    // Connect to theme changes
    connect(m_themeManager, &ThemeManager::themeChanged,
            this, &BaseComponent::onThemeChanged);
    
    // Apply initial theme
    applyTheme();
    
    // Set up default properties
    setFocusPolicy(Qt::StrongFocus);
    setAttribute(Qt::WA_Hover, true);
}

void BaseComponent::applyTheme()
{
    if (!m_themeManager) return;
    
    // Apply base styling
    QString styleSheet = generateBaseStyleSheet();
    setStyleSheet(styleSheet);
    
    // Update palette
    QPalette palette = this->palette();
    palette.setColor(QPalette::Window, m_themeManager->getColor(ThemeManager::ColorRole::Background));
    palette.setColor(QPalette::WindowText, m_themeManager->getColor(ThemeManager::ColorRole::Foreground));
    palette.setColor(QPalette::Base, m_themeManager->getColor(ThemeManager::ColorRole::BackgroundAlternate));
    palette.setColor(QPalette::Text, m_themeManager->getColor(ThemeManager::ColorRole::Foreground));
    palette.setColor(QPalette::Button, m_themeManager->getColor(ThemeManager::ColorRole::BackgroundAlternate));
    palette.setColor(QPalette::ButtonText, m_themeManager->getColor(ThemeManager::ColorRole::Foreground));
    palette.setColor(QPalette::Highlight, m_themeManager->getColor(ThemeManager::ColorRole::Primary));
    palette.setColor(QPalette::HighlightedText, QColor(255, 255, 255));
    
    setPalette(palette);
    
    // Trigger repaint
    update();
}

QString BaseComponent::generateBaseStyleSheet() const
{
    if (!m_themeManager) return QString();
    
    return QString(R"(
        QWidget {
            background-color: %1;
            color: %2;
            border: none;
        }
        
        QWidget:focus {
            outline: 2px solid %3;
            outline-offset: 1px;
        }
    )")
    .arg(m_themeManager->getColor(ThemeManager::ColorRole::Background).name())
    .arg(m_themeManager->getColor(ThemeManager::ColorRole::Foreground).name())
    .arg(m_themeManager->getColor(ThemeManager::ColorRole::Primary).name());
}

void BaseComponent::setComponentState(ComponentState state)
{
    if (m_state == state) return;
    
    ComponentState oldState = m_state;
    m_state = state;
    
    onStateChanged(oldState, m_state);
    emit stateChanged(m_state);
    
    // Update visual appearance
    updateAppearance();
}

BaseComponent::ComponentState BaseComponent::componentState() const
{
    return m_state;
}

void BaseComponent::setDisabled(bool disabled)
{
    QWidget::setDisabled(disabled);
    setComponentState(disabled ? ComponentState::Disabled : ComponentState::Normal);
}

void BaseComponent::onThemeChanged(const QString& themeId)
{
    Q_UNUSED(themeId)
    applyTheme();
}

void BaseComponent::onStateChanged(ComponentState oldState, ComponentState newState)
{
    Q_UNUSED(oldState)
    Q_UNUSED(newState)
    // Base implementation - subclasses can override
}

void BaseComponent::updateAppearance()
{
    // Update cursor based on state
    switch (m_state) {
        case ComponentState::Disabled:
            setCursor(Qt::ForbiddenCursor);
            break;
        case ComponentState::Loading:
            setCursor(Qt::WaitCursor);
            break;
        default:
            setCursor(Qt::ArrowCursor);
            break;
    }
    
    // Update tooltip based on state
    updateTooltip();
    
    // Repaint
    update();
}

void BaseComponent::updateTooltip()
{
    QString tooltip = m_baseTooltip;
    
    switch (m_state) {
        case ComponentState::Disabled:
            if (!m_disabledTooltip.isEmpty()) {
                tooltip = m_disabledTooltip;
            } else if (!tooltip.isEmpty()) {
                tooltip += " (Disabled)";
            }
            break;
        case ComponentState::Loading:
            tooltip = "Loading...";
            break;
        case ComponentState::Error:
            if (!m_errorTooltip.isEmpty()) {
                tooltip = m_errorTooltip;
            } else if (!tooltip.isEmpty()) {
                tooltip += " (Error)";
            }
            break;
        default:
            break;
    }
    
    setToolTip(tooltip);
}

void BaseComponent::setBaseTooltip(const QString& tooltip)
{
    m_baseTooltip = tooltip;
    updateTooltip();
}

void BaseComponent::setDisabledTooltip(const QString& tooltip)
{
    m_disabledTooltip = tooltip;
    updateTooltip();
}

void BaseComponent::setErrorTooltip(const QString& tooltip)
{
    m_errorTooltip = tooltip;
    updateTooltip();
}

bool BaseComponent::event(QEvent* event)
{
    switch (event->type()) {
        case QEvent::Enter:
            if (m_state == ComponentState::Normal) {
                setComponentState(ComponentState::Hover);
            }
            break;
            
        case QEvent::Leave:
            if (m_state == ComponentState::Hover) {
                setComponentState(ComponentState::Normal);
            }
            break;
            
        case QEvent::FocusIn:
            if (m_state == ComponentState::Normal || m_state == ComponentState::Hover) {
                setComponentState(ComponentState::Focus);
            }
            break;
            
        case QEvent::FocusOut:
            if (m_state == ComponentState::Focus) {
                setComponentState(ComponentState::Normal);
            }
            break;
            
        case QEvent::MouseButtonPress:
            if (m_state != ComponentState::Disabled && m_state != ComponentState::Loading) {
                setComponentState(ComponentState::Pressed);
            }
            break;
            
        case QEvent::MouseButtonRelease:
            if (m_state == ComponentState::Pressed) {
                // Check if mouse is still over the widget
                if (rect().contains(mapFromGlobal(QCursor::pos()))) {
                    setComponentState(ComponentState::Hover);
                } else {
                    setComponentState(ComponentState::Normal);
                }
            }
            break;
            
        default:
            break;
    }
    
    return QWidget::event(event);
}

QColor BaseComponent::getThemeColor(ThemeManager::ColorRole role) const
{
    return m_themeManager ? m_themeManager->getColor(role) : QColor();
}

void BaseComponent::showLoadingState()
{
    setComponentState(ComponentState::Loading);
}

void BaseComponent::hideLoadingState()
{
    setComponentState(ComponentState::Normal);
}

void BaseComponent::showErrorState(const QString& errorMessage)
{
    setErrorTooltip(errorMessage);
    setComponentState(ComponentState::Error);
}

void BaseComponent::clearErrorState()
{
    setErrorTooltip(QString());
    setComponentState(ComponentState::Normal);
}

} // namespace ui
} // namespace rudebase3d
