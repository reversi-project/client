#include <qobject.h>

#include <QtTest/QTest>

#include "context.h"
#include "menu.h"
#include "play.h"
#include "utils.h"
#include "wait.h"

class MenuTest : public QObject {
  Q_OBJECT

 private slots:
  void initTestCase() {
    widget_ = new QWidget();
    stack_ = new QStackedWidget(widget_);
    ctx_ = std::make_shared<Context>(stack_);
    ctx_->Init(widget_);
  }

  void cleanupTestCase() { delete widget_; }

  void testCreateGameButtonExists() {
    auto* menu = stack_->widget(0);
    auto* btn = menu->findChild<QPushButton*>("create_game_btn_");
    QVERIFY(btn != nullptr);
    QCOMPARE(btn->text(), "Create game");
  }

  void testJoinGameExists() {
    auto* menu = stack_->widget(0);
    auto* joinEdit = menu->findChild<QLineEdit*>();
    auto* joinBtn = menu->findChild<QPushButton*>("join_game_btn_");
    QVERIFY(joinEdit != nullptr);
    QVERIFY(joinBtn != nullptr);
    QCOMPARE(joinBtn->text(), "Join game");
  }

  void testWarningShownOnInvalidJoin() {
    auto* menu = stack_->widget(0);
    auto* edit = menu->findChild<QLineEdit*>();
    edit->setText("invalid_id");

    int closes = 0;
    ignoreMessageBox(closes);
    QTest::mouseClick(menu->findChild<QPushButton*>("join_game_btn_"),
                      Qt::LeftButton);

    QVERIFY(stack_->currentIndex() == 0);
    QVERIFY(closes == 1);
  }

 private:
  ContextPtr ctx_;
  QWidget* widget_;
  QStackedWidget* stack_;
};

class WaitTest : public QObject {
  Q_OBJECT

 private slots:
  void initTestCase() {
    widget_ = new QWidget();
    stack_ = new QStackedWidget(widget_);
    ctx_ = std::make_shared<Context>(stack_);
    ctx_->Init(widget_);
    ctx_->ToWaitPage();
  }

  void cleanupTestCase() { delete widget_; }

  void testGameIdExists() {
    ctx_->game_id = 42;
    auto* wait = qobject_cast<Wait*>(stack_->widget(1));
    wait->Refresh();

    auto* label = wait->findChild<QLabel*>("game_id_label_");
    QVERIFY(label->text() == "Game ID is: 42");
  }

  void testGameStartResponseTriggerPlayPage() {
    ignoreMessageBox();
    emit ctx_->GetSocket()->textMessageReceived("start");
    QVERIFY(stack_->currentIndex() == 2);
  }

  void testErrorResponseTriggerMenuPage() {
    ignoreMessageBox();
    emit ctx_->GetSocket()->textMessageReceived("error error_message");
    QVERIFY(stack_->currentIndex() == 0);
  }

  void testInvalidMessageTriggerMenuPage() {
    ignoreMessageBox();
    emit ctx_->GetSocket()->textMessageReceived("invalid");
    QVERIFY(stack_->currentIndex() == 0);
  }

  void testSocketErrorTriggerMenuPage() {
    ignoreMessageBox();
    emit ctx_->GetSocket()->errorOccurred(
        QAbstractSocket::ConnectionRefusedError);
    QVERIFY(stack_->currentIndex() == 0);
  }

 private:
  ContextPtr ctx_;
  QWidget* widget_;
  QStackedWidget* stack_;
};

class PlayTest : public QObject {
  Q_OBJECT

 private slots:
  void initTestCase() {
    widget_ = new QWidget();
    stack_ = new QStackedWidget(widget_);
    ctx_ = std::make_shared<Context>(stack_);
    ctx_->Init(widget_);
    ctx_->game_id = 42;
    ctx_->player_side = PlayerSide::kWhite;
    ctx_->ToPlayPage();
  }

  void cleanupTestCase() { delete widget_; }

  void testCurrentSideLabelExists() {
    auto* play = stack_->widget(2);
    auto* current_side_label = play->findChild<QLabel*>("current_side_label_");
    QVERIFY(current_side_label != nullptr);
    QCOMPARE(current_side_label->text(), "Current turn: white");
  }

  void testPlayerSideLabelExists() {
    auto* play = stack_->widget(2);
    auto* player_side_label = play->findChild<QLabel*>("player_side_label_");
    QVERIFY(player_side_label != nullptr);
    QCOMPARE(player_side_label->text(), "You play white");
  }

  void testCurrentSideLabelChanges() {
    auto* play = stack_->widget(2);
    auto buttons = play->findChildren<QPushButton*>();

    ignoreMessageBox();
    QTest::mouseClick(buttons[20], Qt::LeftButton);

    auto* current_side_label = play->findChild<QLabel*>("current_side_label_");
    QVERIFY(current_side_label != nullptr);
    QCOMPARE(current_side_label->text(), "Current turn: black");
  }

 private:
  ContextPtr ctx_;
  QWidget* widget_;
  QStackedWidget* stack_;
};

int main(int argc, char** argv) {
  QApplication app(argc, argv);

  int status = 0;
  auto runTest = [&status, argc, argv](QObject* obj) {
    status |= QTest::qExec(obj, argc, argv);
  };

  runTest(new MenuTest);
  runTest(new WaitTest);
  runTest(new PlayTest);

  return status;
}

#include "test_main.moc"
