#pragma once

#include <memory>

class Menu;
class Wait;
class Play;

class Context;
using ContextPtr = std::shared_ptr<Context>;

constexpr static auto kServerUrl = "ws://localhost:8080";
