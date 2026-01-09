#include "CBLT_Console.hpp"

namespace CBLT {
    Console::Console(void) {
        width = 500;
        toggled = false;
        cursor.AddCursorAt(0, 1); // Same logic as in the CBLT_Controller.hpp/cpp

        directive = Directive();
    }
    
    Console::~Console(void) {}

    void Console::Toggle(void) {
        toggled = !toggled;
    }

    UT::b Console::IsOpen(void) const {
        return toggled;
    }

    DirectiveResult Console::Execute(File& f) {
        DirectiveResult dr = { "", ConsoleMessage::NONE }; // Write here for any messages that need to be displayed, info, error, guide or none if all's well

        std::string directiveLine = directive.DirectiveFile().GetCurrentLine(1);
        
        if (directiveLine.empty()) return dr; // Nothing to show
        
        if (directiveLine[0] == ':') { // Directive mode
            std::string drctv = U::TrimSemiColon(directiveLine); // Trim

            // Match the remainder

            // Exi
            if (drctv == "e") {
                exit(EXIT_SUCCESS);
            }

            // Save and exit
            if (drctv == "q") {
                f.Save();

                exit(EXIT_SUCCESS);
            }

            if (drctv == "w") {
                f.Save();
            }

        } else { // File switch mode
            
        }
        
        directive.Clear();

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

        // Draw console cursor
        Cursor& cc = cursor.Primary();
        const std::string& lineText = directive.DirectiveFile().GetCurrentLine(cc.Line());
        
        // Compute cursor position inside console
        UT::i32 cursorX = cc.GetCursorX(lineText, directiveFontSize);
        UT::i32 cursorY = DirectiveMargins::directiveMarginFromConsoleY;
        
        // Offset for the console's left edge + margins
        cursorX += GetScreenWidth() - width + DirectiveMargins::directiveMarginFromConsoleX;
        
        // Draw cursor rectangle
        DrawRectangle(
            cursorX,
            cursorY,
            2,
            UI::directiveFontSize,
            Color{255, 0, 0, 255}
        );
    }

    Directive& Console::ConsoleDirective(void) {
        return directive;
    }

    void Console::Update() {
        
    }

    void Console::Move(UT::f32 offset) {
        UT::f32 maxWidth = GetScreenWidth() * ConsoleWidth::WIDTH_MAX_RATIO;
        width = std::clamp(width + offset, ConsoleWidth::WIDTH_MIN, maxWidth);
    }


    Cursor& Console::ConsoleCursor(void) {
        return cursor.Primary();
    }

    UT::f32 Console::Width(void) const {
        return width;
    }
} // CBLT