/**
 * @file Uri.cpp
 *
 * This module contains the implementation of the Uri::Uri class.
 *
 * © 2021 Manu Nair
 */

#include <string>
#include <Uri/Uri.hpp>
#include <cinttypes>

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
            if (userInfoDelimiter == std::string::npos) {
                userInfo.clear();
                hostPortString = authorityString;
            } else {
                userInfo = authorityString.substr(0, userInfoDelimiter);
                hostPortString = authorityString.substr(userInfoDelimiter + 1);
            }

            // Next, parsing host and port from the authority and path.
            const auto portDelimiter = hostPortString.find(':');
            if (portDelimiter == std::string::npos) {
                host = hostPortString;
                hasPort = false;
            } else {
                host = hostPortString.substr(0, portDelimiter);
                const auto portString = hostPortString.substr(portDelimiter + 1);

                // Next, parse the port number.
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
        auto schemeEnd = uriString.find(':');
        std::string rest;
        if (schemeEnd == std::string::npos) {
            impl_->scheme.clear();
            rest = uriString;
        } else {
            impl_->scheme = uriString.substr(0, schemeEnd);
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

        // Finally, if anything is left, it's the query element.
        if (rest.empty()) {
            impl_->query.clear();
        } else {
            impl_->query = rest.substr(1);
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
