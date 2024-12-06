#include <raylib.h>

int main(void)
{
    const int screen_width = 800;
    const int screen_height = 450;
    
    Vector2 ballPosition = {(float) screen_width / 2 , (float) screen_height / 2};

    InitWindow(screen_width, screen_height, "Test");
    SetTargetFPS(60);
    while (!WindowShouldClose())
    {
        if (IsKeyDown(KEY_RIGHT)) ballPosition.x += 2.0f;
        if (IsKeyDown(KEY_LEFT)) ballPosition.x -= 2.0f;
        if (IsKeyDown(KEY_UP)) ballPosition.y -= 2.0f;
        if (IsKeyDown(KEY_DOWN)) ballPosition.y += 2.0f;
        BeginDrawing();
            ClearBackground(RAYWHITE);
            DrawCircleV(ballPosition, 50, MAROON);
            DrawText("Use arrow keys to move the ball.", 100, 100, 50, BLACK);
        EndDrawing();

    }
    CloseWindow();

    return 0;
}
