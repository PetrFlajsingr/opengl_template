//
// Created by xflajs00 on 22.10.2021.
//

#include "Window.h"
#include <glad/glad.h>
#include <utility>

pf::ogl::Window::Window(std::size_t width, std::size_t height, std::string title) : width(width), height(height), title(std::move(title)) {
}

pf::ogl::Window::~Window() {
  if (windowHandle != nullptr) {
    glfwTerminate();
  }
}

std::optional<std::string> pf::ogl::Window::initialize() {
  if (!glfwInit()) {
    return "Error while initializing glfw";
  }
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  windowHandle = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
  glfwSetWindowUserPointer(windowHandle, this);

  if (!windowHandle) {
    glfwTerminate();
    return "Error while creating window handle";
  }

  glfwMakeContextCurrent(windowHandle);
  glfwSetWindowSizeCallback(windowHandle, windowSizeCallback);
  glfwSetMouseButtonCallback(windowHandle, mouseButtonCallback);
  glfwSetCursorPosCallback(windowHandle, mousePositionCallback);
  glfwSetScrollCallback(windowHandle, mouseWheelCallback);
  glfwSetKeyCallback(windowHandle, keyCallback);
  glfwSetInputMode(windowHandle, GLFW_LOCK_KEY_MODS, GLFW_TRUE);

  if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
    return "Error while initializing GLAD";
  }

  glClearColor(0.f, 0.f, 0.f, 1.f);
  glViewport(0, 0, width, height);

  return std::nullopt;
}

void pf::ogl::Window::close() {
  closed = true;
}

void pf::ogl::Window::show() {
  const auto startTime = glfwGetTime();
  while (!glfwWindowShouldClose(windowHandle)) {
    if (closed) {
      break;
    }
    const auto time = glfwGetTime();
    const auto timeElapsed = time - startTime;
    mainLoop(timeElapsed);
    glfwSwapBuffers(windowHandle);
    glfwPollEvents();
  }
}

size_t pf::ogl::Window::getWidth() const {
  return width;
}

size_t pf::ogl::Window::getHeight() const {
  return height;
}

const std::string &pf::ogl::Window::getTitle() const {
  return title;
}

void pf::ogl::Window::windowSizeCallback(GLFWwindow *window, int width, int height) {
  auto self = reinterpret_cast<Window *>(glfwGetWindowUserPointer(window));
  self->width = width;
  self->height = height;
  self->resizeUserCallback(static_cast<std::size_t>(width), static_cast<std::size_t>(height));
}

void pf::ogl::Window::mouseButtonCallback(GLFWwindow *window, int button, int action, int mods) {
  auto self = reinterpret_cast<Window *>(glfwGetWindowUserPointer(window));
  if (self->inputIgnorePredicate()) {
    return;
  }
  const auto mouseButton = details::glfwButtonToEvents(button);
  if (!mouseButton.has_value()) { return; }
  const auto eventType = action == GLFW_PRESS ? MouseEventType::Down : MouseEventType::Up;
  auto cursorPosition = std::pair<double, double>{};
  glfwGetCursorPos(window, &cursorPosition.first, &cursorPosition.second);
  self->mouseButtonUserCallback(eventType, mouseButton.value(), cursorPosition.first, cursorPosition.second);
}

void pf::ogl::Window::mousePositionCallback(GLFWwindow *window, double xpos, double ypos) {
  auto self = reinterpret_cast<Window *>(glfwGetWindowUserPointer(window));
  if (self->inputIgnorePredicate()) {
    return;
  }
  self->mouseMoveUserCallback(xpos, ypos);
}

void pf::ogl::Window::mouseWheelCallback(GLFWwindow *window, double xpos, double ypos) {
  auto self = reinterpret_cast<Window *>(glfwGetWindowUserPointer(window));
  if (self->inputIgnorePredicate()) {
    return;
  }
  self->mouseWheelUserCallback(xpos, ypos);
}

void pf::ogl::Window::keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods) {
  auto self = reinterpret_cast<Window *>(glfwGetWindowUserPointer(window));
  if (self->inputIgnorePredicate()) {
    return;
  }
  const auto eventType = details::glfwKeyEventToEvents(action);
  if (!eventType.has_value()) { return; }
  const auto keyChar = static_cast<char>(key);
  self->keyUserCallback(eventType.value(), details::modToFlags(mods), keyChar);
}

GLFWwindow *pf::ogl::Window::getWindowHandle() const {
  return windowHandle;
}

std::optional<pf::ogl::MouseButton> pf::ogl::details::glfwButtonToEvents(int button) {
  switch (button) {
    case GLFW_MOUSE_BUTTON_LEFT: return MouseButton::Left;
    case GLFW_MOUSE_BUTTON_MIDDLE: return MouseButton::Middle;
    case GLFW_MOUSE_BUTTON_RIGHT: return MouseButton::Right;
    default: return std::nullopt;
  }
}

pf::Flags<pf::ogl::ModifierKey> pf::ogl::details::modToFlags(int mods) {
  auto result = Flags<ModifierKey>{};
  if (mods & GLFW_MOD_SHIFT) { result |= ModifierKey::Shift; }
  if (mods & GLFW_MOD_CONTROL) { result |= ModifierKey::Control; }
  if (mods & GLFW_MOD_ALT) { result |= ModifierKey::Alt; }
  if (mods & GLFW_MOD_SUPER) { result |= ModifierKey::Super; }
  if (mods & GLFW_MOD_CAPS_LOCK) { result |= ModifierKey::CapsLock; }
  if (mods & GLFW_MOD_NUM_LOCK) { result |= ModifierKey::NumLock; }
  return result;
}
std::optional<pf::ogl::KeyEventType> pf::ogl::details::glfwKeyEventToEvents(int key_event) {
  switch (key_event) {
    case GLFW_PRESS: return KeyEventType::Pressed;
    case GLFW_REPEAT: return KeyEventType::Repeat;
    case GLFW_RELEASE: return KeyEventType::Up;
    default: return std::nullopt;
  }
}
