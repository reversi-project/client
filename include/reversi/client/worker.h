#pragma once

#include <reversi/contract/request.h>

#include <QMutex>
#include <QObject>
#include <QThread>
#include <QWaitCondition>
#include <QWebSocket>

namespace reversi::client {

using Request = reversi::contract::Request;

class Worker : public QObject {
  Q_OBJECT

 public:
  explicit Worker(QObject* parent = nullptr);
  void Init();
  ~Worker() override;

 public slots:
  void MakeConnectWebSocket();
  void MakeSendRequest(Request req);

 signals:
  void Connected();
  void MessageReceived(const QString& message);
  void ErrorOccured(QAbstractSocket::SocketError error);

 private slots:
  void OnConnected();
  void OnTextMessageReceived(const QString& message);
  void OnErrorOccured(QAbstractSocket::SocketError error);

 private:
  QWebSocket* socket_;
};

}  // namespace reversi::client
