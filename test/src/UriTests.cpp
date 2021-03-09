#pragma clang diagnostic push
#pragma ide diagnostic ignored "cert-err58-cpp"
/**
 * @file UriTests.cpp
 *
 * This module contains the unit tests of the Uri::Uri class.
 *
 * © 2018 by Richard Walters
 */

#include <gtest/gtest.h>
#include <cstddef>
#include <Uri/Uri.hpp>


TEST(UriTests, ParseFromStringUrlNoScheme) {
    Uri::Uri uri{};
    ASSERT_TRUE(uri.ParseFromString("foo/bar"));
    ASSERT_EQ("", uri.GetScheme());
    ASSERT_EQ((std::vector<std::string>{
            "foo",
            "bar",
    }),
              uri.GetPath()
    );
}

TEST(UriTests, ParseFromStringUrl) {
    Uri::Uri uri{};
    ASSERT_TRUE(uri.ParseFromString("http://www.example.com/foo/bar"));
    ASSERT_EQ("http", uri.GetScheme());
    ASSERT_EQ("www.example.com", uri.GetHost());
    ASSERT_EQ((std::vector<std::string>{
            "",
            "foo",
            "bar",
    }),
              uri.GetPath()
    );
}

TEST(UriTests, ParseFromStringUrnDefaultPathDeimiter) {
    Uri::Uri uri{};
    ASSERT_TRUE(uri.ParseFromString("urn:book:fantasy:Hobbit"));
    ASSERT_EQ("urn", uri.GetScheme());
    ASSERT_EQ("", uri.GetHost());
    ASSERT_EQ((std::vector<std::string>{
            "book:fantasy:Hobbit",
    }),
              uri.GetPath()
    );
}

TEST(UriTests, ParseFromStringPathCornerCases) {
    struct TestVector {
        std::string PathIn;
        std::vector<std::string> pathOut;
    };
    const std::vector<TestVector> testVectors{
            {"",     {}},
            {"/",    {""}}, // special case
            {"/foo", {"",    "foo"}},
            {"foo/", {"foo", ""}},
    };

    size_t index = 0;

    for (const auto &testVector: testVectors) {
        Uri::Uri uri{};
        ASSERT_TRUE(uri.ParseFromString(testVector.PathIn)) << index; // shift out index
        ASSERT_EQ(testVector.pathOut, uri.GetPath()) << index;
        ++index;
    }

}


TEST(UriTests, ParseFromStringHasAPortNumber) {
    Uri::Uri uri{};
    ASSERT_TRUE(uri.ParseFromString("http://www.example.com:8080/foo/bar"));
    ASSERT_EQ("www.example.com", uri.GetHost());
    ASSERT_TRUE(uri.HasPort());
    ASSERT_EQ(8080, uri.GetPort());
}

TEST(UriTests, ParseFromStringDoesNotHaveAPortNumber) {
    Uri::Uri uri{};
    ASSERT_TRUE(uri.ParseFromString("http://www.example.com/foo/bar"));
    ASSERT_EQ("www.example.com", uri.GetHost());
    ASSERT_FALSE(uri.HasPort());
}

TEST(UriTests, ParseFromStringTWiceFirstWithPortNumberThenWithout) {
    Uri::Uri uri{};
    ASSERT_TRUE(uri.ParseFromString("http://www.example.com:8080/foo/bar"));
    ASSERT_TRUE(uri.ParseFromString("http://www.example.com/foo/bar"));
    ASSERT_FALSE(uri.HasPort());
}

TEST(UriTests, ParseFromStringBadPortNumberPurelyAlphabetic) {
    Uri::Uri uri{};
    ASSERT_FALSE(uri.ParseFromString("http://www.example.com:spam/foo/bar"));
}

TEST(UriTests, ParseFromStringBadPortNumberStartsNumericEndsAlphabetic) {
    Uri::Uri uri{};
    ASSERT_FALSE(uri.ParseFromString("http://www.example.com:8080spam/foo/bar"));
}

TEST(UriTests, ParseFromStringLargestValidPortNumber) {
    Uri::Uri uri{};
    ASSERT_TRUE(uri.ParseFromString("http://www.example.com:65535/foo/bar"));
    ASSERT_TRUE(uri.HasPort());
    ASSERT_EQ(65535, uri.GetPort());

}

TEST(UriTests, ParseFromStringBadPortNumberTooBig) {
    Uri::Uri uri{};
    ASSERT_FALSE(uri.ParseFromString("http://www.example.com:65536/foo/bar"));
}

TEST(UriTests, ParseFromStringBadPortNumberNegative) {
    Uri::Uri uri{};
    ASSERT_FALSE(uri.ParseFromString("http://www.example.com:-1234/foo/bar"));
}

TEST(UriTests, ParseFromStringEndsAfterAuthority) {
    Uri::Uri uri{};
    ASSERT_TRUE(uri.ParseFromString("http://www.example.com"));
}

TEST(UriTests, ParseFromStringRelativeVsNonRelativeReferences) {
    struct TestVector {
        std::string uriString;
        bool isRelative;
    };
    const std::vector<TestVector> testVectors{
            {"http://www.example.com/", false},
            {"http://www.example.com",  false},
            {"/",                       true},
            {"foo",                     true},
    };

    size_t index = 0;

    for (const auto &testVector: testVectors) {
        Uri::Uri uri{};
        ASSERT_TRUE(uri.ParseFromString(testVector.uriString)) << index; // shift out index
        ASSERT_EQ(testVector.isRelative, uri.IsRelativeReference()) << index;
        ++index;
    }
}

TEST(UriTests, ParseFromStringRelativeVsNonRelativePaths) {
    struct TestVector {
        std::string uriString;
        bool containsRelativePath;
    };
    const std::vector<TestVector> testVectors{
            {"http://www.example.com/", false},
            {"http://www.example.com",  true},
            {"/",                       false},
            {"foo",                     true},


            /**
             * This is only a valid test vector if we understand
             * correctly that an empty string is a valid
             * "relative reference" URI with an empty path.
             */
            {"",                        true},
    };

    size_t index = 0;

    for (const auto &testVector: testVectors) {
        Uri::Uri uri{};
        ASSERT_TRUE(uri.ParseFromString(testVector.uriString)) << index; // shift out index
        ASSERT_EQ(testVector.containsRelativePath, uri.ContainsRelativePath()) << index;
        ++index;
    }

}

TEST(UriTests, ParseFromStringFragmentQueryAndFragmentElements) {
    struct TestVector {
        std::string uriString;
        std::string host;
        std::string query;
        std::string fragment;
    };
    const std::vector<TestVector> testVectors{
            {"http://www.example.com/",              "www.example.com", "",          ""},
            {"http://example.com?foo",               "example.com",     "foo",       ""},
            {"http://www.example.com#foo",           "www.example.com", "",          "foo"},
            {"http://www.example.com?foo#bar",       "www.example.com", "foo",       "bar"},
            {"http://www.example.com?earth?day#bar", "www.example.com", "earth?day", "bar"},
            {"http://www.example.com/spam?foo#bar",  "www.example.com", "foo",       "bar"},

            /**
             * NOTE: curiously, but we think this is correct, that
             * having a trailing question mark is equivalent to not having
             * any question mark, because in both cases, query element
             * is an empty string. Perhaps research deeper to see if this is right.
             */
            {"http://www.example.com/?",             "www.example.com", "",          ""},
    };

    size_t index = 0;

    for (const auto &testVector: testVectors) {
        Uri::Uri uri{};
        ASSERT_TRUE(uri.ParseFromString(testVector.uriString)) << index; // shift out index
        ASSERT_EQ(testVector.host, uri.GetHost()) << index;
        ASSERT_EQ(testVector.query, uri.GetQuery()) << index;
        ASSERT_EQ(testVector.fragment, uri.GetFragment()) << index;
        ++index;
    }

}

TEST(UriTests, ParseFromStringUserInfo) {
    struct TestVector {
        std::string uriString;
        std::string userInfo;
    };
    const std::vector<TestVector> testVectors{
            {"http://www.example.com/",               ""},
            {"http://manu@www.example.com",           "manu"},
            {"http://manu:istesting@www.example.com", "manu:istesting"},
            {"//www.example.com",                     ""},
            {"//bob@www.example.com",                 "bob"},
            {"/",                                     ""},
            {"foo",                                   ""},
    };

    size_t index = 0;

    for (const auto &testVector: testVectors) {
        Uri::Uri uri{};
        ASSERT_TRUE(uri.ParseFromString(testVector.uriString)) << index; // shift out index
        ASSERT_EQ(testVector.userInfo, uri.GetUserInfo()) << index;
        ++index;
    }

}


TEST(UriTests, ParseFromStringTWiceFirstWithUserInfoThenWithout) {
    Uri::Uri uri{};
    ASSERT_TRUE(uri.ParseFromString("http://manu@www.example.com/foo/bar"));
    ASSERT_TRUE(uri.ParseFromString("/foo/bar"));
    ASSERT_TRUE(uri.GetUserInfo().empty());
}

#pragma clang diagnostic pop