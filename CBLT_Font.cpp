#include "CBLT_Font.hpp"

namespace CBLT {
    EditorFont::EditorFont(void) :
        size(20),
        f()
    {}

    EditorFont::~EditorFont(void) {}

    void EditorFont::Load(std::string fontName) {
        this->f = LoadFont(fontName.c_str());
    }

    std::vector<UT::i32> EditorFont::Utf8ToCodepoints(const std::string& str) {
        std::vector<UT::i32> cps;
        for(size_t i = 0; i < str.size(); ) {
            unsigned char c = str[i];
            UT::i32 cp = 0;
            UT::i32 len = 0;
    
            if(c < 0x80) { cp = c; len = 1; }
            else if((c & 0xE0) == 0xC0) { cp = c & 0x1F; len = 2; }
            else if((c & 0xF0) == 0xE0) { cp = c & 0x0F; len = 3; }
            else if((c & 0xF8) == 0xF0) { cp = c & 0x07; len = 4; }
    
            for(UT::i32 j = 1 ; j < len ; j++) {
                cp = (cp << 6) | (str[i + j] & 0x3F);
            }
    
            cps.push_back(cp);
            i += len;
        }
        return cps;
    }

    EditorFont gFont;
} // CBLT
