#version 430 core

// output
layout (location = 0) out vec2 outTexCoords;

// uniforms

// structs
struct Transform
{
  vec2 pos;
  vec2 size;
  ivec2 atlasOffset;
  ivec2 spriteSize;
};

// buffers
layout (std430, binding = 0) buffer TransformSSBO
{
  Transform transforms[];
};

void main()
{
  Transform transform = transforms[gl_InstanceID];
  
  // vertices
  vec2 vertices[6] = {
    transform.pos,                                                                      // left top
    vec2(transform.pos.x, transform.pos.y + transform.size.y),                          // left bottom
    vec2(transform.pos.x + transform.size.x, transform.pos.y),                          // right top

    vec2(transform.pos.x + transform.size.x, transform.pos.y),                          // right top
    vec2(transform.pos.x, transform.pos.y + transform.size.y),                          // left bottom
    vec2(transform.pos.x + transform.size.x, transform.pos.y + transform.size.y),       // right bottom
  };

  vec2 textureCoords[6] = {
    vec2(transform.atlasOffset.x, transform.atlasOffset.y + transform.spriteSize.y),
    transform.atlasOffset,
    vec2(transform.atlasOffset.x + transform.spriteSize.x, transform.atlasOffset.y + transform.spriteSize.y),

    vec2(transform.atlasOffset.x + transform.spriteSize.x, transform.atlasOffset.y + transform.spriteSize.y),
    transform.atlasOffset,
    vec2(transform.atlasOffset.x + transform.spriteSize.x, transform.atlasOffset.y),
  };

  gl_Position = vec4(vertices[gl_VertexID], 0.0, 1.0);

  outTexCoords = textureCoords[gl_VertexID];
}
