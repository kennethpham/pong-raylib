#include <stdlib.h>
#include <time.h>

#include "raylib.h"

#define SCREEN_WIDTH (800)
#define SCREEN_HEIGHT (600)

#define WINDOW_TITLE "Pong"

struct Ball {
  float x, y;
  float speedX, speedY;
  float radius;

  void Reset() {
    x = GetScreenWidth() / 2.0f;
    y = GetScreenHeight() / 2.0f;
    radius = 5.0f;
    // Set a random direction in x and y coordinates
    speedX = 300.0f * (GetRandomValue(0, 1) ? 1 : -1);
    speedY = 300.0f * (GetRandomValue(0, 1) ? 1 : -1);
  }

  void Draw() { DrawCircle((int)x, (int)y, radius, WHITE); }
};

struct Paddle {
  float x, y;
  float speed;
  float width, height;

  Rectangle GetRect() {
    return Rectangle{x - width / 2, y - height / 2, 10, 100};
  }

  void Draw() { DrawRectangleRec(GetRect(), WHITE); }
};

int main(void) {
  InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, WINDOW_TITLE);
  SetWindowState(FLAG_VSYNC_HINT);
  SetRandomSeed(time(NULL));

  Ball ball;
  ball.Reset();

  Paddle leftPaddle;
  leftPaddle.x = 50;
  leftPaddle.y = GetScreenHeight() / 2;
  leftPaddle.width = 10;
  leftPaddle.height = 100;
  leftPaddle.speed = 500;

  Paddle rightPaddle;
  rightPaddle.x = GetScreenWidth() - 50;
  rightPaddle.y = GetScreenHeight() / 2;
  rightPaddle.width = 10;
  rightPaddle.height = 100;
  rightPaddle.speed = 500;

  const char *winnerText = nullptr;

  while (!WindowShouldClose()) {

    ball.x += ball.speedX * GetFrameTime();
    ball.y += ball.speedY * GetFrameTime();

    if (ball.y < 0) {
      ball.y = 0;
      ball.speedY *= -1;
    }
    if (ball.y > GetScreenHeight()) {
      ball.y = GetScreenHeight();
      ball.speedY *= -1;
    }

    if (IsKeyDown(KEY_W)) {
      leftPaddle.y -= leftPaddle.speed * GetFrameTime();
    }
    if (IsKeyDown(KEY_S)) {
      leftPaddle.y += leftPaddle.speed * GetFrameTime();
    }
    if (IsKeyDown(KEY_UP)) {
      rightPaddle.y -= rightPaddle.speed * GetFrameTime();
    }
    if (IsKeyDown(KEY_DOWN)) {
      rightPaddle.y += rightPaddle.speed * GetFrameTime();
    }

    if (leftPaddle.y < 50) {
      leftPaddle.y = 50;
    }
    if (leftPaddle.y > GetScreenHeight() - 50) {
      leftPaddle.y = GetScreenHeight() - 50;
    }
    if (rightPaddle.y < 50) {
      rightPaddle.y = 50;
    }
    if (rightPaddle.y > GetScreenHeight() - 50) {
      rightPaddle.y = GetScreenHeight() - 50;
    }

    if (CheckCollisionCircleRec(Vector2{ball.x, ball.y}, ball.radius,
                                leftPaddle.GetRect())) {
      if (ball.speedX < 0) {
        ball.speedX *= -1.1;
      }
    }
    if (CheckCollisionCircleRec(Vector2{ball.x, ball.y}, ball.radius,
                                rightPaddle.GetRect())) {
      if (ball.speedX > 0) {
        ball.speedX *= -1.1;
      }
    }

    if (ball.x < 0) {
      winnerText = "Right Player Wins!";
    }

    if (ball.x > GetScreenWidth()) {
      winnerText = "Left Player Wins!";
    }

    if (winnerText && IsKeyPressed(KEY_SPACE)) {
      ball.Reset();
      winnerText = nullptr;
      leftPaddle.x = 50;
      leftPaddle.y = GetScreenHeight() / 2;
      rightPaddle.x = GetScreenWidth() - 50;
      rightPaddle.y = GetScreenHeight() / 2;
    }

    BeginDrawing();
    ClearBackground(BLACK);

    ball.Draw();
    leftPaddle.Draw();
    rightPaddle.Draw();

    if (winnerText) {
      int textWidth = MeasureText(winnerText, 60);
      DrawText(winnerText, GetScreenWidth() / 2 - textWidth / 2,
               GetScreenHeight() / 2 - 30, 60, YELLOW);
    }

    DrawFPS(10, 10);
    EndDrawing();
  }

  CloseWindow();

  return 0;
}
