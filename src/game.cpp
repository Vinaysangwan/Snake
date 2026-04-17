#include "pch.h"
#include "game.h"
#include "renderer.h"
#include "inputs.h"
#include "config.h"
#include "display.h"

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

Rect get_entity_collider(SpriteID spriteID, Vec2 pos)
{
  const Sprite &sprite = SPRITES[spriteID];

  return Rect{
    .pos = Vec2{pos.x + sprite.colliderOffset.x, pos.y + sprite.colliderOffset.y},
    .size = ivec2_f(sprite.colliderSize)
  };
} 

float move_to_target(Vec2 &pos, const Vec2 &destination, float speed)
{
  float xDistance = destination.x - pos.x;
  float yDistance = destination.y - pos.y;
  float distance = sqrtf(xDistance * xDistance + yDistance * yDistance);

  if (distance <= speed)
  {
    pos.x = destination.x;
    pos.y = destination.y;

    return 0.0f;
  }
  else
  {
    float xNormal = xDistance / distance;
    float yNormal = yDistance / distance;

    pos.x += xNormal * speed;
    pos.y += yNormal * speed;
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
    Snake &player = gameState.player;
    player.spriteID = SPRITE_SLIME;
    player.speed = 1.5f;
    player.direction = {1, 0};
    player.nextDirection = player.direction;

    // init player head
    SnakeSection head;
    head.pos = {7 * GRID_SIZE, 4 * GRID_SIZE};
    head.prevPos = head.pos;
    player.destination = head.pos;

    player.sections[player.sectionCount++] = head;
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
  Snake &player = gameState.player;
  {
    // update player head
    SnakeSection &head = player.sections[0];
    float distance = move_to_target(head.pos, player.destination, player.speed);
    
    // update player next direction
    if (key_down(GLFW_KEY_W) && player.direction.y != 1)
    {
      player.nextDirection = {0, -1};
    }
    else if (key_down(GLFW_KEY_S) && player.direction.y != -1) 
    {
      player.nextDirection = {0, 1};
    }
    else if (key_down(GLFW_KEY_A) && player.direction.x != 1) 
    {
      player.nextDirection = {-1, 0};
    }
    else if (key_down(GLFW_KEY_D) && player.direction.x != -1) 
    {
      player.nextDirection = {1, 0};
    }

    // player align to Grid
    if (distance == 0)
    {
      for (int i=0; i < player.sectionCount; ++i)
      {
        player.sections[i].prevPos = player.sections[i].pos;
      }

      // Update player direction
      player.direction = player.nextDirection;

      // update player destination
      player.destination.x = head.pos.x + player.direction.x * GRID_SIZE;
      player.destination.y = head.pos.y + player.direction.y * GRID_SIZE;
      
      // Wall Clamp
      if (player.destination.x < LEFT_WALL || player.destination.x > RIGHT_WALL - 20 || 
          player.destination.y < TOP_WALL || player.destination.y > BOTTOM_WALL - 20)
      {
        player.destination = head.pos;
      }
    }

    // update player sections [except head]
    for (int i=1; i < player.sectionCount; ++i)
    {
      SnakeSection &section = player.sections[i];
      SnakeSection &prevSection = player.sections[i-1];

      move_to_target(section.pos, prevSection.prevPos, player.speed);
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
    if (collision_rects(get_entity_collider(player.spriteID, player.sections[0].pos), get_entity_collider(bat.spriteID, bat.pos)))
    {
      gameState.score += 100;
      spawn_bat();
      sound_play(gameState.collectSound);

      // Add new Section to player
      if (player.sectionCount < MAX_SNAKE_SECTIONS)
      {
        const SnakeSection &tail = player.sections[player.sectionCount - 1];

        SnakeSection section;
        section.pos = tail.prevPos;

        player.sections[player.sectionCount++] = section;
      }
    }
  }

  // Game Debug Stuff
  {
    if (key_pressed(GLFW_KEY_Q))
    {
      SN_INFO("Music: is %s", music_is_playing(gameState.gameMusic) ? "Playing" : "Not Playing");
    }
  }

  // update animation index
  {
    player.animIdx = animate(player.spriteID, player.timer, 1.0f, dt);
    bat.animIdx = animate(bat.spriteID, bat.timer, 0.8f, dt);
  }
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

  // render player
  {
    Snake &player = gameState.player;

    // render player sections
    for (int i = player.sectionCount-1; i >= 0; --i)
    {
      SnakeSection &section = player.sections[i];
      render_sprite(player.spriteID, section.pos, 1.0f, player.animIdx);
    }
    render_sprite(player.spriteID, player.sections[0].pos, 1.0f, player.animIdx, {0, 0, 255, 255});
  }

  // render bat
  {
    Entity &bat = gameState.bat;
    render_sprite(bat.spriteID, bat.pos, 1.0f, bat.animIdx);
  }

  // render ui
  {
    render_ui_format_text({20, 0}, 1, {255, 255, 255, 255}, "SCORE: %d", gameState.score);
    render_ui_format_text({WINDOW_WIDTH - 250, 0}, 1, {255, 0, 0, 255}, "FPS: %d", get_fps());
  }

  // render debug
  if (gameState.debug)
  {
    render_ui_quad({0, 64}, {500, 200});
    
    // render player collider
    {
      Snake &player = gameState.player;
      const Rect &rect = get_entity_collider(player.spriteID, player.sections[0].pos);
      render_quad(rect.pos, rect.size, {255, 0, 0, 120});
      render_ui_format_text({20, 64}, 1, {255, 0, 0, 255}, "Player Speed: %d", (int)player.speed);
    }

    // render bat collider
    {
      Entity &bat = gameState.bat;
      const Rect &rect = get_entity_collider(bat.spriteID, bat.pos);
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
