/**
 * @file UriTests.cpp
 *
 * This module contains the unit tests of the Uri::Uri class.
 *
 * © 2018 by Richard Walters
 */

#include <gtest/gtest.h>
#include <Uri/Uri.hpp>

TEST(UriTests, Placeholder) {
    Uri::Uri uri;
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

    ASSERT_TRUE(uri.ParseFromString("urn:book:fantasy:Hobbit"));
    ASSERT_EQ("urn", uri.GetScheme());
    ASSERT_EQ("", uri.GetHost());
    ASSERT_EQ((std::vector<std::string>{
            "urn:book:fantasy:Hobbit",
    }),
              uri.GetPath()
    );

    uri.setPathDelimiter(":");

    ASSERT_TRUE(uri.ParseFromString("urn:book:fantasy:Hobbit"));
    ASSERT_EQ("urn", uri.GetScheme());
    ASSERT_EQ("", uri.GetHost());
    ASSERT_EQ((std::vector<std::string>{
            "book",
            "fantasy",
            "Hobbit",
    }),
              uri.GetPath()
    );
}
