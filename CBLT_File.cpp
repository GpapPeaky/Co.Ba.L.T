#include "CBLT_File.hpp"

namespace CBLT {
    File::File() {
        lines.emplace_back("");
    }
        
    UT::b File::Load(const std::string& fpath) {
        std::ifstream file(fpath);
        if(!file.is_open())
            return false;
    
        Clear(); // Clear the previous open file
        path = fpath;
    
        std::string line;

        while(std::getline(file, line)) {
            lines.push_back(line);
        }
    
        if(lines.empty())
            lines.emplace_back("");
    
        dirty = false;

        return true;
    }
            
    UT::b File::Save(void) {
        if(path.empty())
            return false;
    
        std::ofstream file(path);
        if(!file.is_open())
            return false;
    
        for(const auto& line : lines) {
            file << line << '\n';
        }
    
        dirty = false;

        return true;
    }

    UT::b File::Clear(void) {
        lines.clear();
        lines.emplace_back("");

        return true;
    }

    void File::InsertChar(UT::ui32 col, UT::ui32 line, UT::i32 c) {
        if (line >= lines.size()) return;
    
        auto& ln = lines[line];
    
        if (col > ln.size()) col = ln.size();
    
        ln.insert(ln.begin() + col, static_cast<char>(c));
    }
    
    const std::string& File::GetPath(void) const {
        return path;
    }

    void File::Draw(void) const {
        float lineHeight = gFont.size;
    
        for(size_t i = 0 ; i < lines.size() ; i++) {
            Vector2 pos = {
                0.0f,
                i * lineHeight
            };
    
            DrawTextEx(
                gFont.f,
                lines[i].c_str(),
                pos,
                gFont.size,
                0.0f,
                BLACK
            );
        }
    }

    size_t File::GetLineCount() const {
        return lines.size();
    }

    size_t File::GetLineLength(size_t line) const {
        if (line >= lines.size()) return 0;
        return lines[line].size();
    }

    const std::string& File::GetCurrentLine(UT::i32 line) const {
        return lines.at(line);
    }

    std::string& File::GetCurrentLine(UT::i32 line) {
        return lines.at(line);
    }
}