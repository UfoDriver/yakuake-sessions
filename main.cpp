#include <QtWidgets/QApplication>
#include "sessionlist.h"


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    SessionList sl;
    sl.show();

    return a.exec();
}
