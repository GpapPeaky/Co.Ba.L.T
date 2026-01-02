#include "CBLT_Controller.hpp"

namespace CBLT {
    Controller::Controller() {} // Field members' constructors are automatically called!
    
    Controller::~Controller() {}

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
                // HandleInsert();
                
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