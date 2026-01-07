#pragma once

#include <string>

#include "CBLT_Directive.hpp"

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

    // Console class for executing directives
    class Console {
        private:
            Directive directive;        // Directive to execute
            CursorManager cursor;       // Cursor position inside the directive
            UT::b toggled;              // Console is on or off
            UT::ui32 width;             // Console mutable width
        public:
            // Constructor
            Console();
            
            // Destructor
            ~Console();

            // Toggle the console on or off
            void Toggle(void);

            // Check if the console is on or off
            UT::b IsOpen(void);

            // Execute the current directive
            DirectiveResult Execute(void);

            // Draw console window, and CWD contents
            void Draw(std::string cwd);
    }; // Console class
} // CBLT

