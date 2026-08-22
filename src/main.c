#include "raylib.h"
#include "raymath.h"
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#define KiB(n) ((n) << 10)
#define MiB(n) ((n) << 20)
#define GiB(n) ((n) << 30)

#define MAX(a, b) (((a) > (b)) ? (a) : (b))
#define MIN(a, b) (((a) < (b)) ? (a) : (b))

#define ASSERT(Expression)                                                     \
  if (!(Expression)) {                                                         \
    *(char *)0 = 0;                                                            \
  }

#define ALIGN_UP(n, align) (((n) + (align) - 1) & ~((align) - 1))
#define DEFAULT_ALIGNMENT (sizeof(void *))

#define XBOX_ALIAS_1 "xbox"
#define XBOX_ALIAS_2 "x-box"
#define PS_ALIAS_1 "playstation"
#define PS_ALIAS_2 "sony"

typedef float f32;
typedef double f64;

typedef uint64_t u64;
typedef uint32_t u32;
typedef uint16_t u16;
typedef uint8_t u8;

typedef int64_t i64;
typedef int32_t i32;
typedef int16_t i16;
typedef int8_t i8;

typedef i32 b32;

// NOTE: stack based arena
typedef struct {
  u8 *buffer;
  u64 capacity;
  u64 pos;
} Mem_Arena;

typedef struct {
  u8 frame;
  f32 frameTimer;
  f32 frameDuration;
} Animation_State;

typedef enum { PLAYER_IDLE, PLAYER_WALKING } Player_State;
typedef enum {
  RIGHT,
  LEFT,
  UP,
  DOWN,
} Player_Direction;

typedef struct {
  b32 up;
  b32 down;
  b32 right;
  b32 left;
  b32 hit;
} Input_State;

typedef struct {
  Vector2 position;
  f32 speed;
  Player_State playerState;
  Player_Direction playerDirection;
  Animation_State walkingAnimation;
  Animation_State idleAnimation;
} Player_Info;

const float leftStickDeadzoneX = 0.1f;
const float leftStickDeadzoneY = 0.1f;
const float rightStickDeadzoneX = 0.1f;
const float rightStickDeadzoneY = 0.1f;
const float leftTriggerDeadzone = -0.9f;
const float rightTriggerDeadzone = -0.9f;

void ArenaCreate(Mem_Arena *arena, void *backingBuffer, u64 capacity) {
  arena->buffer = backingBuffer;
  arena->capacity = capacity;
  arena->pos = 0;
}

void ArenaClear(Mem_Arena *arena) {
  memset((void *)arena->buffer, 0, arena->pos);
  arena->pos = 0;
}

void *ArenaPush(Mem_Arena *arena, u64 size) {
  u64 alignedPos = ALIGN_UP(arena->pos, DEFAULT_ALIGNMENT);
  ASSERT(alignedPos + size <= arena->capacity)
  arena->pos = alignedPos + size;
  void *mem = arena->buffer + alignedPos;
  memset(mem, 0, size);
  return mem;
}

void ArenaPop(Mem_Arena *arena, u64 size) {
  ASSERT(arena->pos -= size >= 0)
  arena->pos -= size;
}

void ArenaPopTo(Mem_Arena *arena, u64 pos) {
  ASSERT(arena->pos >= 0)
  arena->pos = pos;
}

void GetKeyboardInput(Input_State *input) {
  if (IsKeyDown(KEY_DOWN)) {
    input->down = true;
  }
  if (IsKeyDown(KEY_UP)) {
    input->up = true;
  }
  if (IsKeyDown(KEY_RIGHT)) {
    input->right = true;
  }
  if (IsKeyDown(KEY_LEFT)) {
    input->left = true;
  }
}

void GetGamepadInput(Input_State *input) {
  i32 gamepad = 0;
  while (IsGamepadAvailable(gamepad)) {
    if (IsGamepadButtonDown(gamepad, GAMEPAD_BUTTON_LEFT_FACE_DOWN)) {
      input->down = true;
    }
    if (IsGamepadButtonDown(gamepad, GAMEPAD_BUTTON_LEFT_FACE_UP)) {
      input->up = true;
    }
    if (IsGamepadButtonDown(gamepad, GAMEPAD_BUTTON_LEFT_FACE_RIGHT)) {
      input->right = true;
    }
    if (IsGamepadButtonDown(gamepad, GAMEPAD_BUTTON_LEFT_FACE_LEFT)) {
      input->left = true;
    }

    f32 axisX = GetGamepadAxisMovement(gamepad, GAMEPAD_AXIS_LEFT_X);
    f32 axisY = GetGamepadAxisMovement(gamepad, GAMEPAD_AXIS_LEFT_Y);

    if (axisY > leftStickDeadzoneY) {
      input->down = true;
    }
    if (axisY < -leftStickDeadzoneY) {
      input->up = true;
    }
    if (axisX > leftStickDeadzoneX) {
      input->right = true;
    }
    if (axisX < -leftStickDeadzoneX) {
      input->left = true;
    }

    ++gamepad;
  }
}

void Input(Player_Info *player, f32 dt) {

  Vector2 deltaPosition = {};
  Input_State input = {};
  GetKeyboardInput(&input);
  GetGamepadInput(&input);
  if (input.down) {
    player->playerDirection = DOWN;
    deltaPosition.y += 1.0f;
  }
  if (input.up) {
    player->playerDirection = UP;
    deltaPosition.y -= 1.0f;
  }
  if (input.right) {
    player->playerDirection = RIGHT;
    deltaPosition.x += 1.0f;
  }
  if (input.left) {
    player->playerDirection = LEFT;
    deltaPosition.x -= 1.0f;
  }

  if (Vector2Length(deltaPosition) > 0) {
    player->playerState = PLAYER_WALKING;
    deltaPosition = Vector2Normalize(deltaPosition);

    player->position.x += deltaPosition.x * player->speed * dt;
    player->position.y += deltaPosition.y * player->speed * dt;
  } else {
    player->playerState = PLAYER_IDLE;
  }
}

//===============Player Move========================
Rectangle GetPlayerMoveDownIdleSprite() {
  Rectangle source = {};
  source.x = 3;
  source.y = 7;
  source.width = 16;
  source.height = 16;
  return source;
}

Rectangle GetPlayerRightFootForwardDownSprite() {
  Rectangle source = {};
  source.x = 27;
  source.y = 7;
  source.width = 16;
  source.height = 16;
  return source;
}

Rectangle GetPlayerLeftFootForwardDownSprite() {
  Rectangle source = {};
  source.x = 74;
  source.y = 7;
  source.width = 16;
  source.height = 16;
  return source;
}
// TODO: this should be better
void GetAllWalkingDownAnimations(Rectangle *source) {
  source[0] = GetPlayerMoveDownIdleSprite();
  source[1] = GetPlayerRightFootForwardDownSprite();
  source[2] = GetPlayerMoveDownIdleSprite();
  source[3] = GetPlayerLeftFootForwardDownSprite();
}

Rectangle GetPlayerMoveRightIdleSprite() {
  Rectangle source = {};
  source.x = 3;
  source.y = 55;
  source.width = 16;
  source.height = 16;
  return source;
}

Rectangle GetPlayerRightFootForwardRightSprite() {
  Rectangle source = {};
  source.x = 27;
  source.y = 55;
  source.width = 16;
  source.height = 16;
  return source;
}

Rectangle GetPlayerLeftFootForwardRightSprite() {
  Rectangle source = {};
  source.x = 74;
  source.y = 55;
  source.width = 16;
  source.height = 16;
  return source;
}

// TODO: this should be better
void GetAllWalkingRightAnimations(Rectangle *source) {
  source[0] = GetPlayerMoveRightIdleSprite();
  source[1] = GetPlayerRightFootForwardRightSprite();
  source[2] = GetPlayerMoveRightIdleSprite();
  source[3] = GetPlayerLeftFootForwardRightSprite();
}

Rectangle GetPlayerMoveUpIdleSprite() {
  Rectangle source = {};
  source.x = 3;
  source.y = 103;
  source.width = 16;
  source.height = 16;
  return source;
}

Rectangle GetPlayerRightFootForwardUpSprite() {
  Rectangle source = {};
  source.x = 74;
  source.y = 103;
  source.width = 16;
  source.height = 16;
  return source;
}

Rectangle GetPlayerLeftFootForwardUpSprite() {
  Rectangle source = {};
  source.x = 27;
  source.y = 103;
  source.width = 16;
  source.height = 16;
  return source;
}

// TODO: this should be better
void GetAllWalkingUpAnimations(Rectangle *source) {
  source[0] = GetPlayerMoveUpIdleSprite();
  source[1] = GetPlayerRightFootForwardUpSprite();
  source[2] = GetPlayerMoveUpIdleSprite();
  source[3] = GetPlayerLeftFootForwardUpSprite();
}

Rectangle GetPlayerMoveLeftIdleSprite() {
  Rectangle source = {};
  source.x = 4;
  source.y = 55;
  source.width = -16;
  source.height = 16;
  return source;
}

Rectangle GetPlayerRightFootForwardLeftSprite() {
  Rectangle source = {};
  source.x = 28;
  source.y = 55;
  source.width = -16;
  source.height = 16;
  return source;
}

Rectangle GetPlayerLeftFootForwardLeftSprite() {
  Rectangle source = {};
  source.x = 75;
  source.y = 55;
  source.width = -16;
  source.height = 16;
  return source;
}

// TODO: this should be better
void GetAllWalkingLeftAnimations(Rectangle *source) {
  source[0] = GetPlayerMoveLeftIdleSprite();
  source[1] = GetPlayerRightFootForwardLeftSprite();
  source[2] = GetPlayerMoveLeftIdleSprite();
  source[3] = GetPlayerLeftFootForwardLeftSprite();
}

//===============Player Idle========================
Rectangle GetPlayerDownIdleSprite() {
  Rectangle source = {};
  source.x = 3;
  source.y = 8;
  source.width = 16;
  source.height = 16;
  return source;
}

Rectangle GetPlayerDownIdle2Sprite() {
  Rectangle source = {};
  source.x = 27;
  source.y = 8;
  source.width = 16;
  source.height = 16;
  return source;
}

// TODO: this should be better
void GetAllDownIdleAnimations(Rectangle *source) {
  source[0] = GetPlayerDownIdleSprite();
  source[1] = GetPlayerDownIdle2Sprite();
  source[2] = GetPlayerDownIdle2Sprite();
  source[3] = GetPlayerDownIdleSprite();
}

Rectangle GetPlayerRightIdleSprite() {
  Rectangle source = {};
  source.x = 3;
  source.y = 56;
  source.width = 16;
  source.height = 16;
  return source;
}

Rectangle GetPlayerRightIdle2Sprite() {
  Rectangle source = {};
  source.x = 27;
  source.y = 56;
  source.width = 16;
  source.height = 16;
  return source;
}

// TODO: this should be better
void GetAllRightIdleAnimations(Rectangle *source) {
  source[0] = GetPlayerRightIdleSprite();
  source[1] = GetPlayerRightIdle2Sprite();
  source[2] = GetPlayerRightIdle2Sprite();
  source[3] = GetPlayerRightIdleSprite();
}

Rectangle GetPlayerUpIdleSprite() {
  Rectangle source = {};
  source.x = 3;
  source.y = 103;
  source.width = 16;
  source.height = 16;
  return source;
}

Rectangle GetPlayerUpIdle2Sprite() {
  Rectangle source = {};
  source.x = 27;
  source.y = 103;
  source.width = 16;
  source.height = 16;
  return source;
}

// TODO: this should be better
void GetAllUpIdleAnimations(Rectangle *source) {
  source[0] = GetPlayerUpIdleSprite();
  source[1] = GetPlayerUpIdle2Sprite();
  source[2] = GetPlayerUpIdle2Sprite();
  source[3] = GetPlayerUpIdleSprite();
}

Rectangle GetPlayerLeftIdleSprite() {
  Rectangle source = {};
  source.x = 3;
  source.y = 56;
  source.width = -16;
  source.height = 16;
  return source;
}

Rectangle GetPlayerLeftIdle2Sprite() {
  Rectangle source = {};
  source.x = 27;
  source.y = 56;
  source.width = -16;
  source.height = 16;
  return source;
}

// TODO: this should be better
void GetAllLeftIdleAnimations(Rectangle *source) {
  source[0] = GetPlayerLeftIdleSprite();
  source[1] = GetPlayerLeftIdle2Sprite();
  source[2] = GetPlayerLeftIdle2Sprite();
  source[3] = GetPlayerLeftIdleSprite();
}

void DrawPlayer(Texture2D playerWalkTexture, Texture2D playerIdleTexture,
                f32 dt, Player_Info *player) {

  Rectangle walkingDownAnimationsources[4] = {};
  GetAllWalkingDownAnimations(walkingDownAnimationsources);
  Rectangle walkingUpAnimationsources[4] = {};
  GetAllWalkingUpAnimations(walkingUpAnimationsources);
  Rectangle walkingRightAnimationsources[4] = {};
  GetAllWalkingRightAnimations(walkingRightAnimationsources);
  Rectangle walkingLeftAnimationsources[4] = {};
  GetAllWalkingLeftAnimations(walkingLeftAnimationsources);

  Rectangle idleDownAnimationsources[4] = {};
  GetAllDownIdleAnimations(idleDownAnimationsources);
  Rectangle idleUpAnimationsources[4] = {};
  GetAllUpIdleAnimations(idleUpAnimationsources);
  Rectangle idleRightAnimationsources[4] = {};
  GetAllRightIdleAnimations(idleRightAnimationsources);
  Rectangle idleLeftAnimationsources[4] = {};
  GetAllLeftIdleAnimations(idleLeftAnimationsources);

  Rectangle source = {};
  Rectangle destination = {player->position.x - 16, player->position.y - 16, 32,
                           32};

  if (player->playerState == PLAYER_IDLE) {
    player->idleAnimation.frameTimer += dt;
    if (player->idleAnimation.frameTimer >=
        player->idleAnimation.frameDuration) {
      player->idleAnimation.frame = (player->idleAnimation.frame + 1) % 4;
      player->idleAnimation.frameTimer -= player->idleAnimation.frameDuration;
    }

    switch (player->playerDirection) {
    case DOWN: {
      source = idleDownAnimationsources[player->idleAnimation.frame];
    } break;
    case RIGHT: {
      source = idleRightAnimationsources[player->idleAnimation.frame];
    } break;
    case UP: {
      source = idleUpAnimationsources[player->idleAnimation.frame];
    } break;
    case LEFT: {
      source = idleLeftAnimationsources[player->idleAnimation.frame];
    } break;
    }
    DrawTexturePro(playerIdleTexture, source, destination, (Vector2){0, 0},
                   0.0f, WHITE);

    player->walkingAnimation.frame = 0;
  }
  if (player->playerState == PLAYER_WALKING) {
    player->walkingAnimation.frameTimer += dt;
    if (player->walkingAnimation.frameTimer >=
        player->walkingAnimation.frameDuration) {
      player->walkingAnimation.frame = (player->walkingAnimation.frame + 1) % 4;
      player->walkingAnimation.frameTimer -=
          player->walkingAnimation.frameDuration;
    }

    switch (player->playerDirection) {
    case DOWN: {
      source = walkingDownAnimationsources[player->walkingAnimation.frame];
    } break;
    case RIGHT: {
      source = walkingRightAnimationsources[player->walkingAnimation.frame];
    } break;
    case UP: {
      source = walkingUpAnimationsources[player->walkingAnimation.frame];
    } break;
    case LEFT: {
      source = walkingLeftAnimationsources[player->walkingAnimation.frame];
    } break;
    }
    DrawTexturePro(playerWalkTexture, source, destination, (Vector2){0, 0},
                   0.0f, WHITE);
    player->idleAnimation.frame = 0;
  }
  // DrawCircle(player->position.x, player->position.y, 5, RED);
}

void DrawMap(Texture2D grassTexture) {
  Rectangle grass[4] = {{0, 0, 128, 128},
                        {128, 0, 128, 128},
                        {0, 128, 128, 128},
                        {128, 128, 128, 128}};
  int map[10][10] = {
      {
          1,
          1,
          1,
          1,
          1,
          1,
          1,
          1,
          1,
          1,
      },
      {
          1,
          1,
          1,
          1,
          1,
          1,
          1,
          1,
          1,
          1,
      },
      {
          1,
          1,
          1,
          1,
          1,
          1,
          1,
          1,
          1,
          1,
      },
      {
          1,
          1,
          1,
          1,
          1,
          1,
          1,
          1,
          1,
          1,
      },
      {
          1,
          1,
          1,
          1,
          1,
          1,
          1,
          1,
          1,
          1,
      },
      {
          1,
          1,
          1,
          1,
          1,
          1,
          1,
          1,
          1,
          1,
      },
      {
          1,
          1,
          1,
          1,
          1,
          1,
          1,
          1,
          1,
          1,
      },
      {
          1,
          1,
          1,
          1,
          1,
          1,
          1,
          1,
          1,
          1,
      },
      {
          1,
          1,
          1,
          1,
          1,
          1,
          1,
          1,
          1,
          1,
      },
      {
          1,
          1,
          1,
          1,
          1,
          1,
          1,
          1,
          1,
          1,
      },
  };
  const int TILE_SIZE = 128;
  for (int y = 0; y < 10; y++) {
    for (int x = 0; x < 10; x++) {
      Rectangle destination = {x * TILE_SIZE, y * TILE_SIZE, TILE_SIZE,
                               TILE_SIZE};

      DrawTexturePro(grassTexture, grass[map[y][x]], destination,
                     (Vector2){0, 0}, 0.0f, WHITE);
    }
  }
}

int main(void) {
  const i32 screenWidth = 800;
  const i32 screenHeight = 450;
  SetConfigFlags(FLAG_MSAA_4X_HINT);
  InitWindow(screenWidth, screenHeight, "Platform");
  Texture2D propsTexture = LoadTexture("assets/Map/Texture/TX Props.png");
  Texture2D playerWalkTexture =
      LoadTexture("assets/Char/16x16/16x16 Walk-Sheet.png");
  Texture2D playerIdleTexture =
      LoadTexture("assets/Char/16x16/16x16 Idle-Sheet.png");
  Texture2D grassTexture =
      LoadTexture("assets/Map/Texture/TX Tileset Grass.png");
  SetTextureFilter(grassTexture, TEXTURE_FILTER_POINT);

  SetTargetFPS(60);

  Rectangle source = {};
  Player_Info player = {};
  player.position.x = screenWidth / 2;
  player.position.y = screenHeight / 2;
  player.speed = 125.0f;
  player.playerState = PLAYER_IDLE;
  player.playerDirection = DOWN;
  player.walkingAnimation.frameDuration = 0.15f;
  player.idleAnimation.frameDuration = 0.15f;
  Player_Info enemy = {};
  enemy.position.x = 100;
  enemy.position.y = 100;
  enemy.playerState = PLAYER_IDLE;
  enemy.playerDirection = DOWN;
  enemy.idleAnimation.frameDuration = 0.30f;
  u16 fps = 0;
  u8 InfoBuffer[1024];
  f32 dt = 0.0f;
  Vector2 deltaPosition = {};
  while (!WindowShouldClose()) {
    fps = GetFPS();
    dt = GetFrameTime();
    player.playerState = PLAYER_IDLE;
    Input(&player, dt);

    BeginDrawing();
    ClearBackground(SKYBLUE);
    DrawMap(grassTexture);
    DrawPlayer(playerWalkTexture, playerIdleTexture, dt, &enemy);
    DrawPlayer(playerWalkTexture, playerIdleTexture, dt, &player);

    snprintf(InfoBuffer, sizeof(InfoBuffer), "Fps: %d ", fps);
    DrawText(InfoBuffer, 2, 2, 10, BLACK);

    EndDrawing();
  }

  CloseWindow();

  return 0;
}
