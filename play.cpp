#include "play.h"

#include <qlogging.h>
#include <qobject.h>

#include <QMessageBox>
#include <utility>

#include "context.h"
#include "reversi/contract/common.h"
#include "reversi/contract/request.h"
#include "reversi/contract/response.h"
#include "reversi/core/board.h"

using namespace reversi::contract;
using TurnResult = reversi::core::TurnResult;

Play::Play(ContextPtr ctx, QWidget* parent)
    : QWidget(parent), ctx_(std::move(ctx)) {
  auto* layout = new QGridLayout;
  layout->setSpacing(0);
  layout->setContentsMargins(0, 0, 0, 0);

  for (BoardIdxT row = 0; row < kBoardSize; ++row) {
    for (BoardIdxT col = 0; col < kBoardSize; ++col) {
      auto* btn = new QPushButton;
      btn->setFixedSize(50, 50);
      btn->setStyleSheet("background-color: green; font-size: 24px;");
      layout->addWidget(btn, row, col);
      cells_[row][col] = btn;

      connect(btn, &QPushButton::clicked, this, &Play::OnCellClicked);
    }
  }

  auto* container = new QWidget;
  container->setLayout(layout);

  current_side_label_ = new QLabel();
  current_side_label_->setAlignment(Qt::AlignCenter);
  current_side_label_->setStyleSheet("font-size: 16px; font-weight: bold;");
  current_side_label_->setObjectName("current_side_label_");

  player_side_label_ = new QLabel();
  player_side_label_->setAlignment(Qt::AlignCenter);
  player_side_label_->setStyleSheet("font-size: 16px; font-weight: bold;");
  player_side_label_->setObjectName("player_side_label_");

  auto* outer_layout = new QVBoxLayout;
  outer_layout->addStretch();
  outer_layout->addWidget(player_side_label_);
  outer_layout->addSpacing(10);
  outer_layout->addWidget(container, 0, Qt::AlignCenter);
  outer_layout->addSpacing(10);
  outer_layout->addWidget(current_side_label_);
  outer_layout->addStretch();

  setLayout(outer_layout);
  Redraw();

  connect(ctx_->GetSocket(), &QWebSocket::textMessageReceived, this,
          &Play::OnSocketMessageReceived);
  connect(ctx_->GetSocket(), &QWebSocket::errorOccurred, this,
          &Play::OnSocketError);
}

Play::~Play() = default;

void Play::Refresh() {
  game_ = Game{};
  Redraw();
}

void Play::OnCellClicked() {
  if (!ctx_->player_side || game_.GetCurrentSide() != ctx_->player_side) {
    return;
  }
  if (game_.IsFinished()) {
    return;
  }
  const auto& board = game_.GetBoard();

  for (BoardIdxT row = 0; row < kBoardSize; ++row) {
    for (BoardIdxT col = 0; col < kBoardSize; ++col) {
      if (sender() == cells_[row][col]) {
        MakeTurn({row, col});
        return;
      }
    }
  }
}

void Play::OnSocketMessageReceived(const QString& message) {
  if (!ctx_->IsPlayPage()) {
    return;
  }

  auto msg = message.toStdString();
  auto resp_exp = ResponseFromRaw(msg);

  if (!resp_exp) {
    ToMenuPageWithWarning(QString::fromStdString(resp_exp.error()));
    return;
  }
  auto resp = resp_exp.value();

  auto* opponent_resp = std::get_if<OpponentTurnResponse>(&resp);
  if (opponent_resp != nullptr) {
    MakeOpponentTurn(opponent_resp->pos);
    return;
  }

  auto* quit_resp = std::get_if<QuitResponse>(&resp);
  if (quit_resp != nullptr) {
    ToMenuPageWithWarning("Opponent left the game");
    return;
  }

  auto* error_resp = std::get_if<ErrorResponse>(&resp);
  if (error_resp != nullptr) {
    ToMenuPageWithWarning(QString::fromStdString(error_resp->message));
    return;
  }
}

void Play::OnSocketError(QAbstractSocket::SocketError /*error*/) {
  if (!ctx_->IsPlayPage()) {
    return;
  }

  ToMenuPageWithWarning("Error: unable to connect server");
}

void Play::MakeTurn(Pos pos) {
  auto turn_result = game_.MakeTurn(pos);
  if (turn_result == TurnResult::kFailure) {
    ShowWarning("Invalid turn");
    return;
  }
  Redraw();
  auto req = TurnRequest{.game_id = *ctx_->game_id, .pos = pos};
  SendRequest(req);
  if (game_.IsFinished()) {
    ToMenuOnGameEnd();
  }
}

void Play::MakeOpponentTurn(Pos pos) {
  auto turn_result = game_.MakeTurn(pos);
  if (turn_result == TurnResult::kFailure) {
    ToMenuPageWithWarning("Something going wrong");
    return;
  }
  Redraw();
  if (game_.IsFinished()) {
    ToMenuOnGameEnd();
  }
}

void Play::Redraw() {
  const auto& board = game_.GetBoard();

  for (BoardIdxT row = 0; row < kBoardSize; ++row) {
    for (BoardIdxT col = 0; col < kBoardSize; ++col) {
      RedrawButton(row, col);
    }
  }

  if (game_.GetCurrentSide() == PlayerSide::kWhite) {
    current_side_label_->setText("Current turn: white");
  } else {
    current_side_label_->setText("Current turn: black");
  }

  if (ctx_->player_side) {
    if (*ctx_->player_side == PlayerSide::kWhite) {
      player_side_label_->setText("You play white");
    } else {
      player_side_label_->setText("You play black");
    }
  }
}

void Play::RedrawButton(BoardIdxT row, BoardIdxT col) {
  const auto& btn = cells_[row][col];
  auto item = game_.GetBoard().Get({.row = row, .col = col});

  auto color = GetItemColor(item);
  btn->setStyleSheet(QString("background-color: %1;").arg(color));
}

QString Play::GetItemColor(BoardItem item) {
  switch (item) {
    case BoardItem::kEmpty:
      return "green";
    case BoardItem::kWhite:
      return "white";
    case BoardItem::kBlack:
      return "black";
  }
}

void Play::ShowWarning(const QString& message) {
  QMessageBox::warning(this, "Warning", message);
}

void Play::ToMenuPageWithWarning(const QString& message) {
  ShowWarning(message);
  ctx_->ToMenuPage();
}

GameResult Play::GetGameResult() {
  auto white_stones = game_.CountPlayerStones(PlayerSide::kWhite);
  auto black_stones = game_.CountPlayerStones(PlayerSide::kBlack);

  if (white_stones > black_stones) {
    return GameResult::kWhiteWin;
  }
  if (white_stones < black_stones) {
    return GameResult::kBlackWin;
  }
  return GameResult::kTie;
}

QString Play::GetResultMessage(GameResult result) {
  if (result == GameResult::kWhiteWin) {
    if (ctx_->player_side == PlayerSide::kWhite) {
      return "Result: you win";
    }
    return "Result: you lose";
  }
  if (result == GameResult::kBlackWin) {
    if (ctx_->player_side == PlayerSide::kBlack) {
      return "Result: you win";
    }
    return "Result: you lose";
  }
  return "Result: tie";
}

void Play::ToMenuOnGameEnd() {
  auto game_result = GetGameResult();
  auto message = GetResultMessage(game_result);

  QMessageBox::information(this, "Game end", message);
  ctx_->ToMenuPage();
}

void Play::SendRequest(Request&& req) {
  auto raw = RequestToRaw(req);
  ctx_->Send(QString::fromStdString(raw));
}
