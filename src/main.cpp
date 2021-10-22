#include "renderers/DemoRenderer.h"
#include "ui/DemoImGui.h"
#include "utils/files.h"
#include <filesystem>
#include <fmt/format.h>
#include <magic_enum.hpp>
#include <toml++/toml.h>
#include <ui/DemoImGui.h>
#include <ui/Window.h>

/**
 * Load toml config located next to the exe - config.toml
 * @return
 */
toml::table loadConfig() {
  const auto configPath = pf::getExeFolder() / "config.toml";
  const auto configPathStr = configPath.string();
  fmt::print("Loading config from: '{}'\n", configPathStr);
  return toml::parse_file(configPathStr);
}

/**
 * Serialize UI, save it to the config and save the config next to the exe into config.toml
 */
void saveConfig(toml::table config, pf::ui::ig::ImGuiInterface &imguiInterface) {
  const auto configPath = pf::getExeFolder() / "config.toml";
  const auto configPathStr = configPath.string();
  fmt::print("Saving config file to: '{}'\n", configPathStr);
  imguiInterface.updateConfig();
  config.insert_or_assign("imgui", imguiInterface.getConfig());
  auto ofstream = std::ofstream(configPathStr);
  ofstream << config;
}

int main(int argc, char *argv[]) {
  const auto config = loadConfig();
  const auto resourcesFolder = std::filesystem::path{config["files"]["resources_path"].value<std::string>().value()};

  pf::ogl::Window mainWindow{1200, 900, "OpenGL\n"};
  fmt::print("Initializing window and OpenGL\n");
  if (auto windowInitResult = mainWindow.initialize(); windowInitResult.has_value()) {
    fmt::print(stderr, "Error during initialization: {}\n", windowInitResult.value());
    return -1;
  }
  auto demoUI = pf::ogl::DemoImGui{*config["imgui"].as_table(), mainWindow.getWindowHandle()};

  mainWindow.setInputIgnorePredicate([&] { return demoUI.imguiInterface->isWindowHovered() || demoUI.imguiInterface->isKeyboardCaptured(); });


  pf::ogl::DemoRenderer renderer{resourcesFolder / "shaders"};
  if (const auto initResult = renderer.init(); initResult.has_value()) {
    fmt::print(stderr, "Error during initialization: {}\n", initResult.value());
    return -1;
  }

  mainWindow.setMainLoop([&](auto) {
    renderer.render();
    demoUI.imguiInterface->render();
  });

  fmt::print("Starting main loop\n");
  mainWindow.show();
  fmt::print("Main loop ended\n");

  saveConfig(config, *demoUI.imguiInterface);
  return 0;
}
