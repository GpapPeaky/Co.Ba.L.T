#include "CBLT_Controller.hpp"

namespace CBLT {
    Controller::Controller(void) {} // Field members' constructors are automatically called!
    
    Controller::~Controller(void) {}

    UT::b Controller::HandleSpecialMovement(Cursor& cursor) {
        const UT::ui32 line = cursor.Line();
        const UT::ui32 col  = cursor.Col();

        if (keyboard.m.ctrl && (IsKeyPressed(KEY_RIGHT) || IsKeyPressedRepeat(KEY_RIGHT))) {
            cursor.SetToWordBoundary(file.GetCurrentLine(line), CursorDirection::RIGHT);

            return true;
        } else if (keyboard.m.ctrl && (IsKeyPressed(KEY_LEFT) || IsKeyPressedRepeat(KEY_LEFT))) {
            cursor.SetToWordBoundary(file.GetCurrentLine(line), CursorDirection::LEFT);
        
            return true;
        }

        return false;
    }

    void Controller::HandleMovement(Cursor& cursor) {
        // We need to check specifics AND THEN check for general key presses
        if (HandleSpecialMovement(cursor)) return; // Already handled movement, via LCtrl, skip applying any more movement
        
        const UT::ui32 line = cursor.Line();
        const UT::ui32 col  = cursor.Col();
        const UT::ui32 len  = file.GetLineLength(line);
    
        if (IsKeyPressed(KEY_LEFT) || IsKeyPressedRepeat(KEY_LEFT)) {
            if (col > 0) {
                cursor.Left();
            } else if (line > 1) {
                cursor.SetAt(
                    file.GetLineLength(line - 1),
                    line - 1
                );
            }
        } else if (IsKeyPressed(KEY_RIGHT) || IsKeyPressedRepeat(KEY_RIGHT)) {
            if (col < file.GetLineLength(line)) {
                cursor.Right();
            } else if (line + 1 < file.GetLineCount()) {
                cursor.SetAt(0, line + 1);
            }
        } else if (IsKeyPressed(KEY_UP) || IsKeyPressedRepeat(KEY_UP)) {
            if (line > 1 && col != len) {
                UT::ui32 newLine = line - 1;
                UT::ui32 newCol  = std::min(
                    col,
                    file.GetLineLength(newLine)
                );
          
                cursor.SetAt(newCol, newLine);
            } else if (line > 1) {
                UT::ui32 newLine = line - 1;
                UT::ui32 newCol  = std::max(
                    col,
                    file.GetLineLength(newLine)
                );
          
                cursor.SetAt(newCol, newLine);
            }
        } else if (IsKeyPressed(KEY_DOWN) || IsKeyPressedRepeat(KEY_DOWN)) {
            if (line + 1 < file.GetLineCount() && col != len) {
                UT::ui32 newLine = line + 1;
                UT::ui32 newCol  = std::min(
                    col,
                    file.GetLineLength(newLine)
                );
          
                cursor.SetAt(newCol, newLine);
            } else if (line + 1 < file.GetLineCount()) {
                UT::ui32 newLine = line + 1;
                UT::ui32 newCol  = std::max(
                    col,
                    file.GetLineLength(newLine)
                );
          
                cursor.SetAt(newCol, newLine);
            }
        }
    }

    void Controller::HandleSpecials(Cursor& cursor) {
        // Backspace
        if (IsKeyPressedRepeat(KEY_BACKSPACE) || IsKeyPressed(KEY_BACKSPACE)) {
            if (cursor.Col() > 0) {
                std::string& line = file.GetCurrentLine(cursor.Line());
                int col = cursor.Col();
                int tabSize = keyboard.tabSize;
        
                // If previous char is space → delete indentation block
                if (line[col - 1] == ' ') {
                    int deleteCount = 0;
                    int startCol = col;
        
                    // Walk left while:
                    // still spaces
                    // not past column 0
                    // not past a tab stop
                    while (startCol > 0 &&
                        line[startCol - 1] == ' ' &&
                        ((startCol - 1) % tabSize != 0)) {
                        startCol--;
                        deleteCount++;
                    }
        
                    // Always delete at least one space
                    if (deleteCount == 0) {
                        startCol--;
                        deleteCount = 1;
                    }
        
                    line.erase(startCol, deleteCount);
                    cursor.SetAt(startCol, cursor.Line());
                } else { // Normal character delete
                    line.erase(col - 1, 1);
                    cursor.Left();
                }
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

            file.SetDirt(true);
        }

        // Return
        if (IsKeyPressedRepeat(KEY_ENTER) || IsKeyPressed(KEY_ENTER)) {
            if (cursor.Col() == 0) {
                cursor.Down();

                file.CreateLine(cursor.Line() - 1); 
            } else {
                std::string fragment = file.SplitLine(cursor.Line(), cursor.Col());
                
                cursor.SetAt(0, cursor.Line() + 1);
                
                file.CreateLine(cursor.Line(), fragment);
            }

            file.SetDirt(true);
        }

        // Tab
        if (IsKeyPressedRepeat(KEY_TAB) || IsKeyPressed(KEY_TAB)) {
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

            file.SetDirt(true);
        }
    }

    void Controller::HandleInsert(Cursor& cursor, std::vector<char>& keyQueue) {        
        // Insert the queued input
        for (UT::c32 typed : keyQueue) {
            file.InsertChar(
                cursor.Col(),
                cursor.Line(),
                typed
            );

            cursor.Right();

            file.SetDirt(true); // Mark file as dirty
        }
    }

    UT::b Controller::HandleShorcuts(Cursor& cursor) {
        // LCTRL + LALT ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
        // ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
        // ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

        // Create cursors down
        if (keyboard.m.ctrl && keyboard.m.alt && (IsKeyPressed(KEY_DOWN) || IsKeyPressedRepeat(KEY_DOWN))) {
            cursorManager.RequestLead();

            return true;
        }

        // Create cursors up
        if (keyboard.m.ctrl && keyboard.m.alt && (IsKeyPressed(KEY_UP) || IsKeyPressedRepeat(KEY_UP))) {
            cursorManager.RequestTrail();

            return true;
        }

        // LCTRl ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
        // ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
        // ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

        // Delete current line
        if (keyboard.m.ctrl && (IsKeyPressed(KEY_X) || IsKeyPressedRepeat(KEY_X))) {
            file.DeleteLine(cursor.Line());
            file.SetDirt(true);

            return true;
        }

        // Copy current line
        if (keyboard.m.ctrl && (IsKeyPressed(KEY_D) || IsKeyPressedRepeat(KEY_D))) {
            file.CreateLine(cursor.Line(), file.GetCurrentLine(cursor.Line()));
            file.SetDirt(true);

            return true;
        }

        // Exit
        if (keyboard.m.ctrl && IsKeyPressed(KEY_E)) {
            exit(UDef::GRACEFUL_EXIT);
        }

        // Save and Exit
        if (keyboard.m.ctrl && IsKeyPressed(KEY_Q)) {
            file.Save();

            exit(UDef::GRACEFUL_EXIT);
        }

        // Console toggle
        if (keyboard.m.ctrl && IsKeyPressed(KEY_GRAVE)) {
            console.Toggle();

            return true;
        }

        // Save file contents
        if (keyboard.m.ctrl && IsKeyPressed(KEY_S)) {
            file.Save(); // Automatically cleans the "dirt"

            return true;
        }

        // Reset to primary cursor
        if (keyboard.m.ctrl && IsKeyPressed(KEY_R)) {
            cursorManager.RequestReset();

            return true;
        }

        return false;
    }

    UT::b Controller::HandleConsole(void) {
        // Directive file
        File& df = console.ConsoleDirective().DirectiveFile();
        
        // Console cursor
        Cursor& cc = console.ConsoleCursor();

        UT::i32 c = 0;                // Consumed instantly by the primary cursor 
        
        // Character recording
        while ((c = keyboard.GetKey()) > 0) {
            if (c >= 32 && c <= 126) { // Allow only ASCII
                df.InsertChar(cc.Col(), cc.Line(), c);

                cc.Right(); // Move cursor forward after inserting
            }
        }

        return false;
    }

    void Controller::Update(void) {
        keyboard.UpdateModifiers(); // Update modifiers
        console.Update();           // Update console

        // Console handling
        if (console.IsOpen()) {
            UT::b handleConsole = HandleConsole(); // Input

            if (handleConsole) return; // Input handled, return

            HandleMovement(console.ConsoleCursor());

            // Execute written directive
            if (IsKeyPressed(KEY_ENTER)) {
                console.Execute();
            }

            // Delete
            if (IsKeyPressed(KEY_BACKSPACE) || IsKeyPressedRepeat(KEY_BACKSPACE)) {
                if (console.ConsoleCursor().Col() > 0) {
                    console.ConsoleDirective().DirectiveFile().GetCurrentLine(1).erase(console.ConsoleCursor().Col() - 1, 1);

                    console.ConsoleCursor().Left();
                }
            }

            // Console toggle to get out
            if (keyboard.m.ctrl && IsKeyPressed(KEY_GRAVE)) {
                console.Toggle();
            }

            // Resize console
            if (keyboard.m.shift && (IsKeyPressed(KEY_LEFT))) {
                console.Move(+50.0f);                
            }
            
            if (keyboard.m.shift && (IsKeyPressed(KEY_RIGHT))) {
                console.Move(-50.0f);
            }


            return;
        }
        
        // Get pressed keys
        std::vector<char> keyQueue = GetKeyQueue();

        cursorManager.HandlePendingRequests(file);

        for(auto& c : cursorManager.activeCursors) {
            // HandleShorcuts();
            CBLT::CursorMode m = c.GetMode();
            
            // Handling booleans
            UT::b handledShort;
            
            switch(m) {
                case CBLT::CursorMode::INSERT:
                    handledShort = HandleShorcuts(c);
                    HandleSpecials(c);

                    // Shortcuts include ctrl + arrow key presses so we need to omit movement
                    if (!handledShort) HandleMovement(c);
                    // Shortcuts include letters so it makes sense that we need to omit any leftover I/O's
                    // so they won't spill over to the insert function
                    if (!handledShort) HandleInsert(c, keyQueue);     // Shortcut was handled, do not insert 

                    ClampCursor(c); // Safety check
                
                    // if(keyboard.CtrlActive()) {
                        // DrawText("ctrl", 200, 200, 20, BLACK);
                    // }else if(keyboard.ShiftActive()) {
                        // DrawText("shift", 200, 200, 20, BLACK);
                    // }else if(keyboard.AltActive()) {
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

    const Console& Controller::GetConsole(void) const {
        return console;
    }

    Console& Controller::GetConsole(void) {
        return console;
    }

    const CBLT::CursorManager& Controller::GetCursorManager(void) const {
        return cursorManager;
    }

    CBLT::CursorManager& Controller::GetCursorManager(void) {
        return cursorManager;
    }

    void Controller::ClampCursor(Cursor& c) {
        c.SetAt(
            std::min(c.Col(), static_cast<UT::ui32>(file.GetLineLength(c.Line()))),
            std::min(c.Line(), static_cast<UT::ui32>(file.GetLineCount() - 1))
        );
    }

    std::vector<char> Controller::GetKeyQueue(void) {
        std::vector<char> typedChars; // Typed char, in order to save the queue to apply to the other cursors
        UT::i32 c = 0;                // Consumed instantly by the primary cursor 

        // Character recording
        while ((c = keyboard.GetKey()) > 0) {
            if (c >= 32 && c <= 126) { // Allow only ASCII
                typedChars.push_back(static_cast<char>(c)); // Save typed characters
            }
        }


        return typedChars;
    }

} // CBLT
