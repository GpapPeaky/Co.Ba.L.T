#pragma once

#include <string> // for std::string

#include <iostream> // for console output

namespace CBLT {
    namespace Utils {
        namespace Types {
            typedef unsigned int ui32; // 32 bit unsigned
            typedef const ui32 cui32; // Constant 32 bit unsigned
            typedef int i32; // 32 bit signed
            typedef const i32 ci32; // Constant 32 bit signed
    
            typedef unsigned char ui8; // 8 bit unsigned
            typedef const ui8 cui8; // Constant 8 bit unsigned
            typedef unsigned char i8; // 8 bit signed
            typedef const i8 ci8; // Constant 8 bit signed

            typedef char32_t c32; // 32 bit charater

            typedef bool b; // Boolean
    
            typedef float f32; // Floating point
        } // Types

        namespace Err {
            // Error logging function
            void Log(std::string msg);
        } // Err

    } // Utils
} // CBLT

// Utlity types definition, easier access
#define UT CBLT::Utils::Types

// Utility error logging, easier access
#define UE CBLT::Utils::Err