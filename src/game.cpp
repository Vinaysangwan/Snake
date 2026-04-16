#include "pch.h"
#include "game.h"
#include "renderer.h"
#include "inputs.h"
#include "config.h"

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

float move_to_target(Entity &entity, const Vec2 &destination, float speed)
{
  float xDistance = destination.x - entity.pos.x;
  float yDistance = destination.y - entity.pos.y;
  float distance = sqrtf(xDistance * xDistance + yDistance * yDistance);

  if (distance <= speed)
  {
    entity.pos.x = destination.x;
    entity.pos.y = destination.y;
  }
  else
  {
    float xNormal = xDistance / distance;
    float yNormal = yDistance / distance;

    entity.pos.x += xNormal * speed;
    entity.pos.y += yNormal * speed;
  }

  return distance;
}

// #############################################################################
//                           Functions
// #############################################################################
void game_init()
{
  // init debug
  gameState.debug = false;
  
  // Init Camera
  {
    // game
    Camera &gameCamera = renderState.gameCamera;
    gameCamera.pos = {WORLD_WIDTH / 2, WORLD_HEIGHT / 2};
    gameCamera.size = {WORLD_WIDTH, WORLD_HEIGHT};

    // ui
    Camera &uiCamera = renderState.uiCamera;
    uiCamera.pos = {WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2};
    uiCamera.size = {WINDOW_WIDTH, WINDOW_HEIGHT};
  }
  
  // Init Player Data
  {
    Entity &player = gameState.player;
    player.spriteID = SPRITE_SLIME;
    const Sprite &sprite = SPRITES[player.spriteID];
    player.pos = {7 * GRID_SIZE, 4 * GRID_SIZE};
    player.speed = 1.0f;
  }

  // Init Bat Data
  {
    Entity &bat = gameState.bat;
    bat.spriteID = SPRITE_BAT;
    bat.pos = {WORLD_WIDTH / 2, WORLD_HEIGHT / 2};
    bat.speed = 1.0f;
    bat.speed = 1.0f;
  }

  // Init Music
  {
    gameState.gameMusic = music_load("assets/audio/theme.mp3");
    music_play(gameState.gameMusic);
  }

  // Init sounds
  {
    gameState.bounceSound = sound_load("assets/audio/bounce.wav");
    gameState.collectSound = sound_load("assets/audio/collect.wav");
  }
}

void game_update(float dt)
{
  if (key_pressed(GLFW_KEY_Z)) gameState.debug = !gameState.debug;
  
  // Update Player
  Entity &player = gameState.player;
  {
    static Vec2 destination = player.pos;
    float distance = move_to_target(player, destination, player.speed);

    if (distance == 0)
    {
      if (key_down(GLFW_KEY_W))
      {
        destination.y = player.pos.y - GRID_SIZE;

        if (destination.y < TOP_WALL) destination.y = player.pos.y;
      }
      else if (key_down(GLFW_KEY_S)) 
      {
        destination.y = player.pos.y + GRID_SIZE;

        if (destination.y > BOTTOM_WALL - 20) destination.y = player.pos.y;
      }
      else if (key_down(GLFW_KEY_A)) 
      {
        destination.x = player.pos.x - GRID_SIZE;

        if (destination.x < LEFT_WALL) destination.x = player.pos.x;
      }
      else if (key_down(GLFW_KEY_D)) 
      {
        destination.x = player.pos.x + GRID_SIZE;
        
        if (destination.x > RIGHT_WALL - 20) destination.x = player.pos.x;
      }
    }

    // Player Debug stuff
    if (key_pressed(GLFW_KEY_UP))
    {
      player.speed += 1;
      SN_INFO("Player Speed: %f", player.speed);
    }
    else if(key_pressed(GLFW_KEY_DOWN))
    {
      player.speed--;
      if (player.speed < 0)
      {
        player.speed = 0;
      }
      SN_INFO("Player Speed: %f", player.speed);
    }
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
    bat.pos.x += directionX * bat.speed;
    bat.pos.y += directionY * bat.speed;
  }

  // update score
  {
    if (collision_rects(get_entity_collider(player), get_entity_collider(bat)))
    {
      gameState.score += 100;
      SN_INFO("Score: %d", gameState.score);
      spawn_bat();
      sound_play(gameState.collectSound);
    }
  }

  // Game Debug Stuff
  {
    if (key_pressed(GLFW_KEY_Q))
    {
      SN_INFO("Music: is %s", music_is_playing(gameState.gameMusic) ? "Playing" : "Not Playing");
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

  // render ui
  {
    render_ui_format_text({20, 0}, 1, {255, 255, 255, 255}, "SCORE: %d", gameState.score);
  }

  // render debug
  if (gameState.debug)
  {
    render_ui_quad({0, 64}, {500, 200});
    
    // render player collider
    {;
      Entity &player = gameState.player;
      const Rect &rect = get_entity_collider(player);
      render_quad(rect.pos, rect.size, {255, 0, 0, 120});
      render_ui_format_text({20, 64}, 1, {255, 0, 0, 255}, "Player Speed: %d", (int)player.speed);
    }

    // render bat collider
    {
      Entity &bat = gameState.bat;
      const Rect &rect = get_entity_collider(bat);
      render_quad(rect.pos, rect.size, {0, 255, 0, 120});
      render_ui_format_text({20, 100}, 1, {255, 0, 0, 255}, "Bat Speed: %d", (int)bat.speed);
    }
  }
}

void game_cleanup()
{
  // free sound
  sound_free(gameState.bounceSound);
  sound_free(gameState.collectSound);

  // free music
  music_stop(gameState.gameMusic);
  music_free(gameState.gameMusic);
}
