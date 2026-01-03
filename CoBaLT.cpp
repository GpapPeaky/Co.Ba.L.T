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

            // No heap alloc, no copies, no runtime cost
            CBLT::CursorManager& cm = ctrl.GetCursorManager();
            CBLT::Cursor& c = cm.Primary();

            // Draw open file
            ctrl.GetFile().Draw();
            cm.DrawCursors(ctrl.GetFile());
    
            CBLT::UI::Draw(c.Col(), c.Line(), ctrl.GetFile().GetLineCount(), ctrl.GetFile().Dirt());

            DrawFPS(950, 0);
        EndDrawing();
    }

    CBLT::Win::Destroy();

    return 0;
}
