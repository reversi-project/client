#pragma once

#include <qpushbutton.h>

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
using Game = reversi::core::Game;
using BoardIdxT = reversi::core::BoardIdxT;
using Pos = reversi::core::Pos;
using BoardItem = reversi::core::BoardItem;
constexpr static auto kBoardSize = reversi::core::kBoardSize;

enum class GameResult : uint8_t {
  kWhiteWin,
  kBlackWin,
  kTie,
};

class Play : public QWidget {
  Q_OBJECT

 public:
  explicit Play(ContextPtr ctx, QWidget* parent = nullptr);
  ~Play() override;

  void Refresh();

 private slots:
  void OnCellClicked();
  void OnSocketMessageReceived(const QString& message);
  void OnSocketError(QAbstractSocket::SocketError error);

 private:
  ContextPtr ctx_;
  Game game_;
  std::array<std::array<QPushButton*, kBoardSize>, kBoardSize> cells_;
  QLabel* current_side_label_;
  QLabel* player_side_label_;

  void MakeTurn(Pos pos);
  void MakeOpponentTurn(Pos pos);
  void Redraw();
  void RedrawButton(BoardIdxT row, BoardIdxT col);
  QString GetItemColor(BoardItem item);
  void ShowWarning(const QString& message);
  void ToMenuPageWithWarning(const QString& message);
  GameResult GetGameResult();
  QString GetResultMessage(GameResult result);
  void ToMenuOnGameEnd();
  void SendRequest(Request&& req);
};
