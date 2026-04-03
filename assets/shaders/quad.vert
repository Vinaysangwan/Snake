#version 430 core

// inputs

// output

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

  gl_Position = vec4(vertices[gl_VertexID], 0.0, 1.0);
}
