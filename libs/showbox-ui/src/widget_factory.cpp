#include "widget_factory.h"
#include <QPushButton>

QWidget* WidgetFactory::create(const QString& type) {
    if (type == "button") {
        return new QPushButton("Factory Button");
    }
    return nullptr;
}
