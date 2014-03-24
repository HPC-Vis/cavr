#include <cavr/util/lua_state.h>
#include "gtest/gtest.h"
using cavr::util::LuaState;

const char* lua_file =
"a = {"
"  b = {"
"    c = 1.0;"
"    d = \"hi\";"
"    t = 1;"
"    f = 0;"
"  };"
"  c = true;"
"  doge = \"wow\";"
"}";

const char* bad_file =
"a = {"
"  b = {"
"    c = 1.0;"
"    d = \"hi;"
"  };"
"  c = true;"
"  doge = \"wow\";"
"}";

TEST(lua_state, lua_state) {
  LuaState ls;
  EXPECT_TRUE(ls.loadBuffer(lua_file));
  EXPECT_EQ(0, ls.getStackDepth());
  EXPECT_FALSE(ls.pushTable("b"));
  EXPECT_EQ(0, ls.getStackDepth());
  EXPECT_TRUE(ls.pushTable("a"));
  EXPECT_EQ(1, ls.getStackDepth());
  bool ac = false;
  EXPECT_FALSE(ls.getValue("d", ac));
  EXPECT_EQ(1, ls.getStackDepth());
  EXPECT_TRUE(ls.getValue("c", ac));
  EXPECT_TRUE(ac);
  std::string doge;
  EXPECT_FALSE(ls.getValue("doge", ac));
  EXPECT_TRUE(ac);
  EXPECT_TRUE(ls.getValue("doge", doge));
  EXPECT_EQ("wow", doge);
  EXPECT_FALSE(ls.getValue("b", doge));
  EXPECT_TRUE(ls.pushTable("b"));
  double c = -1.0;
  EXPECT_TRUE(ls.getValue("c", c));
  EXPECT_DOUBLE_EQ(1.0, c);
  std::string d;
  EXPECT_TRUE(ls.getValue("d", d));
  EXPECT_EQ("hi", d);
  bool t = false;
  bool f = true;
  EXPECT_TRUE(ls.getValue("t", t));
  EXPECT_TRUE(ls.getValue("f", f));
  EXPECT_FALSE(f);
  EXPECT_TRUE(t);
  EXPECT_EQ(2, ls.getStackDepth());
  ls.reset();
  EXPECT_EQ(0, ls.getStackDepth());
  EXPECT_TRUE(ls.pushTable("a"));
  doge = "doge";
  EXPECT_TRUE(ls.getValue("doge", doge));
  EXPECT_EQ("wow", doge);
}

