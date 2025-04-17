#pragma once

#include <QStackedWidget>
#include <QWidget>

#include "context.h"

class Window : public QWidget {
  Q_OBJECT

 public:
  explicit Window(QWidget* parent = nullptr);
  ~Window() override;

 private:
  ContextPtr ctx_;
};
