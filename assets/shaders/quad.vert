#version 430 core

// inputs

// output
layout (location = 0) out vec2 outTexCoords;

// uniforms

void main()
{
  // vertices
  vec2 vertices[6] = {
    vec2(-0.5f, 0.5f),        // left top
    vec2(-0.5f, -0.5f),        // left bottom
    vec2(0.5f, 0.5f),        // right top

    vec2(0.5f, 0.5f),        // right top
    vec2(-0.5f, -0.5f),        // left bottom
    vec2(0.5f, -0.5f)         // right bottom
  };

  vec2 textureCoords[6] = {
    vec2(0, 0),
    vec2(0, 16),
    vec2(16, 0),

    vec2(16, 0),
    vec2(0, 16),
    vec2(16, 16)
  };

  gl_Position = vec4(vertices[gl_VertexID], 0.0, 1.0);

  outTexCoords = textureCoords[gl_VertexID];
}
