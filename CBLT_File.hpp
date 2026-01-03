#pragma once

#include <vector>               // for std::vector<> ...
#include <string>               // for std::string ...

#include <fstream>              // File stream

#include "raylib.h"             // for rendering

#include "CBLT_Util.hpp"        // for types

#include "CBLT_Font.hpp"        // for Font global

namespace CBLT {
    // Basic document/file class
    class File {
        private:
            std::vector<std::string> lines;  // Most elemental storage class of a file/document
            std::string path;                // File path, will include its name
            UT::b dirty;                     // File's original contents have been changed and have not been saved
        public:
            // Constructor
            File(void);

            // Load file from name
            UT::b Load(const std::string& fpath);
            
            // Save file
            UT::b Save(void);

            // Clear file?
            UT::b Clear(void);

            // Insert character at a specific line and column
            void InsertChar(UT::ui32 col, UT::ui32 line, UT::i32 c);

            // Get current file path
            const std::string& GetPath(void) const;

            // Draw the document
            void Draw(void) const;

            // Get line count
            size_t GetLineCount(void) const;

            // Get column count in line
            size_t GetLineLength(size_t line) const;

            // Get line at line index, read only
            const std::string& GetCurrentLine(UT::i32 line) const;

            // Get line at line index, mutable access
            std::string& GetCurrentLine(UT::i32 line);
    }; // File class
} // CBLT
