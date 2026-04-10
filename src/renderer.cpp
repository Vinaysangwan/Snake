#include "pch.h"
#include "renderer.h"
#include "utils.h"

// #############################################################################
//                           Statics
// #############################################################################
RenderState renderState = {};

// #############################################################################
//                           internal functions
// #############################################################################
Vec4 opengl_color(const Vec4 &color)
{
  return Vec4{
    color.r / 255.0f,
    color.g / 255.0f,
    color.b / 255.0f,
    color.a / 255.0f
  };
}

// #############################################################################
//                           Functions
// #############################################################################
int animate(SpriteID spriteID, float &timer, float duration, float dt)
{
  Sprite sprite = SPRITES[spriteID];
  timer += dt;
  while (timer > duration)
  {
    timer -= duration;
  }

  return (int)(sprite.frameCount * timer / duration);
}

// rendering
void render_sprite(SpriteID spriteID, Vec2 pos, float scale, int animIdx, Vec4 tintColor)
{
  if (renderState.transformCount >= MAX_TRANSFORM_COUNT)
  {
    SN_ASSERT(false, "Can't render Max Transform Count Reached: %d", renderState.transformCount);
    return;
  }
  
  const Sprite &sprite = SPRITES[spriteID];
  Vec2 size = {sprite.size.x * scale, sprite.size.y * scale};

  Transform transform = {
    .pos = pos,
    .size = size,
    .atlasOffset = sprite.atlasOffset,
    .spriteSize = sprite.size,
    .tintColor = opengl_color(tintColor),
    .animIdx = animIdx,
    .renderingOptions = RENDERING_OPTION_SPRITE
  };

  renderState.transforms[renderState.transformCount++] = transform;
}

void render_quad(Vec2 pos, Vec2 size, Vec4 color)
{
  if (renderState.transformCount >= MAX_TRANSFORM_COUNT)
  {
    SN_ASSERT(false, "Can't render Max Transform Count Reached: %d", renderState.transformCount);
    return;
  }
 
  const Sprite &sprite = SPRITES[SPRITE_QUAD];

  Transform transform = {
    .pos = pos,
    .size = {sprite.size.x * size.x, sprite.size.y * size.y},
    .atlasOffset = sprite.atlasOffset,
    .spriteSize = sprite.size,
    .tintColor = opengl_color(color),
    .animIdx = 0,
    .renderingOptions = RENDERING_OPTION_SPRITE
  };

  renderState.transforms[renderState.transformCount++] = transform;
}

void render_ui_sprite(SpriteID spriteID, Vec2 pos, float scale, int animIdx, Vec4 tintColor)
{ 
  if (renderState.uiTransformCount >= MAX_TRANSFORM_COUNT)
  {
    SN_ASSERT(false, "Can't render: Max Transform Count Reached: %d", renderState.uiTransformCount);
    return;
  }
 
  const Sprite &sprite = SPRITES[spriteID];
  Vec2 size = {sprite.size.x * scale, sprite.size.y * scale};

  Transform transform = {
    .pos = pos,
    .size = size,
    .atlasOffset = sprite.atlasOffset,
    .spriteSize = sprite.size,
    .tintColor = opengl_color(tintColor),
    .animIdx = animIdx,
    .renderingOptions = RENDERING_OPTION_SPRITE
  };

  renderState.uiTransforms[renderState.uiTransformCount++] = transform;
}

void render_ui_quad(Vec2 pos, Vec2 size, Vec4 color)
{
  if (renderState.uiTransformCount >= MAX_TRANSFORM_COUNT)
  {
    SN_ASSERT(false, "Can't render: Max Transform Count Reached: %d", renderState.uiTransformCount);
    return;
  }

  const Sprite &sprite = SPRITES[SPRITE_QUAD];

  Transform transform = {
    .pos = pos,
    .size = {sprite.size.x * size.x, sprite.size.y * size.y},
    .atlasOffset = sprite.atlasOffset,
    .spriteSize = sprite.size,
    .tintColor = opengl_color(color),
    .animIdx = 0,
    .renderingOptions = RENDERING_OPTION_SPRITE
  };

  renderState.uiTransforms[renderState.uiTransformCount++] = transform;
}

void render_ui_text(const char *text, Vec2 pos, int fontSize, Vec4 color)
{
  Vec2 initialPos = pos;

  while(char c = *(text++))
  {
    if (renderState.uiTransformCount >= MAX_TRANSFORM_COUNT)
    {
      SN_ASSERT(false, "Can't render: Max Transform Count Reached: %d", renderState.uiTransformCount);
      break;
    }
    
    if (c == '\n')
    {
      pos.x = initialPos.x;
      pos.y += renderState.fontHeight * fontSize;
      continue;
    }

    const Glyph &g = renderState.glyphs[(unsigned char)c];
    Transform transform = {
      .pos = {pos.x + g.offset.x * fontSize, pos.y + g.offset.y * fontSize},
      .size = {static_cast<float>(g.size.x * fontSize), static_cast<float>(g.size.y * fontSize)},
      .atlasOffset = g.textureCoords,
      .spriteSize = g.size,
      .tintColor = color,
      .animIdx = 0,
      .renderingOptions = RENDERING_OPTION_FONT
    };

    renderState.uiTransforms[renderState.uiTransformCount++] = transform;

    pos.x += g.advance.x * fontSize;
  }
}
