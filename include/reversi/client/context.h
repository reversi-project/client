#pragma once

#include <qwebsocket.h>
#include <reversi/contract/common.h>
#include <reversi/core/game.h>

#include <QStackedWidget>
#include <QWebSocket>
#include <memory>

#include "reversi/client/config.h"
#include "reversi/client/menu.h"
#include "reversi/client/worker.h"

namespace reversi::client {

using GameId = reversi::core::GameId;
using PlayerSide = reversi::core::PlayerSide;

class Context : public std::enable_shared_from_this<Context> {
 public:
  std::optional<GameId> game_id;
  std::optional<PlayerSide> player_side;

  explicit Context(QStackedWidget* stack);
  void Init(QWidget* parent, Worker* worker);

  void ToMenuPage();
  void ToWaitPage();
  void ToPlayPage();

  bool IsMenuPage() const;
  bool IsWaitPage() const;
  bool IsPlayPage() const;

  Menu* GetMenu() const;
  Wait* GetWait() const;
  Play* GetPlay() const;
  Worker* GetWorker() const;

 private:
  Worker* worker_;
  QStackedWidget* stack_{};
  Menu* menu_;
  Wait* wait_;
  Play* play_;
  int menu_page_idx_{};
  int wait_page_idx_{};
  int play_page_idx_{};
};

}  // namespace reversi::client
