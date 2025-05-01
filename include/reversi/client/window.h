#pragma once

#include <QStackedWidget>
#include <QWidget>

#include "reversi/client/context.h"

namespace reversi::client {

class Window : public QWidget {
  Q_OBJECT

 public:
  explicit Window(QWidget* parent = nullptr);

 private:
  ContextPtr ctx_;
};

}  // namespace reversi::client
