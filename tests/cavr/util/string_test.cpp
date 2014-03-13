#include <cavr/util/string.h>
#include "gtest/gtest.h"

using cavr::util::String;

TEST(string, split) {
  std::string a = "ant.bear.cat.dog.elk";
  std::string ant = "doge";
  std::string bearcatdogelk = "whatever";
  EXPECT_EQ(1, String::split(a, ".", ant, bearcatdogelk));
  EXPECT_EQ("ant", ant);
  EXPECT_EQ("bear.cat.dog.elk", bearcatdogelk);
  std::string bear = "philosoraptor";
  std::string cat = "zebra";
  std::string dogelk = "unicorn";
  std::string delimiter = ".";
  EXPECT_EQ(3, String::split(a, delimiter, ant, bear, cat, dogelk));
  EXPECT_EQ("ant", ant);
  EXPECT_EQ("bear", bear);
  EXPECT_EQ("cat", cat);
  EXPECT_EQ("dog.elk", dogelk);
  std::string dog = "parrot";
  EXPECT_EQ(1, String::split(dogelk, ".", ant, bear, cat, dog));
  EXPECT_EQ("dog", ant);
  EXPECT_EQ("elk", bear);
  EXPECT_EQ("", cat);
  EXPECT_EQ("", dog);
}

TEST(string, rsplit) {
#if 0
  std::string abcdef = "a.b.c.d.e.f";
  std::string f = "z";
  std::string abcde = "y";
  EXPECT_EQ(1, String::rsplit(abcdef, ".", abcde, f));
  EXPECT_EQ("a.b.c.d.e", abcde);
  EXPECT_EQ("f", f);
#endif
}
