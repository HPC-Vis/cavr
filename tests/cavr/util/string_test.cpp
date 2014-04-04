#include <cavr/util/string.h>
#include "gtest/gtest.h"

using cavr::util::String;

TEST(string, split) {
  std::string a = "ant-->bear-->cat-->dog-->elk";
  std::string ant = "doge";
  std::string bearcatdogelk = "whatever";
  EXPECT_EQ(1, String::split(a, "-->", ant, bearcatdogelk));
  EXPECT_EQ("ant", ant);
  EXPECT_EQ("bear-->cat-->dog-->elk", bearcatdogelk);
  std::string bear = "philosoraptor";
  std::string cat = "zebra";
  std::string dogelk = "unicorn";
  std::string delimiter = "-->";
  EXPECT_EQ(3, String::split(a, delimiter, ant, bear, cat, dogelk));
  EXPECT_EQ("ant", ant);
  EXPECT_EQ("bear", bear);
  EXPECT_EQ("cat", cat);
  EXPECT_EQ("dog-->elk", dogelk);
  std::string dog = "parrot";
  EXPECT_EQ(1, String::split(dogelk, "-->", ant, bear, cat, dog));
  EXPECT_EQ("dog", ant);
  EXPECT_EQ("elk", bear);
  EXPECT_EQ("", cat);
  EXPECT_EQ("", dog);

  std::vector<std::string> strings;
  String::split(a, "-->", strings);
  ASSERT_EQ(5, strings.size());
  EXPECT_EQ("ant", strings[0]);
  EXPECT_EQ("bear", strings[1]);
  EXPECT_EQ("cat", strings[2]);
  EXPECT_EQ("dog", strings[3]);
  EXPECT_EQ("elk", strings[4]);
  strings.clear();
  String::split(a, "-->", strings, 2);
  ASSERT_EQ(3, strings.size());
  EXPECT_EQ("ant", strings[0]);
  EXPECT_EQ("bear", strings[1]);
  EXPECT_EQ("cat-->dog-->elk", strings[2]);
  strings.clear();
  String::split(".quas.wex.exort", ".", strings);
  ASSERT_EQ(4, strings.size());
  EXPECT_EQ("", strings[0]);
  EXPECT_EQ("quas", strings[1]);
  EXPECT_EQ("wex", strings[2]);
  EXPECT_EQ("exort", strings[3]);
}

TEST(string, rsplit) {
  std::string abcdef = "a->b->c->d->e->f";
  std::string f = "z";
  std::string abcde = "y";
  EXPECT_EQ(1, String::rsplit(abcdef, "->", abcde, f));
  EXPECT_EQ("a->b->c->d->e", abcde);
  EXPECT_EQ("f", f);
  std::string e = "x";
  std::string abcd = "w";
  EXPECT_EQ(2, String::rsplit(abcdef, "->", abcd, e, f));
  EXPECT_EQ("a->b->c->d", abcd);
  EXPECT_EQ("e", e);
  EXPECT_EQ("f", f);

  std::vector<std::string> strings;
  String::rsplit(abcdef, "->", strings);
  ASSERT_EQ(6, strings.size());
  EXPECT_EQ("a", strings[0]);
  EXPECT_EQ("b", strings[1]);
  EXPECT_EQ("c", strings[2]);
  EXPECT_EQ("d", strings[3]);
  EXPECT_EQ("e", strings[4]);
  EXPECT_EQ("f", strings[5]);

  strings.clear();
  String::rsplit(abcdef, "->", strings, 3);
  ASSERT_EQ(4, strings.size());
  EXPECT_EQ("a->b->c", strings[0]);
  EXPECT_EQ("d", strings[1]);
  EXPECT_EQ("e", strings[2]);
  EXPECT_EQ("f", strings[3]);
}
