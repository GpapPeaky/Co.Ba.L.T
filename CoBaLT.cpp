#include "includes.hpp"


// TODO: BASIC INPUT RECORDER, MOVE TO THE CONTROLLER'S HANDLE FUNCTIONS
// namespace CBLT {

//     // Reads a single character from the keyboard and appends it to 'input'
//     void RecordCharacterInput(std::string &input) {
//         int key = GetCharPressed(); // Raylib function: returns a Unicode character, 0 if none

//         if (key > 0) {
//             // Append the character to the string
//             input += static_cast<char>(key);

//             // Important: clear the pressed character from the input queue
//             // so it doesn't get repeated
//             // (Raylib documentation: call this once per key read)
//             // Actually, Raylib automatically clears it, but safe practice:
//             // Flush the input queue manually if needed
//         }

//         // Handle backspace
//         if (IsKeyPressed(KEY_BACKSPACE) && !input.empty()) {
//             input.pop_back();
//         }
//     }

// }

int main() {
    CBLT::Win::Init();

    std::string input;

    // CBLT::Controller ctrl; // Automatically calls the constructor
    // CBLT::Controller ctrl(fields...); // Automatically calls the constructor
    CBLT::Controller ctrl; // Program controller

    while(!WindowShouldClose()) {
        BeginDrawing();

            ClearBackground(WHITE);

            // CBLT::RecordCharacterInput(input);
    
            DrawText(input.c_str(), 0, 0, 20, BLACK);
    
        EndDrawing();
    }

    CBLT::Win::Destroy();

    return 0;
}
