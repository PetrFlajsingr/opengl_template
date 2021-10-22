//
// Created by xflajs00 on 22.10.2021.
//

#include "TriangleRenderer.h"

void pf::ogl::TriangleRenderer::init() {
  glGenBuffers(1, &VBO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(details::triangleVertices), details::triangleVertices, GL_STATIC_DRAW;
}

void pf::ogl::TriangleRenderer::render() {

}
