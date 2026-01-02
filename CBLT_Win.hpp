#pragma once

#include "raylib.h"

#include <string>

namespace CBLT {
    extern std::string Version;
    
    namespace Win {

        //  Window initializing
        void Init(void);

        //  Window destroy
        void Destroy(void);

    } // Win
} // CBLT
