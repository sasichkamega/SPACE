#pragma once
#include <memory>
#include <flecs.h>
#include <SFML/Window/WindowBase.hpp>

namespace engine {
  struct WindowBackendSfml {
    WindowBackendSfml(flecs::world&);
  };
}

namespace engine::window_backend_sfml {
  struct MainWindowSFML {
    std::shared_ptr<sf::WindowBase> window;
  };
}