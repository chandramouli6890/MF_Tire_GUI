#include "MainWindow.hpp"
#include <QApplication>

int main(int argc, char *argv[]) {
  QApplication app(argc, argv);

  MainWindow window(nullptr, DEFAULT_FILENAME);
  window.show();

  return app.exec();
}
