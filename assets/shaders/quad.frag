
// inputs
layout (location = 0) in vec2 inTexCoords;
layout (location = 1) in vec4 inTintColor;
layout (location = 2) in flat int outRenderingOptions;

// output
layout (location = 0) out vec4 outColor;

// uniforms
layout (binding = 0) uniform sampler2D uTextureSampler;
layout (binding = 1) uniform sampler2D uFontSampler;

void main()
{
  if (bool(outRenderingOptions & RENDERING_OPTION_SPRITE))
  {
    vec4 textureColor = texelFetch(uTextureSampler, ivec2(inTexCoords), 0);
    
    outColor = textureColor * inTintColor;
  }
  else if(bool(outRenderingOptions & RENDERING_OPTION_FONT))
  {
    vec4 fontColor = texelFetch(uFontSampler, ivec2(inTexCoords), 0);
    
    outColor = fontColor.r * inTintColor;
  }
}
