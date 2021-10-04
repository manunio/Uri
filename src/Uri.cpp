/**
 * @file Uri.cpp
 *
 * This module contains the implementation of the Uri::Uri class.
 *
 * © 2021 Manu Nair
 */

#include "IsCharacterInSet.hpp"
#include "PercentEncodedCharacterDecoder.hpp"

#include <string>
#include <Uri/Uri.hpp>
#include <cinttypes>
#include <functional>

namespace {
    /**
     * This function parses the given string as an unsigned 16-bit
     * integer, detecting invalid characters, overflow, etc.
     * @param[in] numberString
     *      This is the string containing the number to parse.
     * @param number[out]
     *      This is where to store port number to parse.
     * @return
     *      An indication of whether or not the number was parsed
     *      successfully returned.
     */
    bool ParseUint16(const std::string &numberString, uint16_t &number) {
        uint32_t numberIn32Bits = 0;
        for (auto c : numberString) {
            if ((c < '0') || (c > '9')) {
                return false;
            }
            const uint32_t v = 10;
            numberIn32Bits *= v;
            numberIn32Bits += (uint16_t) (c - '0');
            // TODO: handle following warning.
            if ((numberIn32Bits & ~((1 << 16) - 1)) != 0) {
                return false;
            }
        }
        number = (uint16_t) numberIn32Bits;
        return true;
    }


    /**
     * This function takes a given 'stillPassing' strategy
     * and invokes it on the sequence of characters in the given
     * string, to check if the string passes or not.
     *
     * @param[in] candidate
     * This a string to test.
     *
     * @param[in] stillPassing
     *  This is a strategy to invoke in order to test the string.
     *
     * @return
     *  An indication of whether or not the given candidate string
     *  passes the test is returned.
     *
     * */
    bool FailsMatch(const std::string &candidate, std::function<bool(char, bool)> stillPassing) {
        for (const auto c: candidate) {
            if (!stillPassing(c, false)) {
                return true;
            }
        }
        return !stillPassing(' ', true);
    }

    /**
     * This function returns a strategy function that
     * maybe used with the FailsMatch function to test a scheme
     * to make sure its legal according to the standard.
     *
     * @return
     *  A strategy function that may be used with the
     *  FailsMatch function to test a scheme,
     *  to make sure it is legal according to the standard is returned.
     * */
    std::function<bool(char, bool)> LegalSchemeCheckStrategy() {
        auto isFirstCharacter = std::make_shared<bool>(true);
        return [isFirstCharacter](char c, bool end) {
            if (end) {
                return !*isFirstCharacter;
            }

            bool check{};
            if (*isFirstCharacter) {
                check = Uri::IsCharacterInSet(c, {'a', 'z', 'A', 'Z'});
            } else {
                check = Uri::IsCharacterInSet(c, {'a', 'z', 'A', 'Z', '0', '9', '+', '+', '-', '-', '.', '.'});
            }
            *isFirstCharacter = false;
            return check;
        };
    }

    /**
    * This method checks and decodes the given query or fragment.
    *
    *  @param[in,out] queryOrFragment
    *      On input, this is the path queryOrFragment to check and decode.
    *      On output, this is the decoded query or fragment.
    *
    *  @return
    *      An indication of whether or not the query or fragment
    *      passed all checks and was decoded successfully is returned.
    *
    * */
    bool DecodeQueryOrFragment(std::string &queryOrFragment) {
        const auto originalQueryOrFragment = std::move(queryOrFragment);
        queryOrFragment.clear();
        size_t decoderState = 0;
        Uri::PercentEncodedCharacterDecoder pecDecoder{};
        for (const auto c: originalQueryOrFragment) {
            switch (decoderState) {
                case 0: { // default state
                    if (c == '%') {
                        pecDecoder = Uri::PercentEncodedCharacterDecoder{};
                        decoderState = 1;
                    } else {
                        if (Uri::IsCharacterInSet(
                                c,
                                {
                                        // unreserved
                                        'a', 'z', 'A', 'Z', // ALPHA
                                        '0', '9', //DIGIT
                                        '-', '-', '.', '.', '_', '_', '~', '~',

                                        // sub-delims
                                        '!', '!', '$', '$', '&', '&', '\'', '\'', '(', '(', ')', ')',
                                        '*', '*', '+', '+', ',', ',', ';', ';', '=', '=',

                                        // (also allowed in pchar)
                                        ':', ':', '@', '@',

                                        // (also allowed in query or fragment)
                                        '/', '/', '?', '?'
                                }
                        )) {
                            queryOrFragment.push_back(c);
                        } else {
                            return false;
                        }
                    }
                }
                    break;
                case 1: { // % ...
                    if (!pecDecoder.NextEncodedCharacter(c)) {
                        return false;
                    }

                    if (pecDecoder.Done()) {
                        decoderState = 0;
                        queryOrFragment.push_back((char) pecDecoder.GetDecodedCharacter());
                    }
                }
                    break;

                default:
                    break;
            }
        }
        return true;
    }


}

namespace Uri {
    /**
     * This contains the private properties of a Uri instance.
     */
    struct Uri::Impl {
        /**
         * This is the "scheme" element of the URI.
         */
        std::string scheme;

        /**
         * This is the "host" element of the URI.
        */
        std::string host;

        /**
         * This flag indicates whether or not the
         * URI includes a port number.
        */
        bool hasPort = false;
        /**
        * This is the "port" element of the URI.
       */
        uint16_t port = 0;

        /**
         * This is the "path" element of the URI,
         * as a sequence of segments.
        */
        std::vector<std::string> path;

        /**
        * This is the "fragment" element of the URI,
        * if it has one.
       */
        std::string fragment;

        /**
        * This is the "query" element of the URI,
        * if it has one.
       */
        std::string query;

        /**
        * This is the "UserInfo" element of the URI.
        */
        std::string userInfo;

        // Methods

        /**
         * This method checks and decodes the given path segment.
         *
         *  @param[in,out] segment
         *      On input, this is the path segment to check and decode.
         *      On output, this is the decoded path segment.
         *
         *  @return
         *      An indication of whether or not the path segment
         *      passed all checks and was decoded successfully is returned.
         *
         * */
        bool DecodePathSegment(std::string &segment) {
            const auto originalSegment = std::move(segment);
            segment.clear();
            size_t decoderState = 0;
            PercentEncodedCharacterDecoder pecDecoder{};
            for (const auto c: originalSegment) {
                switch (decoderState) {
                    case 0: { // default state
                        if (c == '%') {
                            pecDecoder = PercentEncodedCharacterDecoder{};
                            decoderState = 1;
                        } else {
                            if (IsCharacterInSet(
                                    c,
                                    {
                                            // unreserved
                                            'a', 'z', 'A', 'Z', // ALPHA
                                            '0', '9', //DIGIT
                                            '-', '-', '.', '.', '_', '_', '~', '~',

                                            // sub-delims
                                            '!', '!', '$', '$', '&', '&', '\'', '\'', '(', '(', ')', ')',
                                            '*', '*', '+', '+', ',', ',', ';', ';', '=', '=',

                                            // (also allowed in segment or pchar)
                                            ':', ':', '@', '@'
                                    }
                            )) {
                                segment.push_back(c);
                            } else {
                                return false;
                            }
                        }
                    }
                        break;

                    case 1: { // % ...
                        if (!pecDecoder.NextEncodedCharacter(c)) {
                            return false;
                        }

                        if (pecDecoder.Done()) {
                            decoderState = 0;
                            segment.push_back((char) pecDecoder.GetDecodedCharacter());
                        }
                    }
                        break;

                    default:
                        break;
                }
            }
            return true;
        }


        /**
         * This method builds internal path element sequence
         * by parsing it from the given path string.
         *
         * @param[in] pathString
         *      This is the string containing the whole part of the URI.
         * @return
         *  An indication if the path was parsed correctly or not
         *  is returned.
         *
         * */
        bool ParsePath(std::string &pathString) {
            path.clear();
            if (pathString == "/") {
                // Special case of a path that is empty but needs a single
                // empty-string element to indicate that it is absolute.
                path.emplace_back("");
                pathString.clear();
            } else if (!pathString.empty()) {
                for (;;) {
                    auto pathDelimiter = pathString.find('/');
                    if (pathDelimiter == std::string::npos) {
                        path.push_back(pathString);
                        pathString.clear();
                        break;
                    }
                    path.emplace_back(
                            pathString.begin(),
                            pathString.begin() + pathDelimiter
                    );
                    pathString = pathString.substr(pathDelimiter + 1);

                }

            }
            for (auto &segment: path) {
                if (!DecodePathSegment(segment)) {
                    return false;
                }
            }
            return true;
        }

        /**
         * This method parses the element that makes up authority
         * composite part of the URI, by parsing from the given string
         *
         * @param[in] authorityString
         *  This is the string containing the whole authority part
         *  of the URI.
         *
         * @return
         *  An indication if the path was parsed correctly or not
         *  is returned.
         * */
        bool ParseAuthority(const std::string &authorityString) {
            // Next, check if there is a UserInfo, if so, extract it.
            const auto userInfoDelimiter = authorityString.find('@');
            std::string hostPortString;
            userInfo.clear();
            if (userInfoDelimiter == std::string::npos) {
                hostPortString = authorityString;
            } else {
                const auto userInfoEncoded = authorityString.substr(0, userInfoDelimiter);
                size_t decoderState = 0;
                PercentEncodedCharacterDecoder pecDecoder{};
                for (const auto c: userInfoEncoded) {
                    switch (decoderState) {
                        case 0: { // default state
                            if (c == '%') {
                                pecDecoder = PercentEncodedCharacterDecoder{};
                                decoderState = 1;
                            } else {
                                if (IsCharacterInSet(
                                        c,
                                        {
                                                // unreserved
                                                'a', 'z', 'A', 'Z', // ALPHA
                                                '0', '9', //DIGIT
                                                '-', '-', '.', '.', '_', '_', '~', '~',

                                                // sub-delims
                                                '!', '!', '$', '$', '&', '&', '\'', '\'', '(', '(', ')', ')',
                                                '*', '*', '+', '+', ',', ',', ';', ';', '=', '=',

                                                // (also allowed in user-info)
                                                ':', ':',
                                        }
                                )) {
                                    userInfo.push_back(c);
                                } else {
                                    return false;
                                }
                            }
                        }
                            break;

                        case 1: { // % ...
                            if (!pecDecoder.NextEncodedCharacter(c)) {
                                return false;
                            }

                            if (pecDecoder.Done()) {
                                decoderState = 0;
                                userInfo.push_back((char) pecDecoder.GetDecodedCharacter());
                            }
                        }
                            break;

                        default:
                            break;
                    }
                }
                hostPortString = authorityString.substr(userInfoDelimiter + 1);
            }

            // Next, parsing host and port from the authority and path.
            std::string portString;
            size_t decoderState = 0;
            int decodedCharacter = 0;
            host.clear();
            PercentEncodedCharacterDecoder pecDecoder{};
            for (const auto c: hostPortString) {
                switch (decoderState) {
                    case 0: { // first character
                        if (c == '[') {
                            host.push_back(c);
                            decoderState = 3;
                            break;
                        } else {
                            decoderState = 1;
                        }
                    } // skipping `break;` is intentional

                    case 1: { // reg-name or IPv4Address
                        if (c == '%') {
                            pecDecoder = PercentEncodedCharacterDecoder{};
                            decoderState = 2;
                        } else if (c == ':') {
                            decoderState = 8;
                        } else {
                            if (IsCharacterInSet(
                                    c,
                                    {
                                            // unreserved
                                            'a', 'z', 'A', 'Z', // ALPHA
                                            '0', '9', //DIGIT
                                            '-', '-', '.', '.', '_', '_', '~', '~',

                                            // sub-delims
                                            '!', '!', '$', '$', '&', '&', '\'', '\'', '(', '(', ')', ')',
                                            '*', '*', '+', '+', ',', ',', ';', ';', '=', '=',

                                            // (also allowed in reg-name)
                                            ':', ':',
                                    }
                            )) {
                                host.push_back(c);
                            } else {
                                return false;
                            }
                        }
                    }
                        break;

                    case 2: { // % ...
                        if (!pecDecoder.NextEncodedCharacter(c)) {
                            return false;
                        }

                        if (pecDecoder.Done()) {
                            decoderState = 1;
                            host.push_back((char) pecDecoder.GetDecodedCharacter());
                        }
                    }
                        break;

                    case 3: { // IP-literal
                        if (c == 'v') {
                            host.push_back(c);
                            decoderState = 5;
                            break;
                        } else {
                            decoderState = 4;
                        }
                    }
                        break;

                    case 4: { // IPv6Address
                        // TODO: research this offline first,
                        //  before attempting to code it.
                        host.push_back(c);
                        if (c == ']') {
                            decoderState = 7;
                        }
                    }
                        break;

                    case 5: { // IPvFuture: v ...
                        if (c == '.') {
                            decoderState = 6;
                        } else if (!IsCharacterInSet(c, {'0', '9', 'A', 'F'})) {
                            return false;
                        }
                        host.push_back(c);
                    }
                        break;

                    case 6: { // IPvFuture:v 1*HEXDIG ...
                        host.push_back(c);
                        if (c == ']') {
                            decoderState = 7;
                        } else if (
                                !IsCharacterInSet(
                                        c,
                                        {
                                                // unreserved
                                                'a', 'z', 'A', 'Z', // ALPHA
                                                '0', '9', //DIGIT
                                                '-', '-', '.', '.', '_', '_', '~', '~',

                                                // sub-delims
                                                '!', '!', '$', '$', '&', '&', '\'', '\'', '(', '(', ')', ')',
                                                '*', '*', '+', '+', ',', ',', ';', ';', '=', '=',

                                                // (also allowed in IPvFuture)
                                                ':', ':',
                                        }
                                )) {
                            return false;
                        }
                    }
                        break;

                    case 7: { // illegal to have anything else, unless its a colon,
                        // in which case it's a port delimiter
                        if (c == ':') {
                            decoderState = 8;
                        } else {
                            return false;
                        }
                    }
                        break;

                    case 8: { // port
                        portString.push_back(c);
                    }
                        break;

                    default:
                        break;
                }
            }
            if (portString.empty()) {
                hasPort = false;
            } else {
                if (!ParseUint16(portString, port)) {
                    return false;
                }
                hasPort = true;
            }
            return true;
        }

    };

    Uri::~Uri() = default;

    Uri::Uri()
            : impl_(new Impl) {
    }


    bool Uri::ParseFromString(const std::string &uriString) {

        // First, parse the "scheme".
        // Limit our search so we don't scan into the authority
        // or path elements, because these may have the colon
        // character as well, which we might misinterpret
        // as the scheme delimiter.
        auto authorityOrPathDelimiterStart = uriString.find('/');
        if (authorityOrPathDelimiterStart == std::string::npos) {
            authorityOrPathDelimiterStart = uriString.length();
        }
        const auto schemeEnd = uriString.substr(0, authorityOrPathDelimiterStart).find(':');
        std::string rest;
        if (schemeEnd == std::string::npos) {
            impl_->scheme.clear();
            rest = uriString;
        } else {
            impl_->scheme = uriString.substr(0, schemeEnd);
            if (FailsMatch(
                    impl_->scheme,
                    LegalSchemeCheckStrategy()
            )) {
                return false;
            }
            rest = uriString.substr(schemeEnd + 1);
        }


        // Next, parse the host.
        const auto pathEnd = rest.find_first_of("?#");
        auto authorityAndPathString = rest.substr(0, pathEnd);
        const auto queryAndOrFragment = rest.substr(authorityAndPathString.length());
        std::string pathString;
        if (authorityAndPathString.substr(0, 2) == "//") {
            // Strip off authority marker.
            authorityAndPathString = authorityAndPathString.substr(2);

            // First separate the authority from the path.
            auto authorityEnd = authorityAndPathString.find('/');
            if (authorityEnd == std::string::npos) {
                authorityEnd = authorityAndPathString.length();
            }
            pathString = authorityAndPathString.substr(authorityEnd);
            auto authorityString = authorityAndPathString.substr(0, authorityEnd);

            // Parse the elements inside the authority string.
            if (!impl_->ParseAuthority(authorityString)) {
                return false;
            }

        } else {
            impl_->userInfo.clear();
            impl_->host.clear();
            impl_->hasPort = false;
            pathString = authorityAndPathString;
        }

        // Next, parse the "path".
        if (!impl_->ParsePath(pathString)) {
            return false;
        }

        // Next, parse the fragment if there is one.
        // query always starts with # sign.
        const auto fragmentDelimiter = queryAndOrFragment.find('#');
        if (fragmentDelimiter == std::string::npos) {
            impl_->fragment.clear();
            rest = queryAndOrFragment;
        } else {
            impl_->fragment = queryAndOrFragment.substr(fragmentDelimiter + 1);
            rest = queryAndOrFragment.substr(0, fragmentDelimiter);
        }
        if (!DecodeQueryOrFragment(impl_->fragment)) {
            return false;
        }

        // Finally, if anything is left, it's the query element.
        if (rest.empty()) {
            impl_->query.clear();
        } else {
            impl_->query = rest.substr(1);
        }

        if (!DecodeQueryOrFragment(impl_->query)) {
            return false;
        }

        return true;
    }

    std::string Uri::GetScheme() const {
        return impl_->scheme;
    }

    std::string Uri::GetHost() const {
        return impl_->host;
    }

    std::vector<std::string> Uri::GetPath() const {
        return impl_->path;
    }

    bool Uri::HasPort() const {
        return impl_->hasPort;
    }

    uint16_t Uri::GetPort() const {
        return impl_->port;
    }

    bool Uri::IsRelativeReference() const {
        return impl_->scheme.empty();
    }

    bool Uri::ContainsRelativePath() const {
        if (impl_->path.empty()) {
            return true;
        }
        return !impl_->path[0].empty();
    }

    std::string Uri::GetFragment() const {
        return impl_->fragment;
    }

    std::string Uri::GetQuery() const {
        return impl_->query;
    }

    std::string Uri::GetUserInfo() const {
        return impl_->userInfo;
    }


}
