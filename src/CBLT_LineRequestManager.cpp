#include "CBLT_LineRequestManager.hpp"

namespace CBLT {
    LineRequestManager::LineRequestManager(void) {}

    LineRequestManager::~LineRequestManager(void) {}

    void LineRequestManager::Request(LineRequestType type, UT::ui32 line) {
        switch (type) {
            case LineRequestType::DELETE_LINE:
                deleteLineRequests.push_back(line);
                
                // Sort with desceding line ids, to avoid reindexing after deletion
                std::sort(deleteLineRequests.rbegin(), deleteLineRequests.rend());

                break;
            case LineRequestType::INDENT_LINE:
                indentLineRequests.push_back(line);
                break;
            default:
                CBLT::Utils::Err::Log("INVALID LINE REQUEST TYPE");
        }
    }

    void LineRequestManager::Pop(LineRequestType type) {
        switch (type) {
            case LineRequestType::DELETE_LINE:
                deleteLineRequests.pop_front();
                break;
            case LineRequestType::INDENT_LINE:
                indentLineRequests.pop_front();
                break;
            default:
                CBLT::Utils::Err::Log("INVALID LINE REQUEST TYPE");
        }
    }

    std::deque<UT::ui32>& LineRequestManager::Deletions(void) {
        return deleteLineRequests;
    }
            
    std::deque<UT::ui32>& LineRequestManager::Indents(void) {
        return indentLineRequests;
    }
}