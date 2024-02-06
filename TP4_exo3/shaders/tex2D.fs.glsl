#version 330 core

in vec2 vFragTexture;
in vec2 vFragPosition;

out vec4 FragColor;

uniform vec3 uColor;
uniform sampler2D uTexture;

void main() {
  vec4 texture = texture(uTexture, vFragTexture);
  FragColor = vec4(texture);

  //FragColor = vec4(1., 0, vFragTexture);

  //FragColor = vec4(uColor, 1.);
}