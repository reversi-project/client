#include "reversi/client/context.h"

#include <QUrl>
#include <QWebSocket>

#include "reversi/client/config.h"
#include "reversi/client/menu.h"
#include "reversi/client/play.h"
#include "reversi/client/wait.h"
#include "reversi/client/worker.h"

namespace reversi::client {

Context::Context(QStackedWidget* stack)
    : game_id(std::nullopt), stack_(stack) {}

void Context::Init(QWidget* parent, Worker* worker) {
  worker_ = worker;
  auto self = shared_from_this();

  menu_ = new Menu(self, parent);
  menu_page_idx_ = stack_->addWidget(menu_);

  wait_ = new Wait(self, parent);
  wait_page_idx_ = stack_->addWidget(wait_);

  play_ = new Play(self, parent);
  play_page_idx_ = stack_->addWidget(play_);
}

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

Menu* Context::GetMenu() const { return menu_; }

Wait* Context::GetWait() const { return wait_; }

Play* Context::GetPlay() const { return play_; }

Worker* Context::GetWorker() const { return worker_; }

}  // namespace reversi::client
