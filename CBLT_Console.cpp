#include "CBLT_Console.hpp"

namespace CBLT {
    Console::Console(void) {
        width = 450;
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

    DirectiveResult Console::Execute(void) {
        DirectiveResult dr = { "", ConsoleMessage::NONE };
        
        // if (directive.DirectiveFile.empty()) return dr; // Nothing to show
        // 
        // if (directive.DirectiveFile[0] == ':') { // Directive mode
        // 
        // } else { // File switch mode
            // TODO
        // }
        
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
        if (interpolator.IsActive()) {
            auto [newWidth, _] = interpolator.Update();
            width = static_cast<UT::f32>(newWidth);
        }
    }

    void Console::Move(UT::f32 offset) {
        UT::f32 newTarget = width + offset;
        interpolator.Start(
            width, 0,                       // fromX, fromY (Y not used)
            newTarget, 0,                   // targetX, targetY
            0.12f                           // speed, tweak as needed
        );
    }

    Cursor& Console::ConsoleCursor(void) {
        return cursor.Primary();
    }

    UT::f32 Console::Width(void) const {
        return width;
    }
} // CBLT