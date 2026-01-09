#include "CBLT_Win.hpp"

namespace CBLT {
    std::string Version = std::string("CoBaLT v2.0.0");
    
    namespace Win {

        void Init(void) {
            Image icon = LoadImage("assets/icon/muse.png");
            
            const int screenWidth = 1536;
            const int screenHeight = 1024;
            
            InitWindow(screenWidth, screenHeight, Version.c_str());
            
            SetWindowIcon(icon);
            
            // Free image
            UnloadImage(icon);
            
            SetTargetFPS(180);

            SetExitKey(KEY_NULL); // Disable automatic ESC exit
        }

        void Destroy(void) {
            CloseWindow();
        }

    } // Win
} // CBLT
