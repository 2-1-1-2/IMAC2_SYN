#version 330 core

layout (location = 0) in vec2 aVertexPosition;
layout (location = 1) in vec3 aVertexColor;
#define M_PI 3.1415926535897932384626433832795

out vec3 vFragColor;
out vec2 vFragPosition;

mat3 translate(const float tx, const float ty) {

  mat3 M = mat3(vec3(1, 0, 0), vec3(0, 1, 0), vec3(tx, ty, 1));
  return M;
}

mat3 scale(const float sx, const float sy) {
  mat3 M = mat3(vec3(sx, 0, 0), vec3(0, sy, 0), vec3(0, 0, 1));
  return M;
}

mat3 rotateZ(const float alpha) {
  float a = radians(alpha);
  mat3 M = mat3(vec3(cos(a), sin(a), 0), vec3(-sin(a), cos(a), 0), vec3(0, 0, 1));
  return M;
}

void main() {
  vFragColor = aVertexColor;
  vFragPosition = aVertexPosition;

  /* vec2 aTranslation = vec2(0., 0.5);
  vec2 aHomothetie = vec2(2, 0.5);
  gl_Position = vec4(aHomothetie*2*(aVertexPosition+aTranslation), 0., 1.); */
  mat3 T = translate(0.5, 0);

  mat3 T2 = translate(-0.5, 0);
  mat3 R = rotateZ(45);
  mat3 S = scale(2, 2);
  vec2 transformed = (R * S * vec3(aVertexPosition, 1.)).xy;
  gl_Position = vec4(transformed, 0., 1.);
  // Version sans var tmp
  //gl_Position = vec4((M * vec3(aVertexPosition, 1.)).xy, 0., 1.);
}