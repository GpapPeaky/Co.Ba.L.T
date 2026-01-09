#include "CBLT_Console.hpp"

namespace CBLT {
    Console::Console(void) {
        width = 450;
        toggled = false;
        cursor.AddCursorAt(0, 1); // Same logic as in the CBLT_Controller.hpp/cpp

        openFactor = 0.0f;
    
        directive = Directive();

        widthInterpolator = Interpolator();
        toggleInterpolator = Interpolator();
    }
    
    Console::~Console(void) {}

    void Console::Toggle(void) {
        toggled = !toggled;
    
        if (toggleInterpolator.IsActive())
            toggleInterpolator.Stop();
    
        if (toggled) {
            // Open
            toggleInterpolator.Start(
                openFactor, 0.0f,
                1.0f, 0.0f,
                0.01f
            );
        } else {
            // Close
            toggleInterpolator.Start(
                openFactor, 0.0f,
                0.0f, 0.0f,
                0.01f
            );
        }
    }

    UT::b Console::IsOpen(void) const {
        return openFactor > 0.01f;
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
        
        UT::f32 visibleWidth = width * openFactor;

        // Background rectangle
        DrawRectangle(
            GetScreenWidth() - visibleWidth,
            0,
            visibleWidth + 1,
            GetScreenHeight(),
            Color{0, 255, 0, 255}
        );

        // Foreground rectangle
        DrawRectangle(
            GetScreenWidth() - visibleWidth + 1,
            0,
            visibleWidth,
            GetScreenHeight(),
            Color{0, 0, 0, 255}
        );

        // Directive/CWD contents seperator
        DrawLine(
            GetScreenWidth() - visibleWidth,
            directiveFontSize + directiveBottomMargin,
            GetScreenWidth(),
            directiveFontSize + directiveBottomMargin,
            Color{0, 255, 0, 255}
        );

        // Draw directive contents
        directive.Draw(
            GetScreenWidth() - visibleWidth,
            0
        );

        // Draw console cursor
        Cursor& cc = cursor.Primary();
        const std::string& lineText = directive.DirectiveFile().GetCurrentLine(cc.Line());
        
        // Compute cursor position inside console
        UT::i32 cursorX = cc.GetCursorX(lineText, directiveFontSize);
        UT::i32 cursorY = DirectiveMargins::directiveMarginFromConsoleY;
        
        // Offset for the console's left edge + margins
        cursorX += GetScreenWidth() - visibleWidth + DirectiveMargins::directiveMarginFromConsoleX;
        
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
        if (toggleInterpolator.IsActive()) {
            auto [x, _] = toggleInterpolator.Update();
            openFactor = x;
        }

        if (widthInterpolator.IsActive()) {
            auto [newWidth, _] = widthInterpolator.Update();
    
            UT::f32 maxWidth = GetScreenWidth() * 0.5f;
            width = std::clamp(newWidth, 20.0f, maxWidth);
        }
    }

    void Console::Move(UT::f32 offset) {
        if (widthInterpolator.IsActive())
            return;
    
        UT::f32 maxWidth = GetScreenWidth() * ConsoleWidth::WIDTH_MAX_RATIO;
        UT::f32 target = std::clamp(width + offset, ConsoleWidth::WIDTH_MIN, maxWidth);
    
        widthInterpolator.Start(width, 0.0f, target, 0.0f, 0.075f);
    }


    Cursor& Console::ConsoleCursor(void) {
        return cursor.Primary();
    }

    UT::f32 Console::Width(void) const {
        return width;
    }
} // CBLT