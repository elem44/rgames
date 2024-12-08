#include <raylib.h>
#include <stdlib.h>

typedef struct player {
    Vector2 position;
    float size;
    Color color;
}player;

typedef struct coin {
    Vector2 position;
    float size;
    Color color;
    bool is_collected;
} coin;


int score = 0;

int main(void)
{
    int screen_width = 800;
    int screen_height = 600;
    player ball = {
        (Vector2) { (float)screen_width / 2, (float) screen_height / 2 },
        50,
        RED,
    };

    coin coin = {
        (Vector2) { 100, 100 },
        15,
        YELLOW,
        false,
    };
    
    char score_display[1]; 
    score_display[0] = '0';

    InitWindow(screen_width, screen_height, "Coins");
    SetTargetFPS(60);

    while (!WindowShouldClose())
    {

        if (IsKeyDown(KEY_DOWN))
        {
            ball.position.y += 1;
        }
        if (IsKeyDown(KEY_UP))
        {
            ball.position.y -= 1;
        }
        if (IsKeyDown(KEY_LEFT))
        {
            ball.position.x -= 1;
        }
        if (IsKeyDown(KEY_RIGHT))
        {
            ball.position.x += 1;
        }

        if (CheckCollisionCircles(ball.position, ball.size, coin.position, coin.size))
        {
            coin.is_collected = true;
            score += 1;
            score_display[0] = '1';
        }

        BeginDrawing();

        ClearBackground(RAYWHITE);

        DrawText(score_display, 50, 50, 15, BLACK);

        DrawCircleV(ball.position, ball.size, ball.color);
        if (!coin.is_collected)
        {
            DrawCircleV(coin.position, coin.size, coin.color);
        }

        EndDrawing();

    }


    CloseWindow();
    return 0;
}


