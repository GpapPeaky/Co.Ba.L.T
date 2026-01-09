#include "CBLT_Console.hpp"

namespace CBLT {
    Console::Console(void) {
        width = 450;
        toggled = false;
        cursor.AddCursorAt(0, 1); // Same logic as in the CBLT_Controller.hpp/cpp
    
        directive = Directive();

        interpolator = Interpolator();
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
    
            UT::f32 maxWidth = GetScreenWidth() * 0.5f;
            width = std::clamp(newWidth, 20.0f, maxWidth);
        }
    }

    void Console::Move(UT::f32 offset) {
        if (interpolator.IsActive())
            return;
    
        UT::f32 maxWidth = GetScreenWidth() * ConsoleWidth::WIDTH_MAX_RATIO;
        UT::f32 target = std::clamp(width + offset, ConsoleWidth::WIDTH_MIN, maxWidth);
    
        interpolator.Start(width, 0.0f, target, 0.0f, 0.12f);
    }


    Cursor& Console::ConsoleCursor(void) {
        return cursor.Primary();
    }

    UT::f32 Console::Width(void) const {
        return width;
    }
} // CBLT