#ifndef WIDGET_CONFIGS_H
#define WIDGET_CONFIGS_H

#include <QString>
#include <QStringList>
#include <QList>
#include <QMap>

namespace Showbox {
namespace Models {

struct BaseConfig {
    QString name;
    
    virtual ~BaseConfig() = default;
    
    virtual bool isValid() const {
        return !name.isEmpty();
    }
};

struct WindowConfig : public BaseConfig {
    QString title;
    int width = 800;
    int height = 600;

    // Window might not strictly need a 'name' if it's the main window,
    // but usually in showbox it's implicitly 'window'.
    // Overriding isValid to be permissive for Window title.
    bool isValid() const override {
        return width > 0 && height > 0;
    }
};

struct ButtonConfig : public BaseConfig {
    QString text = "Button";
    bool checkable = false;
    bool checked = false;
};

struct LabelConfig : public BaseConfig {
    QString text = "Label";
    bool wordWrap = false;
};

struct LineEditConfig : public BaseConfig {
    QString text;
    QString placeholder;
    bool passwordMode = false;
};

struct ComboBoxConfig : public BaseConfig {
    QStringList items;
    int currentIndex = -1;
};

struct ListConfig : public BaseConfig {
    QStringList items;
    bool multipleSelection = false;
};

struct TableConfig : public BaseConfig {
    QStringList headers;
    QList<QStringList> rows;
};

struct ProgressBarConfig : public BaseConfig {
    int value = 0;
    int minimum = 0;
    int maximum = 100;
    QString format = "%p%";
};

struct ChartConfig : public BaseConfig {
    enum Type { Line, Bar, Pie };
    Type type = Line;
    QString title;
    QMap<QString, double> data;
};

struct LayoutConfig : public BaseConfig {
    enum Type { VBox, HBox, Grid };
    Type type = VBox;
    int spacing = 5;
    int margin = 5;
};

} // namespace Models
} // namespace Showbox

#endif // WIDGET_CONFIGS_H
