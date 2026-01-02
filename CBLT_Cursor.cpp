#include "CBLT_Cursor.hpp"

namespace CBLT {
    Cursor::Cursor(UT::ui32 col, UT::ui32 ln) :
        column(col),
        line(ln),
        selectColumn(col),
        selectLine(ln),
        m(CursorMode::INSERT) // Default
    {}

   Cursor::~Cursor(void) {}

    UT::ui32 Cursor::Col(void) const {
        return this->column;
    }

    UT::ui32 Cursor::Line(void) const {
        return this->line;
    }

    void Cursor::SetAt(UT::ui32 col, UT::ui32 line) {
        this->column = col;
        this->line = line;
    }

    CursorMode Cursor::GetMode(void) const {
        return this->m;
    }

    void Cursor::SetMode(CursorMode m) {
        this->m = m;
    }

    void Cursor::Up(void) {
        if(this->line > 0) {
            this->line -= 1;
        }
    }

    void Cursor::Down(void) {
        this->line += 1;
    }

    void Cursor::Left(void) {
        if(this->column > 0) {
            this->column -= 1;
        }
    }

    void Cursor::Right(void) {
        this->column += 1;
    }

    CursorManager::CursorManager() {
        activeCursors.emplace_back(0, 0); // Initialize one cursor at 0,0
    }

    CursorManager::~CursorManager() {}

    void CursorManager::AddCursorAt(UT::ui32 col, UT::ui32 line) {
        this->activeCursors.emplace_back(col, line); // Constructs directly inside the vector
    }

    void CursorManager::RemoveCursorAt(UT::ui32 col, UT::ui32 line) {
        for(size_t i = 0 ; i < activeCursors.size() ; i++) {
            if(activeCursors.at(i).Col() == col && activeCursors.at(i).Line() == line) {
                activeCursors.erase(activeCursors.begin() + i); // Remove the matching cursor 
            }
        }
    }
} // CBLT