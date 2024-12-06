#include <raylib.h>

int main(void)
{
    int screen_width = 800;
    int screen_height = 450;
    
    Vector2 target = { 400, 225 };
    InitWindow(screen_width, screen_height, "DrawRectangle Test");

    while (!WindowShouldClose())
    {
        ClearBackground(WHITE);

        BeginDrawing();
        Rectangle targetRect = { target.x - 20, target.y - 40, 10.0f, 80.0f };
        DrawRectangleRec(targetRect, RED);
        EndDrawing();
    }
}
