#include "includes.hpp"

int main() {
    CBLT::Win::Init();

    CBLT::gFont.Load("assets/font/GoogleSansCode-Regular.ttf");
    CBLT::gFont.size = 17;

    // CBLT::Controller ctrl; // Automatically calls the constructor
    // CBLT::Controller ctrl(fields...); // Automatically calls the constructor
    CBLT::Controller ctrl; // Program controller
    ctrl.GetFile().Load(".tests/test.c");

    while(!WindowShouldClose()) {
        BeginDrawing();

            ClearBackground(BLACK);

            ctrl.Update();

            // Draw open file
            ctrl.GetFile().Draw();
            ctrl.GetCursorManager().DrawCursors(ctrl.GetFile());
    
            DrawFPS(950, 0);

            DrawText(
                (std::string("Col: ") + std::to_string(ctrl.GetCursorManager().activeCursors[0].Col())).c_str(),
                600,
                700,
                CBLT::gFont.size,
                BLUE);

            DrawText(
                (std::string("Line: ") + std::to_string(ctrl.GetCursorManager().activeCursors[0].Line())).c_str(),
                750,
                700,
                CBLT::gFont.size,
                BLUE);
    
        EndDrawing();
    }

    CBLT::Win::Destroy();

    return 0;
}
