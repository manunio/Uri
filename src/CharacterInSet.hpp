//
// Created by maxx on 04/10/21.
//

#ifndef URI_CHARACTERINSET_HPP
#define URI_CHARACTERINSET_HPP

#include <initializer_list>
#include <memory>

namespace Uri {

    /*
     * This represents a set of characters which can be queried
     * to find out if a character is in this set or not.
     * */
    class CharacterSet {
    public:
        // Lifecycle management
        // rule 0 or 5
        ~CharacterSet() noexcept;

        CharacterSet(const CharacterSet &);

        CharacterSet(CharacterSet &&) noexcept;

        CharacterSet &operator=(const CharacterSet &);

        CharacterSet &operator=(CharacterSet &&) noexcept;

        /*
         * default constructor
         * */
        CharacterSet();

        /*
         * This constructs a character set that contains
         * just the given character.
         *
         * @param[in] c
         *  This is the only character to put in the set.
         *
         * */
        explicit CharacterSet(char c);

        /*
         * This constructs a character set that contains
         * just the given character.
         *
         * @param[in] first
         *  This is the first of the range of characters
         *  to put in the set.
         *
         * @param[in] last
         *  This is the last of the range of characters
         *  to put in the set.
         *
         * */
        CharacterSet(char first, char last);

        /*
         * This constructs a character set that contains all the
         * characters in all the other given character sets.
         *
         * @param[in] characterSets
         *  These are the character sets to include
         *
         * */
        CharacterSet(std::initializer_list<const CharacterSet> characterSets);

        /*
         * This method checks to see if the given character
         * is in the character set.
         *
         * @param[in] c
         *  This is the character to check.
         *
         * @return
         *  An indication of whether or not the given character
         *  is in the character set is returned.
         *
         * */
        bool Contains(char c) const;

    private:
        /**
         * This is the type of structure that contains the private
         * properties of the instance.  It is defined in the implementation
         * and declared here to ensure that it is scoped inside the class.
         */
        struct Impl;

        /**
         * This contains the private properties of the instance.
         */
        std::unique_ptr<struct Impl> impl_;

    };

    /*
     * This function determines whether or not the given character
     * is in the given character set.
     *
     * @param[in] c
     *  This is the character to check.
     *
     *  @param[in] characterSet
     *  This is the set of characters that are allowed.
     *
     * @return
     *  An indication of whether or not the given character
     *  is in the given character set is returned.
     *
     * */
    bool IsCharacterInSet(char c, const CharacterSet &characterSet);

}

#endif //URI_CHARACTERINSET_HPP
