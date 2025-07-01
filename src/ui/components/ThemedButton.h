#pragma once

#include "BaseComponent.h"
#include <QPushButton>
#include <QIcon>

namespace rudebase3d {
namespace ui {

/**
 * @brief Custom styled button with theme integration
 * 
 * A professional button component that follows the application theme
 * and provides consistent styling and behavior.
 */
class ThemedButton : public BaseComponent
{
    Q_OBJECT

public:
    enum class ButtonStyle {
        Primary,    // Main action button
        Secondary,  // Secondary actions
        Success,    // Positive actions (save, create, etc.)
        Warning,    // Warning actions
        Danger,     // Destructive actions (delete, etc.)
        Ghost       // Minimal style
    };

    explicit ThemedButton(QWidget* parent = nullptr);
    explicit ThemedButton(const QString& text, QWidget* parent = nullptr);
    explicit ThemedButton(const QIcon& icon, const QString& text, QWidget* parent = nullptr);

    /**
     * @brief Set button text
     */
    void setText(const QString& text);
    QString text() const;

    /**
     * @brief Set button icon
     */
    void setIcon(const QIcon& icon);
    QIcon icon() const;

    /**
     * @brief Set button style
     */
    void setButtonStyle(ButtonStyle style);
    ButtonStyle buttonStyle() const;

    /**
     * @brief Set icon size
     */
    void setIconSize(const QSize& size);
    QSize iconSize() const;

protected:
    void applyTheme() override;
    QString generateBaseStyleSheet() const override;
    void onStateChanged(ComponentState oldState, ComponentState newState) override;
    void paintEvent(QPaintEvent* event) override;
    QSize sizeHint() const override;
    QSize minimumSizeHint() const override;

signals:
    void clicked();

private:
    void setupButton();
    QString getStyleSheetForState() const;
    QColor getBackgroundColorForState() const;
    QColor getTextColorForState() const;

    QPushButton* m_button;
    ButtonStyle m_buttonStyle = ButtonStyle::Primary;
    QString m_text;
    QIcon m_icon;
    QSize m_iconSize{16, 16};
};

} // namespace ui
} // namespace rudebase3d
