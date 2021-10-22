#include "ui/DemoImGui.h"
#include "utils/files.h"
#include <filesystem>
#include <fmt/format.h>
#include <magic_enum.hpp>
#include <toml++/toml.h>
#include <ui/DemoImGui.h>
#include <ui/Window.h>

toml::table loadConfig() {
  const auto configPath = pf::getExeFolder() / "config.toml";
  const auto configPathStr = configPath.string();
  fmt::print("Loading config from: '{}'\n", configPathStr);
  return toml::parse_file(configPathStr);
}

int main(int argc, char *argv[]) {
  const auto config = loadConfig();

  pf::ogl::Window mainWindow{1200, 900, "OpenGL\n"};
  fmt::print("Initializing window and OpenGL\n");
  if (auto windowInitResult = mainWindow.initialize(); windowInitResult.has_value()) {
    fmt::print(stderr, "Error during initialization: {}\n", windowInitResult.value());
    return -1;
  }
  mainWindow.setMouseButtonUserCallback([](pf::ogl::MouseEventType type, pf::ogl::MouseButton button, double x, double y) {
    fmt::print("Mouse clicked {} {}: {}x{}\n", magic_enum::enum_name(type), magic_enum::enum_name(button), x, y);
  });
  mainWindow.setKeyUserCallback([](pf::ogl::KeyEventType type, pf::Flags<pf::ogl::ModifierKey>, char ch) {
    fmt::print("Key event {}: {}\n", magic_enum::enum_name(type), ch);
  });

  auto demoUI = pf::ogl::DemoImGui{*config["imgui"].as_table(), mainWindow.getWindowHandle()};

  mainWindow.setMainLoop([&](auto) {
    demoUI.imguiInterface->render();
  });

  fmt::print("Starting main loop\n");
  mainWindow.show();
  return 0;
}
