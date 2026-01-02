#pragma once

#include "raylib.h"

#include <string>

namespace CBLT {
    extern std::string Version;
    
    namespace Win {

        /**
         * @brief Window initializing
         */
        void Init(void);

        /**
         * @brief Window destroy
         */
        void Destroy(void);

    } // Win
} // CBLT
