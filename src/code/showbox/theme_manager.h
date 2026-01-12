#ifndef THEME_MANAGER_H
#define THEME_MANAGER_H

#include <QObject>
#include <QGuiApplication>
#include <QStyleHints>

class ThemeManager : public QObject
{
    Q_OBJECT
public:
    explicit ThemeManager(QObject *parent = nullptr);
    
    enum ThemeMode {
        Light,
        Dark,
        Unknown
    };
    
    ThemeMode currentTheme() const;

signals:
    void themeChanged(ThemeMode newTheme);

private slots:
    void onColorSchemeChanged(Qt::ColorScheme scheme);

private:
    ThemeMode m_currentTheme;
};

#endif // THEME_MANAGER_H
