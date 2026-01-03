#include "CBLT_Controller.hpp"

namespace CBLT {
    Controller::Controller(void) {} // Field members' constructors are automatically called!
    
    Controller::~Controller(void) {}

    void Controller::HandleMovement(Cursor& cursor) {
        const UT::ui32 line = cursor.Line();
        const UT::ui32 col  = cursor.Col();
    
        if (IsKeyPressed(KEY_LEFT)) {
            if (col > 0) {
                cursor.Left();
            } else if (line > 1) {
                cursor.SetAt(
                    file.GetLineLength(line - 1),
                    line - 1
                );
            }
        } else if (IsKeyPressed(KEY_RIGHT)) {
            if (col < file.GetLineLength(line)) {
                cursor.Right();
            } else if (line + 1 < file.GetLineCount()) {
                cursor.SetAt(0, line + 1);
            }
        } else if (IsKeyPressed(KEY_UP)) {
            if (line > 1) {
                UT::ui32 newLine = line - 1;
                UT::ui32 newCol  = std::min(
                    col,
                    file.GetLineLength(newLine)
                );
          
                cursor.SetAt(newCol, newLine);
            }
        } else if (IsKeyPressed(KEY_DOWN)) {
            if (line + 1 < file.GetLineCount()) {
                UT::ui32 newLine = line + 1;
                UT::ui32 newCol  = std::min(
                    col,
                    file.GetLineLength(newLine)
                );
          
                cursor.SetAt(newCol, newLine);
            }
        }
    }
    void Controller::HandleSpecials(Cursor& cursor) {
        // Backspace
        if (IsKeyPressed(KEY_BACKSPACE)) { // TODO: Auto delete spaces matching to a multiple of tab size
            if (cursor.Col() > 0) {
                std::string& line = file.GetCurrentLine(cursor.Line());

                line.erase(cursor.Col() - 1, 1); // Erase 1 from the previous column
                
                cursor.Left();
            } else if (cursor.Col() == 0 && cursor.Line() > 0) {
                std::string& previousLine = file.GetCurrentLine(cursor.Line() - 1);
                std::string& line = file.GetCurrentLine(cursor.Line());
                
                // Move one line up
                cursor.SetAt(previousLine.length(), cursor.Line() - 1);
                
                if (line.empty()){
                    file.DeleteLine(cursor.Line() + 1);
                } else {
                    // Move the rest of the line to the previous one
                    file.PushBackLineFragment(cursor.Line() + 1, cursor.Line());

                    // THEN delete the line
                    file.DeleteLine(cursor.Line() + 1);
                }
            }
        }

        // Return
        if (IsKeyPressed(KEY_ENTER)) {
            if (cursor.Col() == 0) {
                cursor.Down();

                file.CreateLine(cursor.Line() - 1); 
            } else {
                std::string fragment = file.SplitLine(cursor.Line(), cursor.Col());
                
                cursor.SetAt(0, cursor.Line() + 1);
                
                file.CreateLine(cursor.Line(), fragment);
            }
        }

        // Tab
        if (IsKeyPressed(KEY_TAB)) {
            UT::ui8 remainingSpace;
            
            if (cursor.Col() % keyboard.tabSize == 0) {
                remainingSpace = keyboard.tabSize;
            } else if (cursor.Col() > keyboard.tabSize) {
                remainingSpace = cursor.Col() % keyboard.tabSize;
            } else {
                remainingSpace = keyboard.tabSize - cursor.Col();
            }

            for (UT::ui8 i = 0 ; i < remainingSpace ; i++) {
                file.InsertChar(
                    cursor.Col(),
                    cursor.Line(),
                    ' '
                );

                cursor.Right();
            }
        }
    }

    void Controller::HandleInsert(Cursor& cursor) {
        UT::i32 c = 0;

        // Character recording
        while ((c = this->keyboard.GetKey()) > 0) {
            if (c >= 32 && c <= 126) { // Allow only ASCII
                this->file.InsertChar(
                    cursor.Col(),
                    cursor.Line(),
                    c
                );

                cursor.Right();
            }
        }
    }

    void Controller::Update(void) {
        this->keyboard.UpdateModifiers(); // Update modifiers
        
        for(auto& c : this->cursorManager.activeCursors) {
            // HandleShorcuts();
            
            CBLT::CursorMode m = c.GetMode();
            
            switch(m) {
                case CBLT::CursorMode::CONSOLE:
                    // HandleConsole();

                    break;
                case CBLT::CursorMode::INSERT:
                    HandleMovement(c);
                    HandleSpecials(c);

                    HandleInsert(c);

                    ClampCursor(c); // Safety check
                
                    // if(this->keyboard.CtrlActive()) {
                        // DrawText("ctrl", 200, 200, 20, BLACK);
                    // }else if(this->keyboard.ShiftActive()) {
                        // DrawText("shift", 200, 200, 20, BLACK);
                    // }else if(this->keyboard.AltActive()) {
                        // DrawText("alt", 200, 200, 20, BLACK);
                    // }else{
                        // DrawText("nomod", 200, 200, 20, BLACK);
                    // }

                    break;
                case CBLT::CursorMode::SELECT:
                    // HandleSelect();
                
                    break;
                default:
                    UE::Log("Unknown cursor mode");

                    break;
            }
        }
    }

    const File& Controller::GetFile(void) const {
        return file;
    }

    File& Controller::GetFile(void) {
        return file;
    }

    const CBLT::CursorManager& Controller::GetCursorManager(void) const {
        return this->cursorManager;
    }

    CBLT::CursorManager& Controller::GetCursorManager(void) {
        return this->cursorManager;
    }

    void Controller::ClampCursor(Cursor& c) {
        c.SetAt(
            std::min(c.Col(), static_cast<UT::ui32>(file.GetLineLength(c.Line()))),
            std::min(c.Line(), static_cast<UT::ui32>(file.GetLineCount() - 1))
        );
    }

}
