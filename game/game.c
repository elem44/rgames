#include <raylib.h>
#include <raymath.h>

#define DEBUG 1

typedef struct Player {
    Vector2 pos;
    Color color;
    bool can_jump;
    float width;
    float height;
    Vector2 velocity;
} Player;

typedef struct EnvBlockCollision
{
    Rectangle top;
    Rectangle bottom;
    Rectangle left;
    Rectangle right;
    float width;
    float height;
} EnvBlockCollision;

typedef struct BrickBlock
{
    Vector2 pos;
    EnvBlockCollision collision;
    float width;
    float height;
    Color color;
} BrickBlock;

typedef struct Enemy
{
    Vector2 pos;
    Rectangle hitbox;
} Enemy;

typedef struct Coin
{
    Vector2 pos;
    bool is_collected;
} Coin;


const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;

const int TILE_SIZE = 64;
const int ENEMY_SIZE = 14;

const int LEVEL_BLOCKS = 5;


EnvBlockCollision create_EnvBlockCollision(Vector2 position, float width, float height);
void draw_EnvBlockCollision(EnvBlockCollision blocks);
void check_player_collisions(Player *player, EnvBlockCollision block);
void handle_player_input(Player *player);
Enemy create_Enemy (Vector2 position);
void update_enemy_hitbox(Enemy *enemy);
void update_enemy_position(Enemy *enemy);
void update_enemy (Enemy *enemy);
void enemy_hit(Player *player, Enemy *enemy);
void player_apply_velocity(Player *player);
void handle_player(Player *player);
void draw_coin(Coin coin);
void draw_score(int score);

int main(void)
{
    

    Player player = {
        (Vector2){0, -28},
        RED,
        false,
        28,
        14,
        (Vector2){0, 0},
    };


    Vector2 start_block_position = (Vector2) {-100, 250};
    EnvBlockCollision start_block = (EnvBlockCollision) create_EnvBlockCollision(start_block_position, 9000, 500);

    BrickBlock bricks[LEVEL_BLOCKS];
    bricks[0].pos = (Vector2) {500, -150};
    bricks[0].color = YELLOW;
    bricks[1].pos = (Vector2) {600, -150};
    bricks[1].color = BROWN;
    bricks[2].pos = (Vector2) {664, -150};
    bricks[2].color = YELLOW;
    bricks[3].pos = (Vector2) {728, -150};
    bricks[3].color = BROWN;
    bricks[4].pos = (Vector2) {664, -250};
    bricks[4].color = YELLOW;
    for (int i = 0; i < LEVEL_BLOCKS; i++)
    {
        bricks[i].collision = (EnvBlockCollision) create_EnvBlockCollision(bricks[i].pos, TILE_SIZE, TILE_SIZE);
    }

    Coin coin = {0};
    coin.pos = (Vector2) {-100, -100};
    coin.is_collected = false;

    Enemy enemy = create_Enemy((Vector2) {TILE_SIZE * 7, 0});


    Camera2D camera = { 0 };
    camera.target = player.pos;
    camera.offset = (Vector2){ SCREEN_WIDTH/2.0f, SCREEN_HEIGHT/2.0f };
    camera.rotation = 0.0f;
    camera.zoom = 1.0f;

    int score = 0;

    SetTargetFPS(60);
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Game");

    while (!WindowShouldClose())
    {

        handle_player_input(&player);
        handle_player(&player); 


        check_player_collisions(&player, start_block);
        for (int i = 0; i < LEVEL_BLOCKS; i++)
        {
            check_player_collisions(&player, bricks[i].collision);
        }
        
        update_enemy(&enemy);

        enemy_hit(&player, &enemy);

        camera.target = player.pos;

        if (CheckCollisionCircleRec(coin.pos, 20, (Rectangle) {player.pos.x, player.pos.y, player.width, player.height}) && !coin.is_collected)
        {
            coin.is_collected = true;
            score++;
        }

        BeginDrawing();
            
            ClearBackground(RAYWHITE);

            
            draw_score(score);
            
            BeginMode2D(camera);

                DrawRectangle(player.pos.x, player.pos.y, player.height, player.width, player.color);
                DrawCircleV(player.pos, 3, MAGENTA);
                //DrawRectangleRec((Rectangle) {0, 0, 50, 100}, GREEN);
                DrawCircleV((Vector2) {0, 0}, 3, MAGENTA);
                draw_EnvBlockCollision(start_block);

                DrawRectangleRec(enemy.hitbox, MAGENTA);

                for (int i = 0; i < LEVEL_BLOCKS; i++)
                {
                    DrawRectangleV(Vector2Subtract(bricks[i].pos, Vector2Scale((Vector2) {TILE_SIZE, TILE_SIZE}, 0.5)), (Vector2) {TILE_SIZE, TILE_SIZE}, bricks[i].color);
                }
                

                draw_coin(coin);

                // Debug Brick Collision
                /*
                for (int i = 0; i < LEVEL_BLOCKS; i++)
                {
                    draw_EnvBlockCollision(bricks[i].collision);
                }
                */
            EndMode2D();
        EndDrawing();
    }
}

void check_player_collisions(Player *player, EnvBlockCollision block)
{
    Rectangle player_rec = (Rectangle) {player->pos.x, player->pos.y, player->height, player->width};
    if (CheckCollisionRecs(player_rec, block.top) && player_rec.y < block.top.y - player->height)
    {
        player->pos.y = block.top.y - (player->height * 2);
        player->velocity.y = 0;
    }
    if (CheckCollisionRecs(player_rec, block.bottom) && player_rec.y > (block.bottom.y + (block.height / 4) - 10))
    {
        player->pos.y = block.bottom.y + block.height / 4;
    }
    if (CheckCollisionRecs(player_rec, block.left) && player->pos.y > block.left.y - player->height && player->pos.y < (block.left.y + block.height))
    {
        player->pos.x = block.left.x - player->width/2;
    }
    if (CheckCollisionRecs(player_rec, block.right) && player_rec.y > block.right.y - player->height && player->pos.y < (block.right.y + block.height))
    {
        player->pos.x = block.right.x + block.width / 4;
    }

    return;
}

EnvBlockCollision create_EnvBlockCollision(Vector2 position, float width, float height)
{
    float vertical_interval = height / 4;
    float horizontal_interval = width / 4;
    Rectangle top = {position.x - width/2, position.y - height / 2, width, height / 4};
    Rectangle bottom = {position.x - width/2, position.y + vertical_interval, width, vertical_interval};
    Rectangle left = {position.x - width / 2, position.y - height / 2, horizontal_interval, height};
    Rectangle right = {position.x + horizontal_interval, position.y - height / 2, horizontal_interval, height};
    return (EnvBlockCollision) {top, bottom, left, right, width, height};
}

void draw_EnvBlockCollision(EnvBlockCollision blocks)
{
    DrawRectangleRec(blocks.top, GREEN);
    DrawRectangleRec(blocks.bottom, BLUE);
    DrawRectangleRec(blocks.left, RED);
    DrawRectangleRec(blocks.right, YELLOW);
    return;
}

void handle_player_input(Player *player)
{
    if (DEBUG)
    {
        if (IsKeyDown(KEY_UP)) player->pos.y -= 8;
        if (IsKeyDown(KEY_DOWN)) player->pos.y += 8;
        if (IsKeyDown(KEY_LEFT)) player->pos.x -= 8;
        if (IsKeyDown(KEY_RIGHT)) player->pos.x += 8;
    }
    else
    {
        if (IsKeyDown(KEY_LEFT)) player->pos.x -= 8;
        if (IsKeyDown(KEY_RIGHT)) player->pos.x += 8;
        if (IsKeyDown(KEY_SPACE)) player->velocity.y -= 5;
        
        return;
    }
}

void handle_player(Player *player)
{
    if (DEBUG)
    {
        return;
    }
    player->velocity.y += 3;
    if (player->velocity.y > 15)
    {
        player->velocity.y = 15;
    }
    player_apply_velocity(player);
}

void player_apply_velocity(Player *player)
{
    player->pos = Vector2Add(player->pos, player->velocity);
}

Enemy create_Enemy (Vector2 position)
{
    Enemy new_enemy = {};
    new_enemy.pos = position;
    new_enemy.hitbox = (Rectangle) {position.x - ENEMY_SIZE * 3, position.y - ENEMY_SIZE * 3, ENEMY_SIZE * 3, ENEMY_SIZE * 3};
    return new_enemy;
}

void update_enemy (Enemy *enemy)
{
    update_enemy_position(enemy);
    update_enemy_hitbox(enemy);
}

void update_enemy_position(Enemy *enemy)
{
    enemy->pos.x -= 2;
}

void update_enemy_hitbox(Enemy *enemy)
{
    enemy->hitbox = (Rectangle) {enemy->pos.x - ENEMY_SIZE * 3, enemy->pos.y - ENEMY_SIZE * 3, ENEMY_SIZE * 3, ENEMY_SIZE * 3};
}

void enemy_hit(Player *player, Enemy *enemy)
{
    Rectangle player_rec = (Rectangle) {player->pos.x, player->pos.y, player->height, player->width};
    if (CheckCollisionRecs(player_rec, enemy->hitbox))
    {
        player->pos = (Vector2) {0, 0};
    }
}

void draw_coin(Coin coin)
{
    if (!coin.is_collected)
    {
        DrawCircleV(coin.pos, 20, YELLOW);
    }
}

void draw_score(int score)
{
    char score_c[1];
    score_c[0] = score + '0';

    DrawText(score_c, 0, 0, 96, BLACK);
}
