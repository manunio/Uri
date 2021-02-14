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

TEST(UriTests, PathFromStringUrl) {
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
        ASSERT_TRUE(uri.ParseFromString(testVector.PathIn)) << index; //shift out index
        ASSERT_EQ(testVector.pathOut, uri.GetPath()) << index;
        ++index;
    }

}

#pragma clang diagnostic pop