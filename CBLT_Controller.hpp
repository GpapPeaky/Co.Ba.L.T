#pragma once

#include "CBLT_Cursor.hpp"    // Cursor manager
#include "CBLT_Keyboard.hpp"  // Keyboard for input
#include "CBLT_File.hpp"      // Document/File type

#include <cmath>              // min() ..

namespace CBLT {
    // Basic controller and input handling
    class Controller {
        private:
            CBLT::CursorManager cursorManager;     // Cursor manager
            CBLT::Keyboard keyboard;               // Keyboard
            CBLT::File file;                       // Current open document

            // TODO: Finish the handling

            // Handle basic cursor movement
            void HandleMovement(Cursor& cursor);

            // Handle presses like TAB, RETURN and BACKSPACE
            void HandleSpecials(Cursor& cursor);

            // Handle input at insert mode
            void HandleInsert(Cursor& cursor);

            // Handle input at console mode
            void HandleConsole(void);

            // Handle input at selection mode
            void HandleSelect(void);

            // Handle shorcuts, based on the active keyboard modifiers and cursor mode
            void HandleShorcuts(Cursor& cursor);

        public:
            // Controller update, most functions are called here
            void Update(void);

            // Get the current open file, read only
            const File& GetFile(void) const;

            // Get the current open file, mutable access
            File& GetFile(void);

            // Get the cursor manager, read only
            const CBLT::CursorManager& GetCursorManager(void) const;

            // Get the cursor manager, mutable access
            CBLT::CursorManager& GetCursorManager(void);

            // Check line and file lengths to limit cursor movement
            void ClampCursor(Cursor& cursor);

            // Constructor
            Controller(void);

            // Destructor
            ~Controller(void);
            
    }; // Controller class
} // CBLT
