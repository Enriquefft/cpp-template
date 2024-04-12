#include <raylib-cpp.hpp>
#include <spdlog/spdlog.h>

int main() {
  int screenWidth = 800;
  int screenHeight = 450;

  raylib::Window window(screenWidth, screenHeight, "raylib-cpp - basic window");
  raylib::Texture logo(ASSETS_PATH "dogo.png");

  SetTargetFPS(60);

  spdlog::info("Started drawing");

  while (!window.ShouldClose()) {
    BeginDrawing();

    window.ClearBackground(RAYWHITE);

    DrawText("Congrats! You created your first window!", 190, 200, 20,
             LIGHTGRAY);

    // Object methods.
    logo.Draw(screenWidth / 2 - logo.GetWidth() / 2,
              screenHeight / 2 - logo.GetHeight() / 2);

    EndDrawing();
  }
  spdlog::info("Finished drawing");

  return 0;
}
