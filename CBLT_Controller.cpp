#include "CBLT_Controller.hpp"

namespace CBLT {
    Controller::Controller() {} // Field members' constructors are automatically called!
    
    Controller::~Controller() {}

    void Controller::HandleInsert() {
        // this
    }

    void Controller::Update() {
        this->keyboard.UpdateModifiers(); // Update modifiers
        
        for(auto c : this->cursorManager.activeCursors) {
            // HandleShorcuts();
            
            CBLT::CursorMode m = c.GetMode();
            
            switch(m) {
                case CBLT::CursorMode::CONSOLE:
                    // HandleConsole();

                    break;
                case CBLT::CursorMode::INSERT:
                    HandleInsert();
                
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
}