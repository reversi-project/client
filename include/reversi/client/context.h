#pragma once

#include <reversi/contract/common.h>
#include <reversi/core/game.h>

#include <QStackedWidget>
#include <QWebSocket>
#include <memory>

#include "reversi/client/config.h"

namespace reversi::client {

using GameId = reversi::core::GameId;
using PlayerSide = reversi::core::PlayerSide;

class Context : public std::enable_shared_from_this<Context> {
 public:
  std::optional<GameId> game_id;
  std::optional<PlayerSide> player_side;

  explicit Context(QStackedWidget* stack);
  void Init(QWidget* window);
  ~Context();

  QWebSocket* GetSocket();
  void Send(const QString& message);
  void Connect();

  void ToMenuPage();
  void ToWaitPage();
  void ToPlayPage();

  bool IsMenuPage() const;
  bool IsWaitPage() const;
  bool IsPlayPage() const;

 protected:
  QWebSocket* web_socket_;
  QStackedWidget* stack_{};
  Menu* menu_;
  Wait* wait_;
  Play* play_;
  int menu_page_idx_{};
  int wait_page_idx_{};
  int play_page_idx_{};
};

}  // namespace reversi::client
