#include "IsCharacterInSet.hpp"

namespace Uri {
    bool IsCharacterInSet(
            char c,
            std::initializer_list<char> characterSet) {
        for (const auto *charInSet = characterSet.begin(); charInSet != characterSet.end(); ++charInSet) {
            const auto first = *charInSet++; // TODO: refactor to avoid pointer arithmetic
            const auto last = *charInSet;
            if ((c >= first) && (c <= last)) {
                return true;
            }
        }
        return false;
    }
}