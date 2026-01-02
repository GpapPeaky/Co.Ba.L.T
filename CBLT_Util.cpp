#include "CBLT_Util.hpp"

namespace CBLT {
    namespace Utils {
        namespace Err {
            void Log(std::string msg) {
                std::cerr << "CBLT_ERR: " << msg << "\n";
            }
        } // Err
    } // Utils
} // CBLT