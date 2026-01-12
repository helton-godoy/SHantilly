#include "theme_manager.h"
#include "logger.h"
#include <QDebug>

ThemeManager::ThemeManager(QObject *parent)
    : QObject(parent)
{
    // Check for environment variable override
    QByteArray envTheme = qgetenv("SHOWBOX_THEME").toLower();
    if (envTheme == "dark") {
        m_currentTheme = Dark;
    } else if (envTheme == "light") {
        m_currentTheme = Light;
    } else {
        // Initialize current theme from system
        onColorSchemeChanged(QGuiApplication::styleHints()->colorScheme());
    }
    
    // Connect to system changes
    connect(QGuiApplication::styleHints(), &QStyleHints::colorSchemeChanged,
            this, &ThemeManager::onColorSchemeChanged);
}

ThemeManager::ThemeMode ThemeManager::currentTheme() const
{
    return m_currentTheme;
}

void ThemeManager::onColorSchemeChanged(Qt::ColorScheme scheme)
{
    ThemeMode newTheme;
    switch (scheme) {
        case Qt::ColorScheme::Dark:
            newTheme = Dark;
            break;
        case Qt::ColorScheme::Light:
            newTheme = Light;
            break;
        default:
            newTheme = Unknown;
            break;
    }
    
    if (newTheme != m_currentTheme) {
        m_currentTheme = newTheme;
        qCDebug(guiLog) << "Theme changed to:" << (m_currentTheme == Dark ? "Dark" : (m_currentTheme == Light ? "Light" : "Unknown"));
        emit themeChanged(m_currentTheme);
    }
}
