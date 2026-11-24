#include "mainwindow.h"

#include <QApplication>
#include <QCoreApplication>
#include <QIcon>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QCoreApplication::setApplicationName(QStringLiteral("MSN-WhatsApp-XP"));
    QCoreApplication::setOrganizationName(QStringLiteral("ChristyGaming18"));
    QCoreApplication::setApplicationVersion(QStringLiteral(MSN_WA_XP_VERSION));

    MainWindow window;
    window.show();

    return app.exec();
}
