#include "CBLT_Cursor.hpp"

namespace CBLT {
    Cursor::Cursor(UT::ui32 col, UT::ui32 ln) :
        column(col),
        line(ln),
        selectColumn(col),
        selectLine(ln),
        charWidth(MeasureText("A", CBLT::gFont.size)), // Measure once
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

    void Cursor::Draw(const std::string& lineText) {
        int x = GetCursorX(lineText);
        int y = line * gFont.size;
        DrawRectangle(
            x + CBLT::FileMargins::Text::LEFT_FROM_FILE_LINES_UI + CBLT::FileMargins::Lines::LEFT_FROM_WINDOW_Y + CBLT::FileMargins::UI::LEFT_FROM_FILE_LINES,
            y + CBLT::FileMargins::Text::BELLOW_FROM_TOP_BAR,
            3,
            gFont.size,
            RED
        );
    }

    UT::i32 Cursor::GetCursorX(const std::string& lineText){
        UT::f32 scale = (UT::f32)gFont.size / gFont.f.baseSize;
        UT::i32 width = 0;
        auto cps = CBLT::gFont.Utf8ToCodepoints(lineText);

        for(size_t i = 0; i < column && i < cps.size(); i++) {
            UT::i32 cp = cps[i];
            UT::i32 glyphIndex = -1;

            for(UT::i32 g = 0; g < gFont.f.glyphCount; g++) {
                if(gFont.f.glyphs[g].value == cp){
                    glyphIndex = g;
                    break;
                }
            }

            if(glyphIndex >= 0) {
                width += gFont.f.glyphs[glyphIndex].advanceX;
            } else {
                width += gFont.size / 2;
            }
        }
        return (UT::i32)(width * scale);
    }
    
    CursorManager::CursorManager() {
        activeCursors.emplace_back(0, 1); // Initialize one cursor at 0,1
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

    void CursorManager::DrawCursors(CBLT::File& openFile) {
        for(auto& c : activeCursors) {
            const std::string& lineText = openFile.GetCurrentLine(c.Line());
            c.Draw(lineText);
        }
    }
} // CBLT