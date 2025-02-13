#include <QApplication>
#include <QStyleFactory>
#include <QSettings>

#include "gfc.h"


int main(int argc, char* argv[]) {

  QApplication app(argc, argv);

  app.setApplicationName("Game flow controller");

  GFC gfc;
  gfc.show();
  return app.exec();

}