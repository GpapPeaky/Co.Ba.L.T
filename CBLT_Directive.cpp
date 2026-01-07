#include "CBLT_Directive.hpp"

namespace CBLT {
    Directive::Directive() { str =  std::string(""); }

    Directive::~Directive() {}

    void Directive::Clear(void) {
        str.clear();
    }

    void Directive::Draw(UT::ui32 x, UT::ui32 y) {
        DrawText(
            str.c_str(),
            x + DirectiveMargins::directiveMarginFromConsoleX,
            y + DirectiveMargins::directiveMarginFromConsoleY,
            UI::directiveFontSize,                                         // Make it remain as is for now
            Color{255, 255, 255, 255}            
        );
    }

    const std::string& Directive::GetString(void) const {
        return str;
    }

    std::string& Directive::GetString(void) {
        return str;
    }
} // CBLT 
