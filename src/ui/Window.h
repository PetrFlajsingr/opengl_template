//
// Created by xflajs00 on 22.10.2021.
//

#ifndef OPENGL_TEMPLATE_SRC_UI_WINDOW_H
#define OPENGL_TEMPLATE_SRC_UI_WINDOW_H



#include <GLFW/glfw3.h>
#include <concepts>
#include <functional>
#include <optional>
#include <pf_common/enums.h>
#include <string>

namespace pf::ogl {

enum class MouseButton { Left = 1 << 0,
                         Right = 1 << 1,
                         Middle = 1 << 2,
                         None = 1 << 3 };

enum class MouseEventType {
  Down = 1 << 0,
  Up = 1 << 1
};

enum class KeyEventType { Up = 1 << 0,
                          Pressed = 1 << 1,
                          Repeat = 1 << 2 };

enum class ModifierKey {
  Shift = 1 << 0,
  Control = 1 << 1,
  Alt = 1 << 2,
  Super = 1 << 3,
  CapsLock = 1 << 4,
  NumLock = 1 << 5
};

class Window {
 public:
  Window(std::size_t width, std::size_t height, std::string title);
  ~Window();

  [[nodiscard]] std::optional<std::string> initialize();
  void show();
  void close();

  [[nodiscard]] std::size_t getWidth() const;
  [[nodiscard]] std::size_t getHeight() const;
  [[nodiscard]] const std::string &getTitle() const;
  [[nodiscard]] GLFWwindow *getWindowHandle() const;
  void setResizeUserCallback(std::invocable<std::size_t, std::size_t> auto &&resizeUserCallback) {
    Window::resizeUserCallback = resizeUserCallback;
  }
  void setMouseButtonUserCallback(std::invocable<MouseEventType, MouseButton, double, double> auto &&mouseButtonUserCallback) {
    Window::mouseButtonUserCallback = mouseButtonUserCallback;
  }
  void setMouseMoveUserCallback(std::invocable<double, double> auto &&mouseMoveUserCallback) {
    Window::mouseMoveUserCallback = mouseMoveUserCallback;
  }
  void setMouseWheelUserCallback(std::invocable<double, double> auto &&mouseWheelUserCallback) {
    Window::mouseWheelUserCallback = mouseWheelUserCallback;
  }
  void setKeyUserCallback(std::invocable<KeyEventType, Flags<ModifierKey>, char> auto &&keyUserCallback) {
    Window::keyUserCallback = keyUserCallback;
  }
  void setMainLoop(std::invocable<double> auto &&mainLoop) {
    Window::mainLoop = mainLoop;
  }

 private:
  static void windowSizeCallback(GLFWwindow *window, int width, int height);
  std::function<void(std::size_t, std::size_t)> resizeUserCallback = [](auto, auto) {};

  static void mouseButtonCallback(GLFWwindow *window, int button, int action, int mods);
  std::function<void(MouseEventType, MouseButton, double, double)> mouseButtonUserCallback = [](auto, auto, auto, auto) {};

  static void mousePositionCallback(GLFWwindow *window, double xpos, double ypos);
  std::function<void(double, double)> mouseMoveUserCallback = [](auto, auto) {};

  static void mouseWheelCallback(GLFWwindow *window, double xpos, double ypos);
  std::function<void(double, double)> mouseWheelUserCallback = [](auto, auto) {};

  static void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods);
  std::function<void(KeyEventType, Flags<ModifierKey>, char)> keyUserCallback = [](auto, auto, auto) {};

  std::function<void(double)> mainLoop = [](auto){};

  std::size_t width;
  std::size_t height;
  std::string title;

  bool closed = false;

  GLFWwindow *windowHandle = nullptr;
};

namespace details {
std::optional<MouseButton> glfwButtonToEvents(int button);
std::optional<KeyEventType> glfwKeyEventToEvents(int key_event);

Flags<pf::ogl::ModifierKey> modToFlags(int mods);
}// namespace details

}// namespace pf::ogl

#endif//OPENGL_TEMPLATE_SRC_UI_WINDOW_H
