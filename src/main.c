#include "raylib.h"
#include <stdint.h>
#include <stdio.h>

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

typedef struct {
  u8 Frame;
  f32 FrameTimer;
  f32 FrameDuration;
} Animation_State;

enum Player_State { PLAYER_IDLE, PLAYER_WALKING };
enum Player_Direction {
  RIGHT,
  LEFT,
  UP,
  DOWN,
  DOWNLEFT,
  DOWNRIGHT,
  UPRIGHT,
  UPLEFT
};

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

Rectangle GetPlayerMoveDownRightIdleSprite() {
  Rectangle source = {};
  source.x = 3;
  source.y = 31;
  source.width = 16;
  source.height = 16;
  return source;
}

Rectangle GetPlayerRightFootForwardDownRightSprite() {
  Rectangle source = {};
  source.x = 27;
  source.y = 31;
  source.width = 16;
  source.height = 16;
  return source;
}

Rectangle GetPlayerLeftFootForwardDownRightSprite() {
  Rectangle source = {};
  source.x = 74;
  source.y = 31;
  source.width = 16;
  source.height = 16;
  return source;
}

// TODO: this should be better
void GetAllWalkingDownRightAnimations(Rectangle *source) {
  source[0] = GetPlayerMoveDownRightIdleSprite();
  source[1] = GetPlayerRightFootForwardDownRightSprite();
  source[2] = GetPlayerMoveDownRightIdleSprite();
  source[3] = GetPlayerLeftFootForwardDownRightSprite();
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

Rectangle GetPlayerMoveUpRightIdleSprite() {
  Rectangle source = {};
  source.x = 3;
  source.y = 79;
  source.width = 16;
  source.height = 16;
  return source;
}

Rectangle GetPlayerRightFootForwardUpRightSprite() {
  Rectangle source = {};
  source.x = 27;
  source.y = 79;
  source.width = 16;
  source.height = 16;
  return source;
}

Rectangle GetPlayerLeftFootForwardUpRightSprite() {
  Rectangle source = {};
  source.x = 74;
  source.y = 79;
  source.width = 16;
  source.height = 16;
  return source;
}

// TODO: this should be better
void GetAllWalkingUpRightAnimations(Rectangle *source) {
  source[0] = GetPlayerMoveUpRightIdleSprite();
  source[1] = GetPlayerRightFootForwardUpRightSprite();
  source[2] = GetPlayerMoveUpRightIdleSprite();
  source[3] = GetPlayerLeftFootForwardUpRightSprite();
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

Rectangle GetPlayerMoveDownLeftIdleSprite() {
  Rectangle source = {};
  source.x = 4;
  source.y = 31;
  source.width = -16;
  source.height = 16;
  return source;
}

Rectangle GetPlayerRightFootForwardDownLeftSprite() {
  Rectangle source = {};
  source.x = 28;
  source.y = 31;
  source.width = -16;
  source.height = 16;
  return source;
}

Rectangle GetPlayerLeftFootForwardDownLeftSprite() {
  Rectangle source = {};
  source.x = 75;
  source.y = 31;
  source.width = -16;
  source.height = 16;
  return source;
}

// TODO: this should be better
void GetAllWalkingDownLeftAnimations(Rectangle *source) {
  source[0] = GetPlayerMoveDownLeftIdleSprite();
  source[1] = GetPlayerRightFootForwardDownLeftSprite();
  source[2] = GetPlayerMoveDownLeftIdleSprite();
  source[3] = GetPlayerLeftFootForwardDownLeftSprite();
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

Rectangle GetPlayerMoveUpLeftIdleSprite() {
  Rectangle source = {};
  source.x = 4;
  source.y = 79;
  source.width = -16;
  source.height = 16;
  return source;
}

Rectangle GetPlayerRightFootForwardUpLeftSprite() {
  Rectangle source = {};
  source.x = 28;
  source.y = 79;
  source.width = -16;
  source.height = 16;
  return source;
}

Rectangle GetPlayerLeftFootForwardUpLeftSprite() {
  Rectangle source = {};
  source.x = 75;
  source.y = 79;
  source.width = -16;
  source.height = 16;
  return source;
}

// TODO: this should be better
void GetAllWalkingUpLeftAnimations(Rectangle *source) {
  source[0] = GetPlayerMoveUpLeftIdleSprite();
  source[1] = GetPlayerRightFootForwardUpLeftSprite();
  source[2] = GetPlayerMoveUpLeftIdleSprite();
  source[3] = GetPlayerLeftFootForwardUpLeftSprite();
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

Rectangle GetPlayerDownRightIdleSprite() {
  Rectangle source = {};
  source.x = 3;
  source.y = 31;
  source.width = 16;
  source.height = 16;
  return source;
}

Rectangle GetPlayerDownRightIdle2Sprite() {
  Rectangle source = {};
  source.x = 27;
  source.y = 31;
  source.width = 16;
  source.height = 16;
  return source;
}

// TODO: this should be better
void GetAllDownRightIdleAnimations(Rectangle *source) {
  source[0] = GetPlayerDownRightIdleSprite();
  source[1] = GetPlayerDownRightIdle2Sprite();
  source[2] = GetPlayerDownRightIdle2Sprite();
  source[3] = GetPlayerDownRightIdleSprite();
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

Rectangle GetPlayerUpRightIdleSprite() {
  Rectangle source = {};
  source.x = 3;
  source.y = 80;
  source.width = 16;
  source.height = 16;
  return source;
}

Rectangle GetPlayerUpRightIdle2Sprite() {
  Rectangle source = {};
  source.x = 27;
  source.y = 80;
  source.width = 16;
  source.height = 16;
  return source;
}

// TODO: this should be better
void GetAllUpRightIdleAnimations(Rectangle *source) {
  source[0] = GetPlayerUpRightIdleSprite();
  source[1] = GetPlayerUpRightIdle2Sprite();
  source[2] = GetPlayerUpRightIdle2Sprite();
  source[3] = GetPlayerUpRightIdleSprite();
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

Rectangle GetPlayerDownLeftIdleSprite() {
  Rectangle source = {};
  source.x = 3;
  source.y = 32;
  source.width = -16;
  source.height = 16;
  return source;
}

Rectangle GetPlayerDownLeftIdle2Sprite() {
  Rectangle source = {};
  source.x = 27;
  source.y = 32;
  source.width = -16;
  source.height = 16;
  return source;
}

// TODO: this should be better
void GetAllDownLeftIdleAnimations(Rectangle *source) {
  source[0] = GetPlayerDownLeftIdleSprite();
  source[1] = GetPlayerDownLeftIdle2Sprite();
  source[2] = GetPlayerDownLeftIdle2Sprite();
  source[3] = GetPlayerDownLeftIdleSprite();
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

Rectangle GetPlayerUpLeftIdleSprite() {
  Rectangle source = {};
  source.x = 2;
  source.y = 80;
  source.width = -16;
  source.height = 16;
  return source;
}

Rectangle GetPlayerUpLeftIdle2Sprite() {
  Rectangle source = {};
  source.x = 26;
  source.y = 80;
  source.width = -16;
  source.height = 16;
  return source;
}

// TODO: this should be better
void GetAllUpLeftIdleAnimations(Rectangle *source) {
  source[0] = GetPlayerUpLeftIdleSprite();
  source[1] = GetPlayerUpLeftIdle2Sprite();
  source[2] = GetPlayerUpLeftIdle2Sprite();
  source[3] = GetPlayerUpLeftIdleSprite();
}

int main(void) {
  const i32 screenWidth = 800;
  const i32 screenHeight = 450;

  InitWindow(screenWidth, screenHeight, "Platform");
  Texture2D propsTexture = LoadTexture("assets/Map/Texture/TX Props.png");
  Texture2D playerWalkTexture =
      LoadTexture("assets/Char/16x16/16x16 Walk-Sheet.png");
  Texture2D playerIdleTexture =
      LoadTexture("assets/Char/16x16/16x16 Idle-Sheet.png");

  SetTargetFPS(60);

  Rectangle walkingDownAnimationsources[4] = {};
  GetAllWalkingDownAnimations(walkingDownAnimationsources);
  Rectangle walkingDownRightAnimationsources[4] = {};
  GetAllWalkingDownRightAnimations(walkingDownRightAnimationsources);
  Rectangle walkingRightAnimationsources[4] = {};
  GetAllWalkingRightAnimations(walkingRightAnimationsources);
  Rectangle walkingUpRightAnimationsources[4] = {};
  GetAllWalkingUpRightAnimations(walkingUpRightAnimationsources);
  Rectangle walkingUpAnimationsources[4] = {};
  GetAllWalkingUpAnimations(walkingUpAnimationsources);
  Rectangle walkingDownLeftAnimationsources[4] = {};
  GetAllWalkingDownLeftAnimations(walkingDownLeftAnimationsources);
  Rectangle walkingLeftAnimationsources[4] = {};
  GetAllWalkingLeftAnimations(walkingLeftAnimationsources);
  Rectangle walkingUpLeftAnimationsources[4] = {};
  GetAllWalkingUpLeftAnimations(walkingUpLeftAnimationsources);

  Rectangle idleDownAnimationsources[4] = {};
  GetAllDownIdleAnimations(idleDownAnimationsources);
  Rectangle idleDownRightAnimationsources[4] = {};
  GetAllDownRightIdleAnimations(idleDownRightAnimationsources);
  Rectangle idleRightAnimationsources[4] = {};
  GetAllRightIdleAnimations(idleRightAnimationsources);
  Rectangle idleUpRightAnimationsources[4] = {};
  GetAllUpRightIdleAnimations(idleUpRightAnimationsources);
  Rectangle idleUpAnimationsources[4] = {};
  GetAllUpIdleAnimations(idleUpAnimationsources);
  Rectangle idleUpLeftAnimationsources[4] = {};
  GetAllUpLeftIdleAnimations(idleUpLeftAnimationsources);
  Rectangle idleLeftAnimationsources[4] = {};
  GetAllLeftIdleAnimations(idleLeftAnimationsources);
  Rectangle idleDownLeftAnimationsources[4] = {};
  GetAllDownLeftIdleAnimations(idleDownLeftAnimationsources);

  Rectangle source = {};
  Rectangle destination = {screenWidth / 2 - 32, screenHeight / 2 - 32, 64, 64};
  Animation_State walkingAnimationState = {};
  walkingAnimationState.FrameDuration = 0.15f;
  Animation_State idleAnimationState = {};
  idleAnimationState.FrameDuration = 0.15f;
  enum Player_State playerState = PLAYER_IDLE;
  enum Player_Direction playerDirection = DOWN;
  u16 fps = 0;
  u8 InfoBuffer[1024];
  f32 dt = 0.0f;
  while (!WindowShouldClose()) {
    fps = GetFPS();
    dt = GetFrameTime();
    playerState = PLAYER_IDLE;

    if (IsKeyDown(KEY_DOWN)) {
      playerState = PLAYER_WALKING;
      playerDirection = DOWN;
    }
    if (IsKeyDown(KEY_UP)) {
      playerState = PLAYER_WALKING;
      playerDirection = UP;
    }
    if (IsKeyDown(KEY_RIGHT)) {
      playerState = PLAYER_WALKING;
      playerDirection = RIGHT;
    }
    if (IsKeyDown(KEY_LEFT)) {
      playerState = PLAYER_WALKING;
      playerDirection = LEFT;
    }
    if (IsKeyDown(KEY_DOWN) && IsKeyDown(KEY_RIGHT)) {
      playerState = PLAYER_WALKING;
      playerDirection = DOWNRIGHT;
    }
    if (IsKeyDown(KEY_DOWN) && IsKeyDown(KEY_LEFT)) {
      playerState = PLAYER_WALKING;
      playerDirection = DOWNLEFT;
    }
    if (IsKeyDown(KEY_UP) && IsKeyDown(KEY_RIGHT)) {
      playerState = PLAYER_WALKING;
      playerDirection = UPRIGHT;
    }
    if (IsKeyDown(KEY_UP) && IsKeyDown(KEY_LEFT)) {
      playerState = PLAYER_WALKING;
      playerDirection = UPLEFT;
    }

    BeginDrawing();
    ClearBackground(SKYBLUE);
    if (playerState == PLAYER_IDLE) {
      idleAnimationState.FrameTimer += dt;
      if (idleAnimationState.FrameTimer >= idleAnimationState.FrameDuration) {
        idleAnimationState.Frame = (idleAnimationState.Frame + 1) % 4;
        idleAnimationState.FrameTimer -= idleAnimationState.FrameDuration;
      }

      switch (playerDirection) {
      case DOWN: {
        source = idleDownAnimationsources[idleAnimationState.Frame];
      } break;
      case DOWNRIGHT: {
        source = idleDownRightAnimationsources[idleAnimationState.Frame];
      } break;
      case RIGHT: {
        source = idleRightAnimationsources[idleAnimationState.Frame];
      } break;
      case UPRIGHT: {
        source = idleUpRightAnimationsources[idleAnimationState.Frame];
      } break;
      case UP: {
        source = idleUpAnimationsources[idleAnimationState.Frame];
      } break;
      case UPLEFT: {
        source = idleUpLeftAnimationsources[idleAnimationState.Frame];
      } break;
      case LEFT: {
        source = idleLeftAnimationsources[idleAnimationState.Frame];
      } break;
      case DOWNLEFT: {
        source = idleDownLeftAnimationsources[idleAnimationState.Frame];
      } break;
      }
      DrawTexturePro(playerIdleTexture, source, destination, (Vector2){0, 0},
                     0.0f, WHITE);

      walkingAnimationState.Frame = 0;
    }
    if (playerState == PLAYER_WALKING) {
      walkingAnimationState.FrameTimer += dt;
      if (walkingAnimationState.FrameTimer >=
          walkingAnimationState.FrameDuration) {
        walkingAnimationState.Frame = (walkingAnimationState.Frame + 1) % 4;
        walkingAnimationState.FrameTimer -= walkingAnimationState.FrameDuration;
      }

      switch (playerDirection) {
      case DOWN: {
        source = walkingDownAnimationsources[walkingAnimationState.Frame];
      } break;
      case DOWNRIGHT: {
        source = walkingDownRightAnimationsources[walkingAnimationState.Frame];
      } break;
      case RIGHT: {
        source = walkingRightAnimationsources[walkingAnimationState.Frame];
      } break;
      case UPRIGHT: {
        source = walkingUpRightAnimationsources[walkingAnimationState.Frame];
      } break;
      case UP: {
        source = walkingUpAnimationsources[walkingAnimationState.Frame];
      } break;
      case UPLEFT: {
        source = walkingUpLeftAnimationsources[walkingAnimationState.Frame];
      } break;
      case LEFT: {
        source = walkingLeftAnimationsources[walkingAnimationState.Frame];
      } break;
      case DOWNLEFT: {
        source = walkingDownLeftAnimationsources[walkingAnimationState.Frame];
      } break;
      }
      DrawTexturePro(playerWalkTexture, source, destination, (Vector2){0, 0},
                     0.0f, WHITE);
      idleAnimationState.Frame = 0;
    }

    snprintf(InfoBuffer, sizeof(InfoBuffer), "Fps: %d ", fps);
    DrawText(InfoBuffer, 2, 2, 10, BLACK);

    EndDrawing();
  }

  CloseWindow();

  return 0;
}
