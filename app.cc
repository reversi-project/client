#include "app.h"

#include "window.h"

namespace reversi::client {

int RunApp(int argc, char **argv) {
  auto app = QApplication{argc, argv};
  auto window = Window{};
  window.show();
  return QApplication::exec();
}

}  // namespace reversi::client
