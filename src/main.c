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

int main(void) {
  const int ScreenWidth = 800;
  const int ScreenHeight = 450;

  InitWindow(ScreenWidth, ScreenHeight,
             "Platform");

  SetTargetFPS(120);

  while (!WindowShouldClose()) {
    BeginDrawing();

    ClearBackground(BLACK);
     DrawFPS(2, 2);                                                     
    DrawText("Hello World", 500, 200, 10, WHITE);

    EndDrawing();
  }

  CloseWindow();

  return 0;
}
