#include <ui/Window.h>
#include <fmt/format.h>
#include <spdlog/spdlog.h>
#include <toml++/toml.h>
#include <filesystem>

toml::table loadConfig() {
  const auto currentPath = std::filesystem::current_path();
  return toml::parse_file(absolute(currentPath).string());
}

int main(int argc, char *argv[]) {
  const auto config = loadConfig();

  pf::ogl::Window mainWindow{1200, 900, "OpenGL"};
  if (auto windowInitResult = mainWindow.initialize(); windowInitResult.has_value()) {
    fmt::print(stderr, "Error during initialization: {}", windowInitResult.value());
    return -1;
  }

  mainWindow.show();
  return 0;
}
