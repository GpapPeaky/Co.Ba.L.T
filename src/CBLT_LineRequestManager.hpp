#pragma once

#include <deque>
#include "CBLT_Util.hpp"

namespace CBLT {
    enum class LineRequestType : UT::ui8 {
        DELETE_LINE = 0,
        INDENT_LINE  
    }; 

    class LineRequestManager {
        private:
            std::deque<UT::ui32> deleteLineRequests; /* Line ids to be deleted, decrement ids after each deletion as not to overflow */
            std::deque<UT::ui32> indentLineRequests; /* Line ids to indent, need to be updated accordingly before any new indentations */
        public:
            LineRequestManager(void);
            ~LineRequestManager(void);

            // Make a line request of specific type at a specific line
            void Request(LineRequestType type, UT::ui32 line);

            // Pop the oldest element
            void Pop(LineRequestType type);

            std::deque<UT::ui32>& Deletions(void);
            
            std::deque<UT::ui32>& Indents(void);
    }; // Line request manager
} // CBLT