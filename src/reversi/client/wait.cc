#include "reversi/client/wait.h"

#include <reversi/contract/response.h>

#include <QMessageBox>
#include <utility>

#include "reversi/client/context.h"

namespace reversi::client {

using namespace reversi::contract;

Wait::Wait(ContextPtr ctx, QWidget* parent)
    : QWidget(parent), ctx_(std::move(ctx)) {
  auto* loading_label = new QLabel("Connecting...");
  loading_label->setAlignment(Qt::AlignCenter);
  loading_label->setStyleSheet("font-size: 24px; font-weight: bold;");

  game_id_label_ = new QLabel();
  game_id_label_->setAlignment(Qt::AlignCenter);
  game_id_label_->setStyleSheet("font-size: 18px;");
  game_id_label_->setObjectName("game_id_label_");

  auto* text_layout = new QVBoxLayout;
  text_layout->addWidget(loading_label);
  text_layout->addWidget(game_id_label_);
  text_layout->setSpacing(10);
  text_layout->setAlignment(Qt::AlignCenter);

  auto* container = new QWidget;
  container->setLayout(text_layout);

  auto* main_layout = new QVBoxLayout;
  main_layout->addStretch();
  main_layout->addWidget(container, 0, Qt::AlignCenter);
  main_layout->addStretch();

  setLayout(main_layout);
  connect(ctx_->GetSocket(), &QWebSocket::textMessageReceived, this,
          &Wait::OnSocketMessageReceived);
  connect(ctx_->GetSocket(), &QWebSocket::errorOccurred, this,
          &Wait::OnSocketError);
}

Wait::~Wait() = default;

void Wait::Refresh() {
  if (ctx_->game_id) {
    auto game_id = QString::number(*ctx_->game_id);
    game_id_label_->setText("Game ID is: " + game_id);
  }
}

void Wait::OnSocketMessageReceived(const QString& message) {
  if (!ctx_->IsWaitPage()) {
    return;
  }

  auto msg = message.toStdString();
  auto resp_exp = ResponseFromRaw(msg);

  if (!resp_exp) {
    ToMenuPageWithWarning(QString::fromStdString(resp_exp.error()));
    return;
  }
  auto resp = resp_exp.value();

  auto* game_start_resp = std::get_if<GameStartResponse>(&resp);
  if (game_start_resp != nullptr) {
    ctx_->ToPlayPage();
    return;
  }

  auto* error_resp = std::get_if<ErrorResponse>(&resp);
  if (error_resp != nullptr) {
    ToMenuPageWithWarning(QString::fromStdString(error_resp->message));
    return;
  }
}

void Wait::OnSocketError(QAbstractSocket::SocketError /*error*/) {
  if (!ctx_->IsWaitPage()) {
    return;
  }

  ToMenuPageWithWarning("Error: unable to connect server");
}

void Wait::ShowWarning(const QString& message) {
  QMessageBox::warning(this, "Warning", message);
}

void Wait::ToMenuPageWithWarning(const QString& message) {
  ShowWarning(message);
  ctx_->ToMenuPage();
}

}  // namespace reversi::client
