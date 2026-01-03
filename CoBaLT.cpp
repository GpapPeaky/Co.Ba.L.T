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
            CBLT::CursorManager& cm = ctrl.GetCursorManager();

            // Draw open file
            ctrl.GetFile().Draw();
            cm.DrawCursors(ctrl.GetFile());
    
            CBLT::UI::Draw(cm.Primary().Col(), cm.Primary().Line());

            DrawFPS(950, 0);
        EndDrawing();
    }

    CBLT::Win::Destroy();

    return 0;
}
