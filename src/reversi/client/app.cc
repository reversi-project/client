#include "reversi/client/app.h"

#include "reversi/client/window.h"

namespace reversi::client {

int RunApp(int argc, char** argv) {
  auto app = QApplication{argc, argv};

  auto window = Window(&app);
  window.show();

  return QApplication::exec();
}

}  // namespace reversi::client
