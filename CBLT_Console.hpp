#pragma once

#include <string>
#include <algorithm>

#include "CBLT_Directive.hpp"
#include "CBLT_Interpolator.hpp"

namespace CBLT {
    // Different type of messages the console can display
    enum class ConsoleMessage {
        NONE = -1,                  // No message to display
        INFO,                       // The console will display info
        ERROR,                      // The console will display an error
        GUIDE                       // The console will display a guide
    };

    // Results a directive can return
    typedef struct DirectiveResult {
        std::string message;            // Message to return
        ConsoleMessage messageType;     // The type of message
    } DirectiveResult;

    namespace ConsoleWidth {
        const UT::f32 WIDTH_MIN = 20.0f;            // Width min
        const UT::f32 WIDTH_MAX_RATIO = 0.5f;       // Max width ratio relative to screen
    } // Console

    // Console class for executing directives
    class Console {
        private:
            Directive directive;        // Directive to execute
            UT::b toggled;              // Console is on or off
            UT::f32 width;             // Console mutable width
            CursorManager cursor;       // Cursor position inside the directive
        public:
            // Constructor
            Console();
            
            // Destructor
            ~Console();

            // Toggle the console on or off
            void Toggle(void);

            // Console general updates
            void Update(void);

            // Check if the console is on or off
            UT::b IsOpen(void) const ;

            // Execute the current directive
            DirectiveResult Execute(File& f);

            // Draw console window, and CWD contents
            void Draw(std::string cwd);

            // Console directive
            Directive& ConsoleDirective(void);

            // Resize console window
            void Move(UT::f32 offset);

            // Get the console cursor
            Cursor& ConsoleCursor(void);

            // Console width
            UT::f32 Width(void) const ;
    }; // Console class
} // CBLT

