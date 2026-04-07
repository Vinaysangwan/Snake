
// inputs
layout (location = 0) in vec2 inTexCoords;
layout (location = 1) in vec3 inTintColor;

// output
layout (location = 0) out vec4 outColor;

// uniforms
uniform sampler2D uTextureSampler;

void main()
{
  vec4 textureColor = texelFetch(uTextureSampler, ivec2(inTexCoords), 0);
  if (textureColor.a < 0.1)
    discard;
  
  outColor = textureColor * vec4(inTintColor, 1.0);
}
