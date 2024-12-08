// From Camera
#include <raylib.h>
#include <raymath.h>

typedef struct Player {
    Vector2 position;
    Vector2 velocity;
    bool can_jump;
} Player;

typedef struct EnvRectangle {
    Rectangle rect;
} EnvRectangle;

const int G = 10;
void update_player(Player *player, Rectangle floor);

int main(void) {
    int screen_width = 800;
    int screen_height = 600;

    //int G = 10;

    Player player = {
        (Vector2){(float)screen_width / 2, (float)screen_height / 2},
        {0, 0},
        false,
    };
    Camera2D camera = { 0 };
    camera.target = player.position;
    camera.offset = (Vector2){ screen_width/2.0f, screen_height/2.0f };
    camera.rotation = 0.0f;
    camera.zoom = 1.0f;

    EnvRectangle floor = {{50, 500, 700, 80}};

    InitWindow(screen_width, screen_height, "movement");
    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        
        update_player(&player, floor.rect);

        if (CheckCollisionCircleRec(player.position, 40, floor.rect)) 
        {
            player.can_jump = true;
            player.position.y = 460;
            player.velocity.y = 0;
        }

        camera.target = player.position;

        BeginDrawing();

            ClearBackground(RAYWHITE);

            BeginMode2D(camera);
            DrawCircleV(player.position, 40, BLUE);
            DrawRectangleRec(floor.rect, GRAY);
            EndMode2D();
        EndDrawing();
    }

    CloseWindow();
}

void update_player(Player *player, Rectangle floor)
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
