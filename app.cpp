#include "app.h"

#include "window.h"

int RunApp(int argc, char **argv) {
  auto app = QApplication{argc, argv};

  auto window = Window{};

  window.show();
  return QApplication::exec();
}
