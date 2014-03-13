#include <cavr/config/lua_reader.h>
#include "gtest/gtest.h"
using cavr::config::LuaReader;

TEST(lua_reader, get) {
  LuaReader* reader = LuaReader::createFromBuffer("hi this is invalid syntax");
  EXPECT_EQ(nullptr, reader);
  reader = LuaReader::createFromBuffer("a = { b = { c = 1; d = \"cat\"; } }");
  ASSERT_NE(nullptr, reader);
  int x = -1;
  EXPECT_FALSE(reader->get("a.c", x));
  EXPECT_EQ(-1, x);
  EXPECT_TRUE(reader->get("a.b.c", x));
  EXPECT_EQ(1, x);
  EXPECT_FALSE(reader->get("a.b.d", x));
  std::string y;
  EXPECT_TRUE(reader->get("a.b.d", y));
  EXPECT_EQ("cat", y);
}

