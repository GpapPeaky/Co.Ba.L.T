#include "CBLT_TopBar.hpp"

void CBLT::UI::Draw(UT::ui32 col, UT::ui32 line, UT::ui32 lineCount, UT::b dirty) {
    // Draw top bar seperator
    DrawLine(
        0,
        CBLT::UI::TOP_BAR_HEIGHT + CBLT::gFont.size,
        static_cast<UT::f32>(GetScreenWidth()),
        CBLT::UI::TOP_BAR_HEIGHT + CBLT::gFont.size, 
        Color{0, 255, 0, 255}
    );

    // Draw cursor column and line data
    DrawTextEx(
        gFont.f,
        (std::string("c: ") + std::to_string(col)).c_str(),
        {0, 0},
        CBLT::gFont.size,
        0.0f,
        Color{0, 0, 255, 255}
    );
    
    DrawTextEx(
        gFont.f,
        (std::string("l: ") + std::to_string(line)).c_str(),
        {0, static_cast<UT::f32>(CBLT::gFont.size)},
        CBLT::gFont.size,
        0.0f,
        Color{0, 0, 255, 255}
    );
    
    DrawTextEx(
        gFont.f,
        (std::string("lc: ") + std::to_string(lineCount)).c_str(),
        {0, 2 * static_cast<UT::f32>(CBLT::gFont.size)},
        CBLT::gFont.size,
        0.0f,
        Color{0, 0, 255, 255}
    );

    DrawTextEx(
        gFont.f,
        (std::string("d: ") + std::to_string(dirty)).c_str(),
        {0, 3 * static_cast<UT::f32>(CBLT::gFont.size)},
        CBLT::gFont.size,
        0.0f,
        Color{0, 0, 255, 255}
    );
}
