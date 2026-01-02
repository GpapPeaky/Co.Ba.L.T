#include "includes.hpp"

int main() {
    CBLT::Win::Init();

    // CBLT::Controller ctrl; // Automatically calls the constructor
    // CBLT::Controller ctrl(fields...); // Automatically calls the constructor
    CBLT::Controller ctrl; // Program controller

    while(!WindowShouldClose()) {
        BeginDrawing();

            ClearBackground(WHITE);

            // CBLT::RecordCharacterInput(input);
            ctrl.Update();
    
            DrawFPS(100, 100);
    
        EndDrawing();
    }

    CBLT::Win::Destroy();

    return 0;
}
