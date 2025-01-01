// From Camera
#include <raylib.h>
#include <raymath.h>
#include <stdio.h>
#include <math.h>

typedef struct Player {
    Vector2 position;
    Vector2 velocity;
    bool can_jump;
    const float width;
    const float height;
    Vector2 center;
    Rectangle collision_box;
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
void player_gravity(Player *player);
void player_input(Player *player);
void player_add_velocity(Player *player);
Vector2 midpoint_rec(Rectangle rec);

int main(void) {

    int score = 0;

    Player player = {
        (Vector2){(float)SCREEN_WIDTH / 2, (float)SCREEN_HEIGHT / 2},
        {0, 0},
        false,
        15,
        45,
        {0, 0},
        {player.position.x, player.position.y, player.width, player.height},
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
        
        player_gravity(&player);
        player_input(&player);
        player_add_velocity(&player);

        if (IsKeyDown(KEY_V))
        {
            player.position = (Vector2) {0, 0};
            player.velocity = (Vector2) {0, 0};
        }

        for (int i = 0; i < BOX_COUNT; i++)
        {
            if (CheckCollisionRecs(player.collision_box, platforms[i].rec))
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
        update_player(&player);

        camera.target = (Vector2) { 300, 300 };
        //camera.target = player.position;
        char str[5];
        sprintf(str, "%d", score);

        BeginDrawing();

            ClearBackground(RAYWHITE);
            DrawFPS(0,0);
            DrawText(str, 600, 100, 90, BLACK);
            
            BeginMode2D(camera);
            //DrawRectangle(player.position.x, player.position.y, player.width, player.height, BLUE);

            for (int i = 0; i < BOX_COUNT; i++)
            {
            DrawRectangleRec(platforms[i].rec, platforms[i].color);

            // Debug
            DrawCircle(platforms[i].rec.x, platforms[i].rec.y, 3, MAGENTA);
            DrawCircleV(midpoint_rec(platforms[i].rec), 10, MAGENTA);
            }

            for (int i = 0; i < COIN_COUNT; i++)
            {
                if (!coins[i].is_collected)
                {
                    DrawCircleV(coins[i].position, COIN_SIZE, YELLOW);
                }
            }

            // Debug
            DrawCircle(player.position.x, player.position.y, 3, MAGENTA);
            DrawRectangleRec(player.collision_box, RED);


            EndMode2D();
        EndDrawing();
    }

    CloseWindow();
}

void player_rectangle_collision(Player *player, Rectangle rec)
{

    Vector2 midpoint = midpoint_rec(rec);
    Vector2 offset = { player->center.x - midpoint.x, player->center.y - midpoint.y };

    BeginDrawing();
        DrawCircleV(offset, 5, GREEN);
    EndDrawing();

    if (fabs(offset.x) > fabs(offset.y))
    {
        if (offset.x > 0)
        {
            player->position.x -= offset.x - player->width;
        }
        else if (offset.x < 0)
        {
            player->position.x += offset.x - player->width;
        }
    }
    else if (fabs(offset.y) > fabs(offset.x))
    {
        if (offset.y > 0)
        {
            player->position.y -= offset.y;
        }
        else if (offset.y < 0)
        {
            player->position.y += offset.y;
        }
    }
    else
    {
        player->position.x += offset.x;
        player->position.y += offset.y;
    }
    
    update_player(player);
}

Vector2 midpoint_rec(Rectangle rec)
{
    return (Vector2) { rec.x + (rec.width/2), rec.y + (rec.height/2) };
    return (Vector2) { 100, 100 };
}

void update_player(Player *player)
{
    // Update Position
    player->center = (Vector2) { player->position.x + player->width/2, player->position.y + player->height/2 };
    player->collision_box = (Rectangle) {player->position.x - player->width/2, player->position.y - player->height/2, player->width, player->height};
    //player->position.x = 100;
    //player->position.y = 100;
}

void player_add_velocity(Player *player)
{
    player->position = Vector2Add(player->position, player->velocity);
}

void player_gravity(Player *player)
{
    player->velocity.y += (float)(G * 0.1);
}

void player_input(Player *player)
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
  
}

void start_program(void)
{
    SetTargetFPS(60);
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Level");
}
