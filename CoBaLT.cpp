#include "includes.hpp"

int main() {
    CBLT::Win::Init();

    while(!WindowShouldClose()) {
        BeginDrawing();

            ClearBackground(WHITE);
    
            DrawText("Hello, From Laptop", 0, 0, 20, BLACK);
    
        EndDrawing();
    }

    CBLT::Win::Destroy();

    return 0;
}
