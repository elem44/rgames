// From Camera
#include <raylib.h>
#include <raymath.h>
#include <stdio.h>

typedef struct Player {
    Vector2 position;
    Vector2 velocity;
    bool can_jump;
    float width;
    float height;
} Player;

typedef struct Level {
    Rectangle rect;
    Rectangle rect2;
} Level;

typedef struct Coin {
    Vector2 position;
    bool is_collected;
} Coin;
const int LEVEL_BOX_COUNT = 2;




const int G = 10;
const float COIN_SIZE = 10;


void update_player(Player *player);

int main(void) {
    int screen_width = 800;
    int screen_height = 600;

    int score = 0;

    Player player = {
        (Vector2){(float)screen_width / 2, (float)screen_height / 2},
        {0, 0},
        false,
        15,
        45,
    };
    Camera2D camera = { 0 };
    camera.target = player.position;
    camera.offset = (Vector2){ screen_width/2.0f, screen_height/2.0f };
    camera.rotation = 0.0f;
    camera.zoom = 1.0f;

    Rectangle platforms[LEVEL_BOX_COUNT];
    platforms[0] = (Rectangle) { 50, 500, 700, 80 };
    platforms[1] = (Rectangle) { 800, 500, 700, 80 };

    Level level = {
        platforms[0],
        platforms[1],
    };

    Coin coin = {
        (Vector2) { 850, 300 },
        false,
    };

    SetTargetFPS(60);
    InitWindow(screen_width, screen_height, "level");


    while (!WindowShouldClose()) {
        
        update_player(&player);

        for (int i = 0; i < LEVEL_BOX_COUNT; i++)
        {
            if (CheckCollisionRecs((Rectangle) {player.position.x, player.position.y, player.width, player.height}, platforms[i]) && player.position.y < platforms[i].y)
            {
                player.can_jump = true;
                player.position.y = platforms[i].y - player.height;
                player.velocity.y = 0;
            }
        }

        if (CheckCollisionCircleRec(coin.position, COIN_SIZE, (Rectangle) {player.position.x, player.position.y, player.width, player.height}) && !coin.is_collected)
        {
            coin.is_collected = true;
            score++;
        }

        camera.target = player.position;
        char str[5];
        sprintf(str, "%d", score);

        BeginDrawing();

            ClearBackground(RAYWHITE);
            DrawFPS(0,0);
            DrawText(str, 600, 100, 90, BLACK);
            
            BeginMode2D(camera);
            if (!coin.is_collected)
            {
                DrawCircleV(coin.position, COIN_SIZE, YELLOW);
            }
            DrawRectangle(player.position.x, player.position.y, player.width, player.height, BLUE);
            DrawRectangleRec(platforms[0], GRAY);
            DrawRectangleRec(platforms[1], BLACK);
            EndMode2D();
        EndDrawing();
    }

    CloseWindow();
}

void update_player(Player *player)
{
    if (IsKeyDown(KEY_LEFT)) 
    {
        player->velocity.x = -8;
    } 
    else if (IsKeyDown(KEY_RIGHT)) 
    {
        player->velocity.x = 8;
    } 
    else
    {
        player->velocity.x = 0;
    }

    if (player->can_jump)
    {
        if (IsKeyPressed(KEY_SPACE))
        {
            player->velocity.y -= 20;
            player->can_jump = false;
        }
    }
    player->velocity.y += (float)(G * 0.1);

    player->position = Vector2Add(player->position, player->velocity);
}



/*
 
        if (IsKeyDown(KEY_LEFT)) 
        {
            player.velocity.x = -8;
        } 
        else if (IsKeyDown(KEY_RIGHT)) 
        {
            player.velocity.x = 8;
        } 
        else
        {
            player.velocity.x = 0;
        }

        if (player.can_jump)
        {
            if (IsKeyPressed(KEY_SPACE))
            {
                player.velocity.y -= 20;
                player.can_jump = false;
            }
        }

        player.velocity.y += (float)(G * 0.1);

        player.position = Vector2Add(player.position, player.velocity);

*/
