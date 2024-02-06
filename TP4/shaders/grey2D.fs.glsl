#version 330 core

in vec3 vFragColor;
in vec2 vFragPosition;

out vec4 fFragColor;

void main() {
  float aColorBWtmp = (vFragColor.r + vFragColor.g + vFragColor.b) / 3.;
  vec3 aColorBW = vFragColor;//vec3(aColorBWtmp, aColorBWtmp, aColorBWtmp);
  float alpha = 2, beta = 25;
  vec2 P = vFragPosition;
  float test = mod(floor(10.0 * P.x) + floor(10.0 * P.y), 2.0);
  vec2 v = vec2(test, test);
  float d = distance(v, vec2(0, 0));
  aColorBW *= alpha * exp(-beta * pow(d, 2));
  fFragColor = vec4(aColorBW, 1.);
}