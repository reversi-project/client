#include "reversi/client/window.h"

#include <qlogging.h>

#include <QApplication>
#include <QBoxLayout>

#include "reversi/client/context.h"
#include "reversi/client/menu.h"
#include "reversi/client/play.h"

namespace reversi::client {

Window::Window(QApplication* app) : QWidget(nullptr) {
  auto* worker = new Worker;
  auto* thread = new QThread;
  worker->moveToThread(thread);
  connect(thread, &QThread::started, worker, &Worker::Init);
  thread->start();

  auto* stack = new QStackedWidget;
  ctx_ = std::make_shared<Context>(stack);
  ctx_->Init(this, worker);

  connect(ctx_->GetMenu(), &Menu::ConnectWebSocket, worker,
          &Worker::MakeConnectWebSocket);
  connect(ctx_->GetMenu(), &Menu::SendRequest, worker,
          &Worker::MakeSendRequest);
  connect(ctx_->GetPlay(), &Play::SendRequest, worker,
          &Worker::MakeSendRequest);

  connect(app, &QApplication::aboutToQuit, thread, &QThread::quit);
  connect(thread, &QThread::finished, worker, &QObject::deleteLater);
  connect(thread, &QThread::finished, thread, &QObject::deleteLater);

  emit ctx_->GetMenu()->ConnectWebSocket();

  auto* layout = new QVBoxLayout;
  layout->addWidget(stack);
  layout->setContentsMargins(0, 0, 0, 0);
  setLayout(layout);
  setStyleSheet("background-color: #d1c4e9;");
}

}  // namespace reversi::client
