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

        // Draw a transparent rectangle, to show where the cursor is
        DrawRectangle(
            0,
            y + CBLT::UI::TOP_BAR_HEIGHT,
            GetScreenWidth(),
            gFont.size,
            Color{255, 255, 255, 45}
        );

        DrawRectangle(
            x + CBLT::FileMargins::Text::LEFT_FROM_FILE_LINES_UI + CBLT::FileMargins::Lines::LEFT_FROM_WINDOW_Y + CBLT::FileMargins::UI::LEFT_FROM_FILE_LINES,
            y + CBLT::UI::TOP_BAR_HEIGHT,
            2,
            gFont.size,
            RED
        );
    }

    UT::ui32 Cursor::GetCursorX(const std::string& lineText){
        UT::f32 scale = (UT::f32)gFont.size / gFont.f.baseSize;
        UT::ui32 width = 0;
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

            if (glyphIndex >= 0) {
                width += gFont.f.glyphs[glyphIndex].advanceX;
            } else {
                width += gFont.size / 2;
            }
        }
        
        return (UT::ui32)(width * scale);
    }

    const CharClass Cursor::Classify(UT::cui8 c) const {
        if (c == ' ' || c == '\n') {
            return CharClass::WHITESPACE;
        }

        if (std::isalnum(c) || c == '_') {
            return CharClass::INWORD;
        }

        return CharClass::SYMBOL;
    }
    
    void Cursor::SetToWordBoundary(const std::string& lineText, CursorDirection dir) {
        UT::i32 col  = static_cast<UT::i32>(Col());
        UT::i32 line = static_cast<UT::i32>(Line());
        UT::i32 len  = static_cast<UT::i32>(lineText.size());
    
        if (dir == CursorDirection::RIGHT) {
            if (col >= len) {
                SetAt(len, line + 1); // UNSAFE
                return;
            }

            while (col < len && Classify(lineText[col]) == CharClass::WHITESPACE) col++;

            if (col >= len) {
                SetAt(len, line);
                return;
            }

            CharClass cc = Classify(lineText[col]);

            // Skip current class
            while (col < len && cc == Classify(lineText[col])) col++;

            SetAt(col, line);
            return;
        }
    
        if (dir == CursorDirection::LEFT) {
            if (line <= 1) { // Safety check
                SetAt(0, 1);
                return;
            }

            if (col <= 0) {
                SetAt(0, line - 1);
                return;
            }
    
            int i = col - 1;
    
            while (i >= 0 && Classify(lineText[i]) == CharClass::WHITESPACE) i--;
    
            if (i < 0) {
                SetAt(0, line);
                return;
            }
    
            CharClass cc = Classify(lineText[i]);
            while (i >= 0 && Classify(lineText[i]) == cc) i--;
    
            SetAt(i + 1, line);
            return;
        }
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

    Cursor& CursorManager::Primary() {
        return activeCursors[0];
    }

    const Cursor CursorManager::Primary() const {
        return activeCursors[0];
    }
} // CBLT