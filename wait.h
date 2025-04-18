#pragma once

#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QWebSocket>
#include <QWidget>

#include "config.h"
#include "reversi/contract/request.h"

using Request = reversi::contract::Request;

class Wait : public QWidget {
  Q_OBJECT

 public:
  explicit Wait(ContextPtr ctx, QWidget* parent = nullptr);
  ~Wait() override;

  void Refresh();

 private slots:
  void OnSocketMessageReceived(const QString& message);
  void OnSocketError(QAbstractSocket::SocketError error);

 private:
  QLabel* game_id_label_;
  ContextPtr ctx_;

  void ShowWarning(const QString& message);
  void ToMenuPageWithWarning(const QString& message);
};
