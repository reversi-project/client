#include "reversi/client/menu.h"

#include <reversi/contract/common.h>
#include <reversi/contract/request.h>
#include <reversi/contract/response.h>

#include <QMessageBox>
#include <utility>

#include "reversi/client/context.h"

namespace reversi::client {

using namespace reversi::contract;

Menu::Menu(ContextPtr ctx, QWidget* parent)
    : QWidget(parent), ctx_(std::move(ctx)) {
  InitLoadingSubpage();
  InitErrorSubpage();
  InitMainSubpage();

  substack_ = new QStackedWidget;
  loading_subpage_idx_ = substack_->addWidget(loading_subpage_);
  main_subpage_idx_ = substack_->addWidget(main_subpage_);
  error_subpage_idx_ = substack_->addWidget(error_subpage_);

  auto* layout = new QVBoxLayout;
  layout->addWidget(substack_);
  layout->setContentsMargins(0, 0, 0, 0);
  setLayout(layout);

  connect(reload_btn_, &QPushButton::clicked, this, &Menu::OnReloadClicked);
  connect(create_game_btn_, &QPushButton::clicked, this,
          &Menu::OnCreateGameClicked);
  connect(join_game_btn_, &QPushButton::clicked, this,
          &Menu::OnJoinGameClicked);
  connect(ctx_->GetWorker(), &Worker::Connected, this, &Menu::OnConnected);
  connect(ctx_->GetWorker(), &Worker::MessageReceived, this,
          &Menu::OnMessageReceived);
  connect(ctx_->GetWorker(), &Worker::ErrorOccured, this,
          &Menu::OnErrorOccured);
}

void Menu::Refresh() {
  emit ConnectWebSocket();
  join_game_edit_->clear();
}

void Menu::OnReloadClicked() { emit ConnectWebSocket(); }

void Menu::OnCreateGameClicked() {
  substack_->setCurrentIndex(loading_subpage_idx_);

  emit SendRequest(CreateRequest{});
}

void Menu::OnJoinGameClicked() {
  auto is_ok = false;
  auto game_id = join_game_edit_->text().toInt(&is_ok);
  if (!is_ok) {
    ShowWarning("Game ID is not valid");
    return;
  }

  substack_->setCurrentIndex(loading_subpage_idx_);
  emit SendRequest(ConnectRequest{static_cast<GameId>(game_id)});
}

void Menu::OnConnected() { substack_->setCurrentIndex(main_subpage_idx_); }

void Menu::OnMessageReceived(const QString& message) {
  if (!ctx_->IsMenuPage()) {
    return;
  }

  auto res = ResponseFromRaw(message.toStdString());
  if (!res) {
    ToMainSubpageWithWarning(QString::fromStdString(res.error()));
    return;
  }
  auto resp = res.value();

  auto* game_created_resp = std::get_if<GameCreatedResponse>(&resp);
  if (game_created_resp != nullptr) {
    ctx_->game_id = game_created_resp->game_id;
    ctx_->player_side = PlayerSide::kWhite;
    ctx_->ToWaitPage();
    return;
  }

  auto* connected_resp = std::get_if<ConnectedResponse>(&resp);
  if (connected_resp != nullptr) {
    ctx_->game_id = connected_resp->game_id;
    ctx_->player_side = PlayerSide::kBlack;
    ctx_->ToPlayPage();
    return;
  }

  auto* error_resp = std::get_if<ErrorResponse>(&resp);
  if (error_resp != nullptr) {
    ToMainSubpageWithWarning(QString::fromStdString(error_resp->message));
    return;
  }
}

void Menu::OnErrorOccured(QAbstractSocket::SocketError /*error*/) {
  if (!ctx_->IsMenuPage()) {
    return;
  }

  error_label_->setText("Error: unable to connect server");
  substack_->setCurrentIndex(error_subpage_idx_);
}

void Menu::InitLoadingSubpage() {
  auto* loading_label = new QLabel("Connecting...");
  loading_label->setAlignment(Qt::AlignCenter);

  auto* loading_layout = new QVBoxLayout;
  loading_layout->addWidget(loading_label);

  loading_subpage_ = new QWidget;
  loading_subpage_->setLayout(loading_layout);
}

void Menu::InitErrorSubpage() {
  error_label_ = new QLabel("");
  error_label_->setAlignment(Qt::AlignCenter);

  reload_btn_ = new QPushButton("Try reconnect");

  auto* error_layout = new QVBoxLayout;
  error_layout->addWidget(error_label_);
  error_layout->addWidget(reload_btn_);

  error_subpage_ = new QWidget;
  error_subpage_->setLayout(error_layout);
}

void Menu::InitMainSubpage() {
  auto* title = new QLabel("Reversi");
  title->setAlignment(Qt::AlignCenter);
  title->setStyleSheet("font-size: 32px; font-weight: bold;");

  create_game_btn_ = new QPushButton("Create game");
  create_game_btn_->setObjectName("create_game_btn_");
  create_game_btn_->setFixedHeight(40);
  create_game_btn_->setStyleSheet("font-size: 16px;");

  join_game_edit_ = new QLineEdit;
  join_game_edit_->setPlaceholderText("Game ID");
  join_game_edit_->setFixedWidth(120);
  join_game_edit_->setFixedHeight(40);
  join_game_edit_->setStyleSheet("font-size: 16px;");

  join_game_btn_ = new QPushButton("Join game");
  join_game_btn_->setObjectName("join_game_btn_");
  join_game_btn_->setFixedWidth(100);
  join_game_btn_->setFixedHeight(40);
  join_game_btn_->setStyleSheet("font-size: 16px;");

  auto* join_layout = new QHBoxLayout;
  join_layout->addWidget(join_game_edit_);
  join_layout->addWidget(join_game_btn_);
  join_layout->setSpacing(10);
  join_layout->setAlignment(Qt::AlignCenter);

  auto* join_container = new QWidget;
  join_container->setLayout(join_layout);

  int container_width = join_game_edit_->width() + join_game_btn_->width() +
                        join_layout->spacing();
  create_game_btn_->setFixedWidth(container_width);

  auto* button_block_layout = new QVBoxLayout;
  button_block_layout->addWidget(create_game_btn_, 0, Qt::AlignCenter);
  button_block_layout->addSpacing(15);
  button_block_layout->addWidget(join_container, 0, Qt::AlignCenter);

  auto* button_block_widget = new QWidget;
  button_block_widget->setLayout(button_block_layout);

  auto* main_layout = new QVBoxLayout;
  main_layout->addWidget(title, 0, Qt::AlignTop | Qt::AlignHCenter);
  main_layout->addStretch(1);
  main_layout->addWidget(button_block_widget, 0, Qt::AlignCenter);
  main_layout->addStretch(2);

  main_subpage_ = new QWidget;
  main_subpage_->setLayout(main_layout);
}

void Menu::ShowWarning(const QString& message) {
  QMessageBox::warning(this, "Warning", message);
}

void Menu::ToMainSubpageWithWarning(const QString& message) {
  substack_->setCurrentIndex(main_subpage_idx_);
  ShowWarning(message);
}

}  // namespace reversi::client
