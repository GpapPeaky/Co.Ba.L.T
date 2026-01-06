#include "CBLT_TopBar.hpp"

void CBLT::UI::Draw(UT::ui32 col, UT::ui32 line, UT::ui32 lineCount, UT::b dirty) {
    const UT::ui32 topBarFontSize = 30;
    const UT::ui32 topBarSeperatorY = 17;
    const UT::ui32 topBarInfoVerticalShift = 5;
    const UT::ui32 topBarSecondColumnX = 150;
    const UT::ui32 topBarCWDFilePathSeperatorX = 300;

    // String to notify the user if the file is dirty (modified/unsaved) or clean (saved)    
    std::string dirtyFile;
    Color dirtyColour;
    if (dirty) {
        dirtyFile = std::string("dirty");
        dirtyColour = {255, 255, 0, 255};
    } else {
        dirtyFile = std::string("clean");
        dirtyColour = {0, 255, 255, 255};
    }
    
    // Seperators

    // Draw top bar seperator
    DrawLine(
        0,
        CBLT::UI::TOP_BAR_HEIGHT + topBarSeperatorY,
        static_cast<UT::f32>(GetScreenWidth()),
        CBLT::UI::TOP_BAR_HEIGHT + topBarSeperatorY, 
        Color{0, 255, 0, 255}
    );

    // Draw vertical seperator for cwd and filepath
    DrawLine(
        topBarCWDFilePathSeperatorX,
        0,
        topBarCWDFilePathSeperatorX,
        CBLT::UI::TOP_BAR_HEIGHT + topBarSeperatorY,
        Color{0, 255, 0, 255}
    );

    // Draw cursor column and line data
    DrawTextEx(
        gFont.f,
        (std::string("c: ") + std::to_string(col)).c_str(),
        {0, 0},
        topBarFontSize,
        0.0f,
        Color{0, 0, 255, 255}
    );
    
    DrawTextEx(
        gFont.f,
        (std::string("l: ") + std::to_string(line)).c_str(),
        {0, static_cast<UT::f32>(topBarFontSize) + topBarInfoVerticalShift},
        topBarFontSize,
        0.0f,
        Color{0, 0, 255, 255}
    );
    
    DrawTextEx(
        gFont.f,
        (std::string("lc: ") + std::to_string(lineCount)).c_str(),
        {topBarSecondColumnX, 0},
        topBarFontSize,
        0.0f,
        Color{0, 0, 255, 255}
    );

    DrawTextEx(
        gFont.f,
        (std::string("d: ") + dirtyFile).c_str(),
        {topBarSecondColumnX, static_cast<UT::f32>(topBarFontSize) + topBarInfoVerticalShift},
        topBarFontSize,
        0.0f,
        dirtyColour
    );
}
