#include "pch.h"
#include "game.h"
#include "renderer.h"
#include "inputs.h"
#include "config.h"
#include <fstream>

// #############################################################################
//                           Constants
// #############################################################################
constexpr const float GRID_SIZE = 20;
constexpr const int MAP_ROWS = 9;
constexpr const int MAP_COLS = 16;
constexpr const int MAP_SIZE = MAP_ROWS * MAP_COLS;

constexpr const float LEFT_WALL = GRID_SIZE;
constexpr const float RIGHT_WALL = WORLD_WIDTH - GRID_SIZE;
constexpr const float TOP_WALL = 2.0f * GRID_SIZE;
constexpr const float BOTTOM_WALL = WORLD_HEIGHT - GRID_SIZE;

constexpr const int MAP[MAP_SIZE] = {
   0,  1,  2,  1,  2,  1,  2,  1,  2,  1,  2,  1,  2,  1,  2,  3, 
   4,  5,  5,  5,  6,  5,  5,  5,  5,  5,  5,  5,  6,  5,  5,  7, 
   8,  9,  9,  9,  9,  9,  9,  9,  9,  9,  9,  9,  9,  9,  9, 11, 
   4,  9,  9,  9,  9,  9,  9, 10,  9,  9,  9,  9, 10,  9,  9,  7, 
   8,  9,  9, 10,  9,  9,  9,  9,  9,  9,  9,  9,  9,  9,  9, 11, 
   4,  9,  9,  9,  9,  9,  9,  9,  9,  9,  9,  9,  9,  9,  9,  7, 
   8,  9,  9,  9,  9,  9,  9,  9,  9, 10,  9,  9,  9,  9,  9, 11, 
   4,  9,  9,  9,  9,  9,  9,  9,  9,  9,  9,  9,  9,  9,  9,  7, 
  12, 13, 14, 13, 14, 13, 14, 13, 14, 13, 14, 13, 14, 13, 14, 15, 
};

// #############################################################################
//                           Internal functions 
// #############################################################################
void spawn_bat()
{
  Vec2 &pos = gameState.bat.pos;
  int gridX = random_get_int(2, MAP_COLS - 2);
  int gridY = random_get_int(2, MAP_ROWS - 2);

  pos.x = gridX * GRID_SIZE;
  pos.y = gridY * GRID_SIZE;
}

Rect get_entity_collider(const Entity &entity)
{
  const Sprite &sprite = SPRITES[entity.spriteID];

  return Rect{
    .pos = Vec2{entity.pos.x + sprite.colliderOffset.x, entity.pos.y + sprite.colliderOffset.y},
    .size = ivec2_f(sprite.colliderSize)
  };
} 

// #############################################################################
//                           Functions
// #############################################################################
void game_init()
{
  // init debug
  gameState.debug = false;
  
  // Init game Camera
  {
    Camera &gameCamera = renderState.gameCamera;
    gameCamera.pos = {WORLD_WIDTH / 2, WORLD_HEIGHT / 2};
    gameCamera.size = {WORLD_WIDTH, WORLD_HEIGHT};
  }
  
  // Init Player Data
  {
    Entity &player = gameState.player;
    player.spriteID = SPRITE_SLIME;
    const Sprite &sprite = SPRITES[player.spriteID];
    player.pos = {WORLD_WIDTH / 2 - sprite.size.x / 2.0f, WORLD_HEIGHT / 2 - sprite.size.y / 2.0f};
  }

  // Init Bat Data
  {
    Entity &bat = gameState.bat;
    bat.spriteID = SPRITE_BAT;
    bat.pos = {WORLD_WIDTH / 2, WORLD_HEIGHT / 2};
  }

  // Init Music
  {
    gameState.gameMusic = music_load("assets/audio/theme.mp3");
    music_play(gameState.gameMusic);
  }

  // Init sounds
  {
    gameState.bounceSound = sound_load("assets/audio/collect.wav");
  }
}

void game_update(float dt)
{
  static const float SPEED = 1.0f;

  if (key_pressed(GLFW_KEY_Z)) gameState.debug = !gameState.debug;
  
  // Update Player
  Entity &player = gameState.player;
  {
    if (key_down(GLFW_KEY_W)) player.pos.y -= 2 * SPEED;
    if (key_down(GLFW_KEY_S)) player.pos.y += 2 * SPEED;
    if (key_down(GLFW_KEY_A)) player.pos.x -= 2 * SPEED;
    if (key_down(GLFW_KEY_D)) player.pos.x += 2 * SPEED;
    if (key_pressed(GLFW_KEY_Q))
    {
      SN_INFO("Music: is %s", music_is_playing(gameState.gameMusic) ? "Playing" : "Not Playing");
    }

    // handle wall Collision
    bool bounceSoundPlay = false;
    static bool prevBounceSoundPlay = bounceSoundPlay;
    if (player.pos.x < LEFT_WALL)
    {
      player.pos.x = LEFT_WALL;
      bounceSoundPlay = true;
    }
    else if(player.pos.x > RIGHT_WALL - 20)
    {
      player.pos.x = RIGHT_WALL - 20;
      bounceSoundPlay = true;
    }

    if (player.pos.y < TOP_WALL)
    {
      player.pos.y = TOP_WALL;
      bounceSoundPlay = true;
    }
    else if(player.pos.y > BOTTOM_WALL - 20)
    {
      player.pos.y = BOTTOM_WALL - 20;
      bounceSoundPlay = true;
    }

    if (bounceSoundPlay && !prevBounceSoundPlay)
    {
      sound_play(gameState.bounceSound);
    }
    prevBounceSoundPlay = bounceSoundPlay;
  }

  // Update Bat
  Entity &bat = gameState.bat;
  {
    static int directionX = 1;
    static int directionY = 1;
    if (bat.pos.x < LEFT_WALL || bat.pos.x > RIGHT_WALL - 20)
    {
      directionX *= -1;
      sound_play(gameState.bounceSound);
    }
    if(bat.pos.y < TOP_WALL || bat.pos.y > BOTTOM_WALL - 20)
    {
      directionY *= -1;
      sound_play(gameState.bounceSound);
    }
    bat.pos.x += directionX * SPEED;
    bat.pos.y += directionY * SPEED;
  }

  // update score
  {
    if (collision_rects(get_entity_collider(gameState.player), get_entity_collider(gameState.bat)))
    {
      gameState.score += 100;
      SN_INFO("Score: %d", gameState.score);
      spawn_bat();
    }
  }

  player.animIdx = animate(player.spriteID, player.timer, 1.0f, dt);
  bat.animIdx = animate(bat.spriteID, bat.timer, 0.8f, dt);
}

void game_render()
{
  // render map
  for (int i=0; i < MAP_SIZE; ++i)
  {
    int r = i / MAP_COLS;
    int c = i % MAP_COLS;
    
    render_sprite((SpriteID)((int)SPRITE_WALL_00 + MAP[i]), {c * GRID_SIZE, r * GRID_SIZE});
  }

  // render bat
  {
    Entity &bat = gameState.bat;
    render_sprite(bat.spriteID, bat.pos, 1.0f, bat.animIdx);
  }

  // render player
  {
    Entity &player = gameState.player;
    render_sprite(player.spriteID, player.pos, 1.0f, player.animIdx);
  }

  // renderCollider
  if (gameState.debug)
  {
    // render bat collider
    {
      Entity &bat = gameState.bat;
      const Rect &rect = get_entity_collider(bat);
      render_quad(rect.pos, rect.size, {0, 255, 0});
    }
    
    // render player collider
    {
      Entity &player = gameState.player;
      const Rect &rect = get_entity_collider(player);
      render_quad(rect.pos, rect.size, {255, 0, 0});
    }
  }
}

void game_cleanup()
{
  music_stop(gameState.gameMusic);
  music_free(gameState.gameMusic);

  sound_free(gameState.bounceSound);
}
