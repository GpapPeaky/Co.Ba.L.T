#include "CBLT_Console.hpp"

namespace CBLT {
    Console::Console(void) {
        width = 200;
        toggled = false;
        cursor.AddCursorAt(0, 1); // Same logic as in the CBLT_Controller.hpp/cpp
    }
    
    Console::~Console(void) {}

    void Console::Toggle(void) {
        toggled = !toggled;
    }

    UT::b Console::IsOpen(void) {
        return toggled;
    }

    DirectiveResult Console::Execute(void) {
        DirectiveResult dr = { "", ConsoleMessage::NONE };

        if (directive.GetString().empty()) return dr; // Nothing to show

        if (directive.GetString()[0] == ':') { // Directive mode
            
        } else { // File switch mode
            // TODO
        }

        return dr;
    }    

    void Console::Draw(std::string cwd) {
        const UT::ui32 directiveFontSize = 20;
        const UT::ui32 directiveBottomMargin = CBLT::DirectiveMargins::directiveMarginFromConsoleY + 5; // 5 + 5 see CBLT_Directive.hpp
        
        // Background rectangle
        DrawRectangle(
            GetScreenWidth() - width,
            0,
            width + 1,
            GetScreenHeight(),
            Color{0, 255, 0, 255}
        );

        // Foreground rectangle
        DrawRectangle(
            GetScreenWidth() - width + 1,
            0,
            width,
            GetScreenHeight(),
            Color{0, 0, 0, 255}
        );

        // Directive/CWD contents seperator
        DrawLine(
            GetScreenWidth() - width,
            directiveFontSize + directiveBottomMargin,
            GetScreenWidth(),
            directiveFontSize + directiveBottomMargin,
            Color{0, 255, 0, 255}
        );

        // Draw directive contents
        directive.Draw(
            GetScreenWidth() - width,
            0
        );
    }

} // CBLT