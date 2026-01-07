#pragma once

#include "CBLT_Util.hpp" // Types
#include "CBLT_Font.hpp" // Font size
#include "CBLT_File.hpp" // Get current text line

#include <vector>        // for std::vector<> ...

#include "raylib.h"      // for MeasureText() ...

#include "CBLT_TopBar.hpp" // for vertical margins

namespace CBLT {

    // Cursor mode
    enum class CursorMode {
        CONSOLE,                // Console mode
        INSERT,                 // Insertion mode
        SELECT                  // Selection mode
    }; 

    // Cursor direction class
    enum class CursorDirection {
        LEFT,                   // Move left
        RIGHT,                  // Move right
        UP,                     // Move up
        DOWN                    // Move down
    };

    // Characters are defined as one of 3 groups, inWord, space, symbol
    enum class CharClass {
        INWORD,             // Character can be found inside a word <'a' 'b'>
        WHITESPACE,         // Character is a whitespace char <' '>
        SYMBOL              // Character is a symbol <',' '.' '/'>
    };

    // Basic cursor object for both file and console
    class Cursor {

        private:
            UT::ui32 column;        // Current column the cursor is at
            UT::ui32 line;          // Current line the cursor is at
            UT::ui32 selectColumn;  // Initial cursor column at select mode entry
            UT::ui32 selectLine;    // Initial cursor line at select mode entry
            UT::ui32 charWidth;     // Monospaced font support ONLY!
            CursorMode m;           // Current cursor mode
        
        public:
            // Constructor
            Cursor(UT::ui32 col, UT::ui32 line);

            // Destructor
            ~Cursor(void);

            // Get the current column
            UT::ui32 Col(void) const;

            // Get the current line
            UT::ui32 Line(void) const;

            // Set cursor position at a specific column and line
            void SetAt(UT::ui32 col, UT::ui32 line);

            // Get the cursor mode
            CursorMode GetMode(void) const;

            // Set cursor at a specific mode
            void SetMode(CursorMode m);

            // Move cursor one line up, line -= 1
            void Up(void);

            // Move cursor one line down, line += 1
            void Down(void);

            // Move cursor one column left, column -= 1
            void Left(void);

            // Move cursor one column right, column += 1
            void Right(void);

            // Draw cursor
            void Draw(const std::string& lineText);

            // Get cursor x in pixels 
            UT::ui32 GetCursorX(const std::string& lineText);

            // Classify character class
            const CharClass Classify(UT::cui8 c) const;

            // Get the distance to a character in the left or right of the cursor and set the cursor there
            void SetToWordBoundary(const std::string& lineText, const CursorDirection dir);
    }; // Cursor class

    class CursorManager {
        public:
            std::vector<CBLT::Cursor> activeCursors; // Multiline cursor management
            
            // Constructor
            CursorManager(void);

            // Destructor
            ~CursorManager(void);

            // Add a cursor at a specific column and line
            void AddCursorAt(UT::ui32 col, UT::ui32 line);

            // Remove a cursor at a specific column and line
            void RemoveCursorAt(UT::ui32 col, UT::ui32 line);

            // Draw all active cursors
            void DrawCursors(CBLT::File& openFile);

            // Get primary cursor mutable access
            Cursor& Primary();

            // Get primary cursor read only
            const Cursor Primary() const;
    }; // Cursor manager class
} // CBLT

