#include "raylib.h"

//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------
int main(void) {
  // Initialization
  //--------------------------------------------------------------------------------------
  const int screenWidth = 800;
  const int screenHeight = 450;

  InitWindow(screenWidth, screenHeight,
             "raylib [core] example - keyboard input");

  Vector2 ballPosition = {(float)screenWidth / 2, (float)screenHeight / 2};

  SetTargetFPS(60); // Set our game to run at 60 frames-per-second
  //--------------------------------------------------------------------------------------

  // Main game loop
  while (!WindowShouldClose()) // Detect window close button or ESC key
  {
    // Update
    //----------------------------------------------------------------------------------
    if (IsKeyDown(KEY_RIGHT))
      ballPosition.x += 2.0f;
    if (IsKeyDown(KEY_LEFT))
      ballPosition.x -= 2.0f;
    if (IsKeyDown(KEY_UP))
      ballPosition.y -= 2.0f;
    if (IsKeyDown(KEY_DOWN))
      ballPosition.y += 2.0f;
    //----------------------------------------------------------------------------------

    // Create Rectangle
    Rectangle death_rectangle = {200, 200, 80, 80};

    // Create Constants
    int ball_radius = 50;
    Color background_color = RAYWHITE;
    if (CheckCollisionCircleRec(ballPosition, ball_radius, death_rectangle))
    {
      background_color = YELLOW;
    }
    else
    {
      background_color = RAYWHITE;
    }
    // Draw
    //----------------------------------------------------------------------------------
    BeginDrawing();

    ClearBackground(background_color);

    DrawText("move the ball with arrow keys", 10, 10, 20, DARKGRAY);

    DrawCircleV(ballPosition, 50, MAROON);

    // Draw Death Rectangle
    DrawRectangleRec(death_rectangle, RED);

    EndDrawing();
    //----------------------------------------------------------------------------------
  }

  // De-Initialization
  //--------------------------------------------------------------------------------------
  CloseWindow(); // Close window and OpenGL context
  //--------------------------------------------------------------------------------------

  return 0;
}
