#include "CBLT_Win.hpp"

namespace CBLT {
    std::string Version = std::string("CoBaLT v2.0.0");
    
    namespace Win {

        void Init(void) {
            const int screenWidth = 1536;
            const int screenHeight = 1024;

            InitWindow(screenWidth, screenHeight, Version.c_str());

            SetTargetFPS(180);
        }

        void Destroy(void) {
            CloseWindow();
        }

    } // Win
} // CBLT
