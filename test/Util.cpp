
#include <vector>

#include "TestUtil.hpp"

#include "LuaCpp/LuaUtil.hpp"

using namespace luacpp;

class LuaUtilTest : public LuaStateTest
{};

TEST_F(LuaUtilTest, TableListPairs)
{
	LuaTable table = LuaTable::create(L);
	table.addValue(1, "test1");
	table.addValue(2, "test2");
	table.addValue("key", "test3");

	std::vector<std::pair<std::string, std::string>> pairList;
	util::tableListPairs(table, pairList);

	ASSERT_EQ(3, pairList.size());
	ASSERT_TRUE(std::find(pairList.begin(), pairList.end(), std::make_pair<std::string, std::string>("1", "test1")) != pairList.end());
	ASSERT_TRUE(std::find(pairList.begin(), pairList.end(), std::make_pair<std::string, std::string>("2", "test2")) != pairList.end());
	ASSERT_TRUE(std::find(pairList.begin(), pairList.end(), std::make_pair<std::string, std::string>("key", "test3")) != pairList.end());
}

TEST_F(LuaUtilTest, TableToList)
{
	LuaTable table = LuaTable::create(L);
	table.addValue(1, "test1");
	table.addValue(2, "test2");
	table.addValue("key", "test3");

	std::vector<std::string> valueList;
	util::tableToList(table, valueList);

	ASSERT_EQ(2, valueList.size());
	ASSERT_TRUE(valueList[0] == "test1");
	ASSERT_TRUE(valueList[1] == "test2");
}
