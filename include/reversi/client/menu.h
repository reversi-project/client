#pragma once

#include <reversi/contract/request.h>

#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QStackedWidget>
#include <QVBoxLayout>
#include <QWebSocket>
#include <QWidget>

#include "reversi/client/config.h"

namespace reversi::client {

using Request = reversi::contract::Request;

class Menu : public QWidget {
  Q_OBJECT

 public:
  explicit Menu(ContextPtr ctx, QWidget* parent = nullptr);

  void Refresh();

 signals:
  void SendRequest(Request req);
  void ConnectWebSocket();

 private slots:
  void OnReloadClicked();
  void OnCreateGameClicked();
  void OnJoinGameClicked();
  void OnConnected();
  void OnMessageReceived(const QString& message);
  void OnErrorOccured(QAbstractSocket::SocketError error);

 private:
  QLabel* error_label_;
  QPushButton* create_game_btn_;
  QPushButton* reload_btn_;
  QLineEdit* join_game_edit_;
  QPushButton* join_game_btn_;
  QWidget* main_subpage_;
  QWidget* loading_subpage_;
  QWidget* error_subpage_;
  QStackedWidget* substack_;
  ContextPtr ctx_;

  void InitLoadingSubpage();
  void InitErrorSubpage();
  void InitMainSubpage();
  void ShowWarning(const QString& message);
  void ToMainSubpageWithWarning(const QString& message);

  int loading_subpage_idx_;
  int error_subpage_idx_;
  int main_subpage_idx_;
};

}  // namespace reversi::client
