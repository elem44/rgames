// From Collision
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
    float gravity = 2;
    float gvelocity = 2;

  // Main game loop
  while (!WindowShouldClose()) // Detect window close button or ESC key
  {
    // Update
    // Apply Gravity
    
    gvelocity = gravity + gvelocity;
    //----------------------------------------------------------------------------------
    if (IsKeyDown(KEY_RIGHT))
      ballPosition.x += 2.0f;
    if (IsKeyDown(KEY_LEFT))
      ballPosition.x -= 2.0f;
    if (IsKeyDown(KEY_UP))
      ballPosition.y -= 2.0f;
    if (IsKeyDown(KEY_DOWN))
      ballPosition.y += 2.0f;
    if (IsKeyDown(KEY_SPACE))
        ballPosition.y -= 55;
    //----------------------------------------------------------------------------------
    
    ballPosition.y += (gvelocity) * 0.2;

    // Clamp to screen
    if (ballPosition.x > 800)
    {
        ballPosition.x -= 2;
    }
    if (ballPosition.x < 0)
    {
        ballPosition.x += 2;
    }
    if (ballPosition.y < 0)
    {
        ballPosition.y += 2;
    }
    if (ballPosition.y > 450)
    {
        gvelocity = 2;
        ballPosition.y = 450;
    }
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
