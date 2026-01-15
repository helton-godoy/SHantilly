#pragma once
#include <QWidget>
#include <QString>

class WidgetFactory {
public:
    static QWidget* create(const QString& type);
};
