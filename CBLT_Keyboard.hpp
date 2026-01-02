#pragma once

#include "raylib.h"

#include "CBLT_Util.hpp" // Types

namespace CBLT {

    // Keyboard class, keycode reading
    class Keyboard {
        typedef struct {
            UT::b ctrl;
            UT::b shift;
            UT::b alt;
        } Modifiers; // Keyboard modifier struct

        private:
            CBLT::Keyboard::Modifiers m; // Active modifiers for shortcuts
        public:
            // Constructor
            Keyboard(void);
    
            // Destructor
            ~Keyboard(void);

            // Keycode getter
            UT::i32 GetKey(void);

            // Update and return a modifier struct each frame if its key was pressed
            void UpdateModifiers(void);
    }; // Keyboard class
} // CBLT