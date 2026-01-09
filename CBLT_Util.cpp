#include "CBLT_Util.hpp"

namespace CBLT {
    namespace Utils {
        namespace Err {
            void Log(std::string msg) {
                std::cerr << "CBLT_ERR: " << msg << "\n";
            }
        } // Err

        std::string TrimLeadingColon(std::string& str) {
            if (!str.empty() && str[0] == ':') return str.substr(1);
            return str;
        }

    } // Utils
} // CBLT