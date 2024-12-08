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

float rec_top(Rectangle rect);

int main(void) {
  int screen_width = 800;
  int screen_height = 600;

  int G = 10;

  Player player = {
      (Vector2){(float)screen_width / 2, (float)screen_height / 2},
      {0, 0},
      false,
  };

  EnvRectangle floor = {{50, 500, 700, 80}};

  InitWindow(screen_width, screen_height, "movement");
  SetTargetFPS(60);

  while (!WindowShouldClose()) {

    if (IsKeyDown(KEY_LEFT)) {
      player.velocity.x = -8;
    } else if (IsKeyDown(KEY_RIGHT)) {
      player.velocity.x = 8;
    } else {
      player.velocity.x = 0;
    }

    if (player.can_jump) {
      if (IsKeyPressed(KEY_SPACE)) {
        player.velocity.y -= 20;
        player.can_jump = false;
      }
    }

    player.velocity.y += (float)(G * 0.1);

    player.position = Vector2Add(player.position, player.velocity);

    if (CheckCollisionCircleRec(player.position, 40, floor.rect)) {
      player.can_jump = true;
      player.position.y = 460;
      player.velocity.y = 0;
    }

    BeginDrawing();

    ClearBackground(RAYWHITE);

    DrawCircleV(player.position, 40, BLUE);
    DrawRectangleRec(floor.rect, GRAY);
    EndDrawing();
  }

  CloseWindow();
}

float rec_top(Rectangle rect) { return (float)rect.y - rect.height; }
