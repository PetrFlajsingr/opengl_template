#include "renderers/DemoRenderer.h"
#include "ui/DemoImGui.h"
#include "utils/files.h"
#include <filesystem>
#include <fmt/format.h>
#include <magic_enum.hpp>
#include <pf_glfw/GLFW.h>
#include <pf_mainloop/MainLoop.h>
#include <toml++/toml.h>
#include <ui/DemoImGui.h>
#include <spdlog/spdlog.h>
#include <argparse/argparse.hpp>

argparse::ArgumentParser createArgParser() {
  auto parser = argparse::ArgumentParser{"OpenGL template"};
  parser.add_description("OpenGL template project using pf_ libraries");
  return parser;
}

/**
 * Load toml config located next to the exe - config.toml
 * @return
 */
toml::table loadConfig() {
  const auto configPath = pf::getExeFolder() / "config.toml";
  const auto configPathStr = configPath.string();
  spdlog::info("Loading config from: '{}'", configPathStr);
  return toml::parse_file(configPathStr);
}

/**
 * Serialize UI, save it to the config and save the config next to the exe into config.toml
 */
void saveConfig(toml::table config, pf::ui::ig::ImGuiInterface &imguiInterface, const std::shared_ptr<pf::glfw::Window> &window) {
  const auto configPath = pf::getExeFolder() / "config.toml";
  const auto configPathStr = configPath.string();
  spdlog::info("Saving config file to: '{}'", configPathStr);
  imguiInterface.updateConfig();
  config.insert_or_assign("imgui", imguiInterface.getConfig());
  const auto &[width, height] = window->getSize();
  config["window"].as_table()->insert_or_assign("width", width);
  config["window"].as_table()->insert_or_assign("height", height);
  auto ofstream = std::ofstream(configPathStr);
  ofstream << config;
}

int main(int argc, char *argv[]) {
  auto parser = createArgParser();
  try {
    parser.parse_args(argc, argv);
  } catch(const std::runtime_error &e) {
    spdlog::error("{}", e.what());
    fmt::print("{}", parser.help().str());
    return 1;
  }
  const auto config = loadConfig();
  const auto resourcesFolder = std::filesystem::path{config["files"]["resources_path"].value<std::string>().value()};

  spdlog::info("Initializing window and OpenGL");
  pf::glfw::GLFW glfw{};
  auto window = glfw.createWindow({.width = static_cast<std::size_t>(config["window"]["width"].value_or(1200)),
                                   .height = static_cast<std::size_t>(config["window"]["height"].value_or(900)),
                                   .title = "OpenGL",
                                   .majorOpenGLVersion = 4,
                                   .minorOpenGLVersion = 6});
  window->setCurrent();
  if (!gladLoadGLLoader((GLADloadproc) glfw.getLoaderFnc())) {
    spdlog::error("Error while initializing GLAD");
    return -1;
  }

  auto demoUI = pf::ogl::DemoImGui{*config["imgui"].as_table(), window->getHandle()};

  pf::ogl::DemoRenderer renderer{resourcesFolder / "shaders"};
  if (const auto initResult = renderer.init(); initResult.has_value()) {
    spdlog::error("Error during initialization: {}", initResult.value());
    return -1;
  }

  window->setMouseButtonCallback([&](pf::glfw::MouseButton btn, pf::glfw::ButtonState state, pf::Flags<pf::glfw::ModifierKey> mods) {
    if (state == pf::glfw::ButtonState::Down) { return; }
    std::string txt = fmt::format("Clicked {} button", magic_enum::enum_name(btn));
    if (mods.is(pf::glfw::ModifierKey::Shift)) {
      txt += " with shift";
    }
    if (mods.is(pf::glfw::ModifierKey::Control)) {
      txt += " with Control";
    }
    demoUI.imguiInterface->getNotificationManager()
        .createNotification(pf::ui::ig::NotificationType::Info,
                            pf::ui::ig::uniqueId(),
                            txt)
        .createChild<pf::ui::ig::Text>(pf::ui::ig::uniqueId(), "Demo notification");
  },
                                 true);

  pf::MainLoop::Get()->setOnMainLoop([&](auto) {
    if (window->shouldClose()) {
      pf::MainLoop::Get()->stop();
    }
    renderer.render();
    demoUI.imguiInterface->render();
    window->swapBuffers();
    glfw.pollEvents();
  });

  spdlog::info("Starting main loop");
  pf::MainLoop::Get()->run();
  spdlog::info("Main loop ended");

  saveConfig(config, *demoUI.imguiInterface, window);
  return 0;
}
