#version 330 core
in vec2 TexCoord;

uniform sampler2D uTexture;

out vec4 fragColor; // output color of the pixel


void main() {
  fragColor = texture(uTexture, TexCoord);
}
