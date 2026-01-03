#include "CBLT_Controller.hpp"

namespace CBLT {
    Controller::Controller(void) {} // Field members' constructors are automatically called!
    
    Controller::~Controller(void) {}

    void Controller::HandleMovement(Cursor& cursor) {
        if (IsKeyPressed(KEY_LEFT)) {
            cursor.Left();
            ClampCursor(cursor);
        } else if (IsKeyPressed(KEY_RIGHT)) {
            cursor.Right();
        } else if (IsKeyPressed(KEY_UP)) {
            cursor.Up();
        } else if (IsKeyPressed(KEY_DOWN)) {
            cursor.Down();
        }
    }

    void Controller::HandleSpecials(Cursor& cursor) {
        if (IsKeyPressed(KEY_BACKSPACE)) {
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
