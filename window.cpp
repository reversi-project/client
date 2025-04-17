#include "window.h"

#include <QBoxLayout>

#include "context.h"

Window::Window(QWidget* parent) : QWidget(parent) {
  auto* stack = new QStackedWidget;
  ctx_ = std::make_shared<Context>(stack);
  ctx_->Init(this);

  auto* layout = new QVBoxLayout;
  layout->addWidget(stack);
  layout->setContentsMargins(0, 0, 0, 0);
  setLayout(layout);
  setStyleSheet("background-color: #d1c4e9;");
}

Window::~Window() = default;
