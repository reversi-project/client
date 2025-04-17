#pragma once

#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QWebSocket>
#include <QWidget>

#include "config.h"
#include "context.h"
#include "reversi/contract/request.h"

using Request = reversi::contract::Request;

class Menu : public QWidget {
  Q_OBJECT

 public:
  explicit Menu(ContextPtr ctx, QWidget* parent = nullptr);
  ~Menu() override;

  void Refresh();

 private slots:
  void OnReloadClicked();
  void OnCreateGameClicked();
  void OnJoinGameClicked();
  void OnSocketConnected();
  void OnSocketMessageReceived(const QString& message);
  void OnSocketError(QAbstractSocket::SocketError error);

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
  void SendRequest(Request&& req);
  void ShowWarning(const QString& message);
  void ToMainSubpageWithWarning(const QString& message);

  int loading_subpage_idx_;
  int error_subpage_idx_;
  int main_subpage_idx_;
};
