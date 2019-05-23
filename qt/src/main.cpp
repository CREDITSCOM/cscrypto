#include <QApplication>
#include "mainwidget.hpp"

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);
    cscrypto::gui::MainWidget w;
    w.show();
    return app.exec();
}
