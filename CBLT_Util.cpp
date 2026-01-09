#include "CBLT_Util.hpp"

namespace CBLT {
    namespace Utils {
        namespace Err {
            void Log(std::string msg) {
                std::cerr << "CBLT_ERR: " << msg << "\n";
            }
        } // Err

        std::string& TrimColon(std::string& str) {
            while (!str.empty() && str.back() == ':') {
                str.pop_back();
            }

            return str;
        }
    } // Utils
} // CBLT