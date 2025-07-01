#pragma once

#include <QWidget>
#include <QString>
#include <QEvent>
#include <QColor>
#include <QPalette>
#include "../core/ThemeManager.h"

namespace rudebase3d {
namespace ui {

/**
 * @brief Base class for all UI components
 * 
 * Provides common functionality like theming, state management, and
 * consistent behavior across all UI components.
 */
class BaseComponent : public QWidget
{
    Q_OBJECT

public:
    /**
     * @brief Component visual states
     */
    enum class ComponentState {
        Normal,
        Hover,
        Focus,
        Pressed,
        Disabled,
        Loading,
        Error
    };

    explicit BaseComponent(QWidget* parent = nullptr);

    /**
     * @brief Set component state
     */
    void setComponentState(ComponentState state);

    /**
     * @brief Get current component state
     */
    ComponentState componentState() const;

    /**
     * @brief Override setDisabled to update component state
     */
    void setDisabled(bool disabled);

    /**
     * @brief Convenience methods for common states
     */
    void showLoadingState();
    void hideLoadingState();
    void showErrorState(const QString& errorMessage = QString());
    void clearErrorState();

    /**
     * @brief Set tooltips for different states
     */
    void setBaseTooltip(const QString& tooltip);
    void setDisabledTooltip(const QString& tooltip);
    void setErrorTooltip(const QString& tooltip);

protected:
    /**
     * @brief Apply current theme to the component
     */
    virtual void applyTheme();

    /**
     * @brief Generate base stylesheet for the component
     */
    virtual QString generateBaseStyleSheet() const;

    /**
     * @brief Called when component state changes
     */
    virtual void onStateChanged(ComponentState oldState, ComponentState newState);

    /**
     * @brief Update visual appearance based on current state
     */
    virtual void updateAppearance();

    /**
     * @brief Get a color from the current theme
     */
    QColor getThemeColor(ThemeManager::ColorRole role) const;

    /**
     * @brief Handle events for state management
     */
    bool event(QEvent* event) override;

signals:
    /**
     * @brief Emitted when component state changes
     */
    void stateChanged(ComponentState state);

private slots:
    void onThemeChanged(const QString& themeId);

private:
    void updateTooltip();

    ThemeManager* m_themeManager = nullptr;
    ComponentState m_state = ComponentState::Normal;
    QString m_baseTooltip;
    QString m_disabledTooltip;
    QString m_errorTooltip;
};

} // namespace ui
} // namespace rudebase3d

// Make ComponentState available for Qt's meta-object system
Q_DECLARE_METATYPE(rudebase3d::ui::BaseComponent::ComponentState)
