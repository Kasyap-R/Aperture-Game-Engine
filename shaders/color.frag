#version 330 core

uniform vec4 uColor;

out vec4 fragColor; // output color of the pixel

void main() {
  fragColor = uColor;
}
