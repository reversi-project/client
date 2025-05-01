#pragma once

#include <QStackedWidget>
#include <QWidget>

#include "reversi/client/context.h"

namespace reversi::client {

class Window : public QWidget {
  Q_OBJECT

 public:
  explicit Window(QApplication* app);

 private:
  ContextPtr ctx_;
};

}  // namespace reversi::client
