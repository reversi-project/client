#pragma once

#include <memory>

namespace reversi::client {

class Menu;

class Wait;
class Play;

class Context;
using ContextPtr = std::shared_ptr<Context>;

constexpr static auto kServerUrl = "ws://localhost:8080";

}  // namespace reversi::client
