#include "reversi/client/app.h"

namespace reversi::client {

int RunApp(int argc, char** argv) {
  auto app = QApplication{argc, argv};

  auto translator = QTranslator{};
  translator.load(":/translations/ru.qm");
  app.installTranslator(&translator);

  auto window = MainWindow{};

  window.show();
  return app.exec();
}

}  // namespace reversi::client
