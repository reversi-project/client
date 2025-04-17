#include "context.h"

#include <qurl.h>
#include <qwebsocket.h>

#include <QWebSocket>

#include "config.h"
#include "menu.h"
#include "play.h"
#include "wait.h"

Context::Context(QStackedWidget* stack)
    : game_id(std::nullopt), stack_(stack), web_socket_(new QWebSocket()) {}

void Context::Init(QWidget* window) {
  auto self = shared_from_this();

  menu_ = new Menu(self, window);
  menu_page_idx_ = stack_->addWidget(menu_);

  wait_ = new Wait(self, window);
  wait_page_idx_ = stack_->addWidget(wait_);

  play_ = new Play(self, window);
  play_page_idx_ = stack_->addWidget(play_);
}

Context::~Context() { delete web_socket_; }

QWebSocket* Context::GetSocket() { return web_socket_; }

void Context::Send(const QString& message) {
  web_socket_->sendTextMessage(message);
}

void Context::Connect() { web_socket_->open(QUrl(kServerUrl)); }

void Context::ToMenuPage() {
  menu_->Refresh();
  stack_->setCurrentIndex(menu_page_idx_);
}

void Context::ToWaitPage() {
  wait_->Refresh();
  stack_->setCurrentIndex(wait_page_idx_);
}

void Context::ToPlayPage() {
  play_->Refresh();
  stack_->setCurrentIndex(play_page_idx_);
}

bool Context::IsMenuPage() const {
  return stack_->currentIndex() == menu_page_idx_;
}

bool Context::IsWaitPage() const {
  return stack_->currentIndex() == wait_page_idx_;
}

bool Context::IsPlayPage() const {
  return stack_->currentIndex() == play_page_idx_;
}
