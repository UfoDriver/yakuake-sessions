#include <QtWidgets/QApplication>
#include <QIcon>
#include "sessionlist.h"


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QIcon::setFallbackSearchPaths(QIcon::fallbackSearchPaths() << ":custom-icons");
    SessionList sl;
    sl.show();

    return a.exec();
}
