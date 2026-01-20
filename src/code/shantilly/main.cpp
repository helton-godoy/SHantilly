#include "SHantilly.h"

#include <QApplication>

int main(int argc, char* argv[]) {
    QGuiApplication::setHighDpiScaleFactorRoundingPolicy(
        Qt::HighDpiScaleFactorRoundingPolicy::PassThrough);
    QApplication app(argc, argv);

    SHantilly dialog("SHantilly Demo", "Demonstração da interface SHantilly");
    dialog.show();

    return app.exec();
}
