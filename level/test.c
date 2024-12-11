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

typedef struct EnvRect {
    Rectangle rec;
    Color color;
} EnvRect;

typedef struct Coin {
    Vector2 position;
    bool is_collected;
} Coin;

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;
const int FPS = 60;


const int BOX_COUNT = 4;
const int COIN_COUNT = 3;




const int G = 10;
const float COIN_SIZE = 10;


void start_program(void);
void update_player(Player *player);
void player_rectangle_collision(Player *player, Rectangle rec);

int main(void) {

    int score = 0;

    Player player = {
        (Vector2){(float)SCREEN_WIDTH / 2, (float)SCREEN_HEIGHT / 2},
        {0, 0},
        false,
        15,
        45,
    };
    Camera2D camera = { 0 };
    camera.target = player.position;
    camera.offset = (Vector2){ SCREEN_WIDTH/2.0f, SCREEN_HEIGHT/2.0f };
    camera.rotation = 0.0f;
    camera.zoom = 1.0f;

    EnvRect platforms[BOX_COUNT];
    platforms[0] = (EnvRect){ (Rectangle) { 50, 500, 700, 80 }, GRAY };
    platforms[1] = (EnvRect){ (Rectangle) { 800, 500, 700, 80 }, BLACK};
    platforms[2] = (EnvRect){ (Rectangle) { 325, 420, 600, 60 }, GREEN};
    platforms[3] = (EnvRect){ (Rectangle) { 200, 350, 400, 50 }, GRAY};


    Coin coins[COIN_COUNT];
    coins[0] = (Coin){
        (Vector2) { 850, 300 },
        false,
    };
    coins[1] = (Coin){ (Vector2) {1000, 400}, false };
    coins[2] = (Coin){ (Vector2) {200, 0}, false};


    start_program();

    while (!WindowShouldClose()) {
        
        update_player(&player);

        for (int i = 0; i < BOX_COUNT; i++)
        {
            if (CheckCollisionRecs((Rectangle) {player.position.x, player.position.y, player.width, player.height}, platforms[i].rec))
            {
                player_rectangle_collision(&player, platforms[i].rec);
            }
        }

        for (int i = 0; i < COIN_COUNT; i++)
        {
            if (CheckCollisionCircleRec(coins[i].position, COIN_SIZE, (Rectangle) {player.position.x, player.position.y, player.width, player.height}) && !coins[i].is_collected)
            {
                coins[i].is_collected = true;
                score++;
            }
        }

        camera.target = player.position;
        char str[5];
        sprintf(str, "%d", score);

        BeginDrawing();

            ClearBackground(RAYWHITE);
            DrawFPS(0,0);
            DrawText(str, 600, 100, 90, BLACK);
            
            BeginMode2D(camera);
            DrawRectangle(player.position.x, player.position.y, player.width, player.height, BLUE);

            for (int i = 0; i < BOX_COUNT; i++)
            {
            DrawRectangleRec(platforms[i].rec, platforms[i].color);
            }

            for (int i = 0; i < COIN_COUNT; i++)
            {
                if (!coins[i].is_collected)
                {
                    DrawCircleV(coins[i].position, COIN_SIZE, YELLOW);
                }
            }
            EndMode2D();
        EndDrawing();
    }

    CloseWindow();
}

void player_rectangle_collision(Player *player, Rectangle rec)
{
    if (player->position.y < rec.y)
    {
        player->can_jump = true;
        player->position.y = rec.y - player->height;
        player->velocity.y = 0;
        return;
    }

    if (player->position.y > rec.y)
    {
        player->position.y = rec.y + rec.height;
        player->velocity.y = 0;
        return;
    }

    if (player->position.x < rec.x)
    {
        player->position.x = rec.x + player->width;
        player->velocity.x = -1;
        return;
    }

    if (player->position.x > rec.x)
    {
        player->position.x = rec.x - player->width;
        player->velocity.x = 1;
        return;
    }
}

void update_player(Player *player)
{
    // Calculate player inputs
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

    // Check if player can/should jump
    if (player->can_jump)
    {
        if (IsKeyPressed(KEY_SPACE))
        {
            player->velocity.y -= 20;
            player->can_jump = false;
        }
    }
    
    // Apply Gravity
    player->velocity.y += (float)(G * 0.1);

    // Update Position
    player->position = Vector2Add(player->position, player->velocity);
}

void start_program(void)
{
    SetTargetFPS(60);
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Level");
}
