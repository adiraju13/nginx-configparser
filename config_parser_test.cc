#include <sstream>
#include <string>

#include "gtest/gtest.h"
#include "config_parser.h"

// test of file config
TEST(NginxConfigParserTest, SimpleConfig) {
  NginxConfigParser parser;
  NginxConfig out_config;

  bool success = parser.Parse("example_config", &out_config);
  EXPECT_TRUE(success);
}

//simple testing of ToString function
TEST(NginxConfigTest, ToString){
	NginxConfigStatement statement;
	statement.tokens_.push_back("foo");
	statement.tokens_.push_back("bar");
	EXPECT_EQ(statement.ToString(0), "foo bar;\n");

}

class NginxStringConfigTest : public ::testing::Test {
protected:
	bool ParseString(const std::string config_string){
		std::stringstream config_stream(config_string);
		return parser_.Parse(&config_stream, &out_config_);
	}
	
	NginxConfigParser parser_;
	NginxConfig out_config_;
};


//tests that a simple valid config for true
//tests that the config output data structure is valid
TEST_F(NginxStringConfigTest, AnotherSimpleConfig) {
	EXPECT_TRUE(ParseString("foo bar;"));
	EXPECT_EQ(out_config_.statements_.size(), 1);
	EXPECT_EQ(out_config_.statements_.at(0)->tokens_.at(0), "foo");
}

//tests a simple invalid config for false
TEST_F(NginxStringConfigTest, InvalidConfig) {
	EXPECT_FALSE(ParseString("foo bar"));
}

//This one fails, unbalance number of open and closed brackets
//FIXED: Kept track of number of open and closed brackets
TEST_F(NginxStringConfigTest, UnbalancedConfig)
{
	EXPECT_FALSE(ParseString("server { listen 80;"));
}

// This one fails, Cannot handle a nested config
// FIXED: A closed bracket can be followed by a semicolon OR a another clsoed bracket
TEST_F(NginxStringConfigTest, NestedConfig){
	EXPECT_TRUE(ParseString("foo bar; \nOutside { inside { foo bar; } }"));
	std::string s = out_config_.ToString(0);
	EXPECT_EQ(s, "foo bar;\nOutside {\n  inside {\n    foo bar;\n  }\n}\n");
}

