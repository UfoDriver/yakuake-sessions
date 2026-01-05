#include "sessionlist.h"
#include <QIcon>
#include <QtWidgets/QApplication>

int main(int argc, char *argv[]) {
  QApplication a(argc, argv);
  QIcon::setFallbackSearchPaths(QIcon::fallbackSearchPaths()
                                << ":custom-icons");
  SessionList sl;
  sl.show();

  return a.exec();
}
