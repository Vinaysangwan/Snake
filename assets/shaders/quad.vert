
// buffers
layout (std430, binding = 0) buffer TransformSSBO
{
  Transform transforms[];
};

// output
layout (location = 0) out vec2 outTexCoords;
layout (location = 1) out vec3 outTintColor;

// uniforms
uniform mat4 uProjectionMatrix;
uniform mat4 uViewMatrix;

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

  gl_Position = uProjectionMatrix * uViewMatrix * vec4(vertices[gl_VertexID], 0.0, 1.0);

  // Texture Coords
  transform.atlasOffset.x += int(transform.tintColor.a) * transform.spriteSize.x;

  vec2 textureCoords[6] = {
    transform.atlasOffset,
    vec2(transform.atlasOffset.x, transform.atlasOffset.y + transform.spriteSize.y),
    vec2(transform.atlasOffset.x + transform.spriteSize.x, transform.atlasOffset.y),

    vec2(transform.atlasOffset.x + transform.spriteSize.x, transform.atlasOffset.y),
    vec2(transform.atlasOffset.x, transform.atlasOffset.y + transform.spriteSize.y),
    vec2(transform.atlasOffset.x + transform.spriteSize.x, transform.atlasOffset.y + transform.spriteSize.y),
  };

  outTexCoords = textureCoords[gl_VertexID];
  outTintColor = transform.tintColor.rgb;
}
