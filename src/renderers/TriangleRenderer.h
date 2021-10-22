//
// Created by xflajs00 on 22.10.2021.
//

#ifndef OPENGL_TEMPLATE_SRC_TRIANGLERENDERER_H
#define OPENGL_TEMPLATE_SRC_TRIANGLERENDERER_H

#include "Renderer.h"
#include <glad/glad.h>

namespace pf::ogl {

namespace details {
const float triangleVertices[] = {
    -0.5f, -0.5f, 0.0f,
    0.5f, -0.5f, 0.0f,
    0.0f, 0.5f, 0.0f};
}

class TriangleRenderer : public Renderer {
 public:
  TriangleRenderer() = default;

  void init() override;
  void render() override;

 private:
  GLuint VBO;
};

}// namespace pf::ogl

#endif//OPENGL_TEMPLATE_SRC_TRIANGLERENDERER_H
