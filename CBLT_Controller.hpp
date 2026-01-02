#pragma once

#include "CBLT_Cursor.hpp"    // Cursor manager
#include "CBLT_Keyboard.hpp"  // Keyboard for input

namespace CBLT {
    // Basic controller and input handling
    class Controller {
        private:
            CBLT::CursorManager cursorManager;     // Cursor manager
            CBLT::Keyboard keyboard;               // Keyboard

            // TODO: Open files should be here

            // Handle input at insert mode
            void HandleInsert(void);

            // Handle input at console mode
            void HandleConsole(void);

            // Handle input at selection mode
            void HandleSelect(void);

            // Handle shorcuts, based on the active keyboard modifiers and cursor mode
            void HandleShorcuts(void);

        public:
            // Controller update, most functions are called here
            void Update(void);

            // Constructor
            Controller();

            // Destructor
            ~Controller();
            
    }; // Controller class
} // CBLT
