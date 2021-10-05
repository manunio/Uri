#include <z3.h>
#include <set>
#include "CharacterInSet.hpp"

namespace Uri {

    struct CharacterSet::Impl {
        /*
         * This holds the character in set.
         * */
        std::set<char> charactersInSet;
    };

    CharacterSet::~CharacterSet() noexcept = default;

    // copy constructor
    CharacterSet::CharacterSet(const CharacterSet &other)
            : impl_{new Impl{*other.impl_}} {

    }

    // copy assignment
    CharacterSet &CharacterSet::operator=(const CharacterSet &other) {
        if (this != &other) {
            *impl_ = *other.impl_;
        }
        return *this;
    }

    // move constructor
    CharacterSet::CharacterSet(CharacterSet &&) noexcept = default;

    //    move constructor assignment
    CharacterSet &CharacterSet::operator=(CharacterSet &&) noexcept = default;


    CharacterSet::CharacterSet() : impl_{new Impl} {}


    CharacterSet::CharacterSet(char c) : impl_{new Impl} {
        (void) impl_->charactersInSet.insert(c);
    }

    CharacterSet::CharacterSet(char first, char last) : impl_{new Impl} {
        for (char c = first; c < last + 1; ++c) {
            impl_->charactersInSet.insert(c);
        }
    }

    CharacterSet::CharacterSet(std::initializer_list<const CharacterSet> characterSets) : impl_{new Impl} {

        for (auto characterSet = characterSets.begin();
             characterSet != characterSets.end();
             ++characterSet) {
            impl_->charactersInSet.insert(
                    characterSet->impl_->charactersInSet.begin(),
                    characterSet->impl_->charactersInSet.end());
        }
    }

    bool CharacterSet::Contains(char c) const {
        return impl_->charactersInSet.find(c) != impl_->charactersInSet.end();
    }

    bool IsCharacterInSet(char c, const CharacterSet &characterSet) {
        return characterSet.Contains(c);
    }


}