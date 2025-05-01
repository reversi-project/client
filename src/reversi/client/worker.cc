#include "reversi/client/worker.h"

#include <qwebsocket.h>
#include <reversi/contract/request.h>

#include <QObject>
#include <QThread>

#include "reversi/client/config.h"

namespace reversi::client {

Worker::Worker(QObject* parent) : QObject(parent) {}

void Worker::Init() {
  socket_ = new QWebSocket;
  connect(socket_, &QWebSocket::connected, this, &Worker::OnConnected);
  connect(socket_, &QWebSocket::textMessageReceived, this,
          &Worker::OnTextMessageReceived);
  connect(socket_, &QWebSocket::errorOccurred, this, &Worker::OnErrorOccured);
}

Worker::~Worker() { delete socket_; }

void Worker::MakeConnectWebSocket() { socket_->open(QUrl(kServerUrl)); }

void Worker::MakeSendRequest(Request req) {
  socket_->sendTextMessage(QString::fromStdString(contract::RequestToRaw(req)));
}

void Worker::OnConnected() { emit Connected(); }

void Worker::OnTextMessageReceived(const QString& message) {
  emit MessageReceived(message);
}

void Worker::OnErrorOccured(QAbstractSocket::SocketError error) {
  emit ErrorOccured(error);
}

}  // namespace reversi::client
