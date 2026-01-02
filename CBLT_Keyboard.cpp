#include "CBLT_Keyboard.hpp"

namespace CBLT {

        Keyboard::Keyboard(void):
            m { false, false, false } // Modifiers set to false
        {}
    
        Keyboard::~Keyboard(void) {}

        UT::i32 Keyboard::GetKey(void) {
            return GetCharPressed(); // Via raylib
        }

        void Keyboard::UpdateModifiers(void) {
            this->m.ctrl = IsKeyDown(KEY_LEFT_CONTROL);
            this->m.shift = IsKeyDown(KEY_LEFT_SHIFT);
            this->m.alt = IsKeyDown(KEY_LEFT_ALT);
        }
} // CBLT
