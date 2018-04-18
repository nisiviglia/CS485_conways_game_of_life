#include "execution/impl/dispatch.h"

#include <memory>
#include <string>

#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include "rapidjson/document.h"
#include "rapidjson/error/en.h"
#include "rapidjson/rapidjson.h"
#include "rapidjson/schema.h"

#include "execution/tests/schema_provider.h"

using rapidjson::Document;
using rapidjson::GetParseError_En;
using rapidjson::SchemaDocument;
using rapidjson::SchemaValidatingReader;
using rapidjson::StringStream;
using rapidjson::UTF8;
using rapidjson::kParseDefaultFlags;
using ::testing::StrEq;

namespace execution {
namespace {
LocalSchemaProvider &GetSchemaProvider() {
  // Initialization runs exactly once.
  static LocalSchemaProvider *schema_provider = new LocalSchemaProvider;

  return *schema_provider;
}

void Validate(Document *doc, const SchemaDocument *schema,
              const std::string &json) {
  StringStream input(json.c_str());
  SchemaValidatingReader<kParseDefaultFlags, StringStream, UTF8<>> reader(
      input, *schema);
  
  doc->Populate(reader);

  ASSERT_FALSE(reader.GetParseResult().IsError())
    << "JSON failed to match schema: "
    << GetParseError_En(reader.GetParseResult().Code());
}

class DispatchTest : public ::testing::Test {
 protected:
  static const SchemaDocument *kCommandSchema;
  static const SchemaDocument *kResultSchema;

  // Initialize the schema documents.
  static void SetUpTestCase() {
    kCommandSchema =
      GetSchemaProvider().GetRemoteDocument("file:command.schema.json");

    ASSERT_TRUE(kCommandSchema) << "Failed to load command.schema.json";

    kResultSchema =
      GetSchemaProvider().GetRemoteDocument("file:result.schema.json");

    ASSERT_TRUE(kResultSchema) << "Failed to load result.schema.json";
  }

  static void ValidateCommand(Document *doc, const std::string &json) {
    SCOPED_TRACE("Validating JSON against Command Schema");
    Validate(doc, kCommandSchema, json);
  }
  static void ValidateResult(Document *doc, const std::string &json) {
    SCOPED_TRACE("Validating JSON against Result Schema");
    Validate(doc, kResultSchema, json);
  }

  ~DispatchTest() override = default;

  void ExpectError(const std::string &json_result) {
    ValidateResult(&result_, json_result);
    EXPECT_FALSE(result_["success"].GetBool());  // Expect Error.
    
    // Expect some message.
    auto it = result_.FindMember("message");
    EXPECT_TRUE(it->value.IsString());
    EXPECT_GT(it->value.GetStringLength(), 0);
  }

  Document doc_;
  Document result_;
};

const SchemaDocument *DispatchTest::kCommandSchema;
const SchemaDocument *DispatchTest::kResultSchema;

TEST_F(DispatchTest, InvalidJSON) {
  std::string not_valid_json = "This should cause an error";

  auto res = Dispatch(not_valid_json);

  EXPECT_TRUE(res.first.empty());  // No return loc.
  ExpectError(res.second);
}

TEST_F(DispatchTest, TopLevelValueIsNotAnObject) {
  std::string top_level_array = R"([ { "error": true } ])";

  auto res = Dispatch(top_level_array);

  EXPECT_TRUE(res.first.empty());  // No return loc.
  ExpectError(res.second);
}

TEST_F(DispatchTest, MissingReturnLocation) {
  std::string missing_return_loc =
    R"({ "command": "get_next", "params": { "frame_id": 15 } })";

  auto res = Dispatch(missing_return_loc);

  EXPECT_TRUE(res.first.empty());  // No return loc.
  ExpectError(res.second);
}

TEST_F(DispatchTest, MissingCommand) {
  std::string missing_command =
    R"({ "params": { "frame_id": 15 }, "return_loc": "test" })";

  auto res = Dispatch(missing_command);

  ExpectError(res.second);
}

TEST_F(DispatchTest, MissingParams) {
  std::string missing_params =
    R"({ "command": "get_next", "return_loc": "test" })";

  auto res = Dispatch(missing_params);

  EXPECT_EQ(res.first, "test");
  ExpectError(res.second);
}

TEST_F(DispatchTest, InvalidCommandType) {
  std::string bad_command = R"({ 
                                 "command": "doesnt_exist",
                                 "params": { "frame_id": 15 },
                                 "return_loc": "test"
                               })";
  auto res = Dispatch(bad_command);

  EXPECT_EQ(res.first, "test");
  ExpectError(res.second);
}

TEST_F(DispatchTest, GetNextBadParams) {
  std::string bad_params = R"({
                                "command": "get_next",
                                "params": { "not_a_property": true },
                                "return_loc": "test"
                              })";

  auto res = Dispatch(bad_params);

  EXPECT_EQ(res.first, "test");
  ExpectError(res.second);
}

// TODO: Update this test case when GetNext is implemented for real.
TEST_F(DispatchTest, GetNextWorks) {
  std::string get_next_command = R"({
                                      "command": "get_next",
                                      "params": {
                                        "world_name": "test_world",
                                        "frame_id": 15
                                      },
                                      "return_loc": "test"
                                    })";
  ValidateCommand(&doc_, get_next_command);

  auto res = Dispatch(get_next_command);

  EXPECT_EQ(res.first, "test");
  
  ValidateResult(&result_, res.second);
  EXPECT_TRUE(result_["success"].GetBool());
  EXPECT_THAT(result_["world_name"].GetString(), StrEq("test_world"));
}

TEST_F(DispatchTest, GetFrameBadParams) {
  std::string bad_params = R"({
                                "command": "get_frame",
                                "params": { "this_is_valid": false },
                                "return_loc": "test"
                              })";
  auto res = Dispatch(bad_params);

  EXPECT_EQ(res.first, "test");
  ExpectError(res.second);
}

// TODO: Update this test case when GetFrame is implemented for real.
TEST_F(DispatchTest, GetFrameWorks) {
  std::string get_frame_command = R"({
                                       "command": "get_frame",
                                       "params": {
                                          "frame_id": 12,
                                          "world_name": "test_world"
                                        },
                                        "return_loc": "test"
                                      })";
  ValidateCommand(&doc_, get_frame_command);

  auto res = Dispatch(get_frame_command);

  EXPECT_EQ(res.first, "test");

  ValidateResult(&result_, res.second);
  EXPECT_TRUE(result_["success"].GetBool());
}

TEST_F(DispatchTest, NewFrameBadParams) {
  std::string bad_params = R"({
                                "command": "new_frame",
                                "params": { "this_isnt_valid": true },
                                "return_loc": "test"
                              })";
  auto res = Dispatch(bad_params);

  EXPECT_EQ(res.first, "test");
  ExpectError(res.second);
}

// TODO: Update this test case when NewFrame is implemented for real.
TEST_F(DispatchTest, NewFrameWorks) {
  std::string new_frame_command = R"({
                                       "command": "new_frame",
                                       "params": {
                                          "world_name": "test_world"
                                        },
                                        "return_loc": "test"
                                      })";
  ValidateCommand(&doc_, new_frame_command);

  auto res = Dispatch(new_frame_command);

  EXPECT_EQ(res.first, "test");

  ValidateResult(&result_, res.second);
  EXPECT_TRUE(result_["success"].GetBool());
}

TEST_F(DispatchTest, NewWorldBadParams) {
  std::string bad_params = R"({
                                "command": "new_world",
                                "params": { "this_is_broken": true },
                                "return_loc": "test"
                              })";
  auto res = Dispatch(bad_params);

  EXPECT_EQ(res.first, "test");
  ExpectError(res.second);
}

// TODO: Update this test case when NewWorld is implemented for real.
TEST_F(DispatchTest, NewWorldWorks) {
  std::string new_world_command = R"({
                                       "command": "new_world",
                                       "params": {
                                          "world_name": "new_world"
                                        },
                                        "return_loc": "test"
                                      })";
  ValidateCommand(&doc_, new_world_command);

  auto res = Dispatch(new_world_command);

  EXPECT_EQ(res.first, "test");

  ValidateResult(&result_, res.second);
  EXPECT_TRUE(result_["success"].GetBool());
}
}  // anonymous namespace
}  // namespace execution
