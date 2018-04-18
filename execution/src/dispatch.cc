#include "execution/impl/dispatch.h"

#include <sstream>
#include <string>
#include <vector>
#include <unordered_map>
#include <utility>
#include <memory>

#include "rapidjson/document.h"
#include "rapidjson/error/en.h"
#include "rapidjson/rapidjson.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/writer.h"

#include "execution/interfaces/world.h"
#include "execution/interfaces/cell.h"
#include "execution/interfaces/frame.h"
#include "execution/impl/simple_world.h"
#include "execution/impl/simple_cell.h"
#include "execution/impl/hash_map_frame.h"

#include "ObjDispatcher.hpp"

using rapidjson::Document;
using rapidjson::GetParseError_En;
using rapidjson::StringBuffer;
using rapidjson::Value;
using rapidjson::Writer;
using rapidjson::kArrayType;
using rapidjson::kObjectType;

using ol::ObjDispatcher;

namespace execution {
// This anonymous namespace acts like an internal API largely wrapping
// rapidjson. In the future, the wrapper to rapidjson may be moved from this
// location to it's own source/header files an treated like an internal library.
namespace {
// Useful typedefs for working with rapidjson.
using Object = Value::Object;
using Array = Value::Array;

std::unordered_map<std::string, std::unique_ptr<WorldInterface>> &WorldMap() {
  static auto *map =
    new std::unordered_map<std::string, std::unique_ptr<WorldInterface>>;
  return *map;
}

// Internal type for easer management of results.
class CommandResult {
 public:
  CommandResult() = default;
  // Disallow Copy and Assign.
  CommandResult(const CommandResult &) = delete;
  CommandResult &operator=(const CommandResult &) = delete;
  // Pull in default move constructor/assignment.
  CommandResult(CommandResult &&) = default;
  CommandResult &operator=(CommandResult &&) = default;

  // NOTE: Treating non-const access of members as setting the member.
  //       THIS IS LAZY but should work.

  // World name accessors.
  const std::string &world_name() const { return world_name_; }
  std::string &world_name() { has_world_name_ = true; return world_name_; }

  // Frame id accessors.
  int frame_id() const { return frame_id_; }
  int &frame_id() { has_frame_id_ = true; return frame_id_; }

  // Alive Cells accessors.
  const std::vector<std::pair<int, int>> &alive_cells() const {
    return alive_cells_;
  }
  std::vector<std::pair<int, int>> &alive_cells() {
    has_alive_cells_ = true;
    return alive_cells_;
  }

  // State accessors.
  bool HasWorldName() const { return has_world_name_; }
  bool HasFrameId() const { return has_frame_id_; }
  bool HasAliveCells() const { return has_alive_cells_; }

 private:
  // Data members to be packed into json response.
  std::string world_name_;
  int frame_id_;
  std::vector<std::pair<int, int>> alive_cells_;
  // Wheter or not each data member's value has been set.
  bool has_world_name_ = false;
  bool has_frame_id_ = false;
  bool has_alive_cells_ = false;
};

// Allows typename -> string conversions for more useful error messages.
template <typename T> struct AsString;
// Primitive specializations.
template <>
struct AsString<bool> {
  static constexpr char value[] = "bool";
};
template <>
struct AsString<int> {
  static constexpr char value[] = "int";
};
template <>
struct AsString<unsigned> {
  static constexpr char value[] = "unsigned";
};
template <>
struct AsString<int64_t> {
  static constexpr char value[] = "int64_t";
};
template <>
struct AsString<uint64_t> {
  static constexpr char value[] = "uint64_t";
};
template <>
struct AsString<double>  {
  static constexpr char value[] = "double";
};
template <>
struct AsString<float> {
  static constexpr char value[] = "float";
};
// C-String Specialization.
template <>
struct AsString<const char *> {
  static constexpr char value[] = "string";
};
// C++ String Specialization.
template <typename Ch>
struct AsString<std::basic_string<Ch>> {
  static constexpr char value[] = "string";
};

// Storage for above values.
constexpr char AsString<bool>::value[];
constexpr char AsString<int>::value[];
constexpr char AsString<unsigned>::value[];
constexpr char AsString<int64_t>::value[];
constexpr char AsString<uint64_t>::value[];
constexpr char AsString<double>::value[];
constexpr char AsString<float>::value[];
constexpr char AsString<const char*>::value[];
template <typename Ch> constexpr char AsString<std::basic_string<Ch>>::value[];

// Generating Utility Methods

// Serializes any JSON value to a C++ string.
std::string Serialize(const Value &val) {
  StringBuffer output;
  Writer<StringBuffer> writer(output);
  val.Accept(writer);

  return std::string(output.GetString(), output.GetLength());
}

// For some errors, return loc is not always known.
// Returns (return_loc, json error response).
std::pair<std::string, std::string> BuildErrorResponse(
    const std::stringstream &error_stream, const std::string &return_loc = "") {
  Document error_doc(kObjectType);
  
  {
    auto &allocator = error_doc.GetAllocator();

    error_doc.AddMember("success", false, allocator);
    error_doc.AddMember("message", error_stream.str(), allocator);
  }

  return make_pair(return_loc, Serialize(error_doc));
}

// Returns (return_loc, json result).
std::pair<std::string, std::string> BuildResponse(const std::string &return_loc,
                                                  const CommandResult &result) {
  Document doc(kObjectType);

  {
    auto &allocator = doc.GetAllocator();

    doc.AddMember("success", true, allocator);

    if (result.HasFrameId())
      doc.AddMember("frame_id", result.frame_id(), allocator);
    if (result.HasWorldName())
      doc.AddMember("world_name", result.world_name(), allocator);

    if (result.HasAliveCells()) {
      Value live_cells(kArrayType);
      for (const auto &cell_coord : result.alive_cells()) {
        Value point(kObjectType);
        point.AddMember("x", cell_coord.first, allocator);
        point.AddMember("y", cell_coord.second, allocator);

        live_cells.PushBack(point, allocator);
      }
      doc.AddMember("live_cells", live_cells, allocator);
    }
  }

  return make_pair(return_loc, Serialize(doc));
}

// Parsing Utility Methods

// Gets a member from a rapidjson::Object (possibly destructive).
// Will assign an error string to err if property is missing, or if types
// mismatch.
template <typename T>
T GetMember(Object obj, const char *prop_name, std::stringstream *err) {
  auto it = obj.FindMember(prop_name);
  if (it == obj.MemberEnd() || !it->value.Is<T>()) {
    (*err) << "Expected " << AsString<T>::value << " value for property '"
           << prop_name << "'";

    return T();
  }

  return it->value.Get<T>();
}
// Same for above, but for Object and Array. Return type is Value instead of
// Object or Array to avoid memory issues. For the same reason, this method also
// needs an allocator for constructing new values.
// Error handling is the same as above.
std::unique_ptr<Value> GetObjectMember(Object obj, const char *prop_name,
                                       std::stringstream *err,
                                       Value::AllocatorType &allocator) {
  auto it = obj.FindMember(prop_name);
  if (it == obj.MemberEnd() || !it->value.IsObject()) {
    (*err) << "Expected object value for property '" << prop_name << "'";

    return std::unique_ptr<Value>(new Value);  // Default constructd to Null.
  }

  // Move semantics by default.
  return std::unique_ptr<Value>(new Value(it->value, allocator));
}
std::unique_ptr<Value> GetArrayMember(Object obj, const char *prop_name,
                                      std::stringstream *err,
                                      Value::AllocatorType &allocator) {
  auto it = obj.FindMember(prop_name);
  if (it == obj.MemberEnd() || !it->value.IsArray()) {
    (*err) << "Expected array value for property '" << prop_name << "'";

    return std::unique_ptr<Value>(new Value);  // Default constructed to Null.
  }

  // Move semantics by default.
  return std::unique_ptr<Value>(new Value(it->value, allocator));
}

// Generic Helpers

// Returns true iff error messages have been written to err_stream.
bool HasErrorMessage(std::stringstream *err_stream) {
  // A stream is considered to have error messages if any content at all has
  // been written to it.
  return err_stream->tellp() > 0;
}

// Routines for specific command types

std::unordered_map<std::string, std::unique_ptr<WorldInterface>>::iterator
InsertWorld(const std::string &name,
    std::unordered_map<std::string, std::unique_ptr<WorldInterface>> &map) {
  static auto *cell_factory = new SimpleCellFactory;
  static auto *obj_dispatcher = new ObjDispatcher;

  auto frame_factory = std::unique_ptr<FrameFactoryInterface>(
      new HashMapFrameFactory);
  frame_factory->SetCellFactory(cell_factory);
  auto res = map.emplace(name, std::unique_ptr<WorldInterface>(
      new SimpleWorld(name.c_str(), std::move(frame_factory), obj_dispatcher)));

  return res.first;
}

// Given a frame id, it returns the subsequent frame.
// TODO: Replace this bogus implementation with the real deal.
CommandResult ProcessGetNext(Object params, std::stringstream *err,
                             Value::AllocatorType &) {
  CommandResult result;

  auto prev_frame_id = GetMember<int>(params, "frame_id", err);
  if (HasErrorMessage(err)) return result;

  auto world_name = GetMember<std::string>(params, "world_name", err);
  if (HasErrorMessage(err)) return result;

  auto &map = WorldMap();
  auto entry = map.find(world_name);
  if (entry == map.end()) {
    entry = InsertWorld(world_name, map);
  }

  auto *world = entry->second.get();
  auto *next_frame = world->GetFrameAfter(prev_frame_id);
  if (!next_frame) {
    (*err) << "No such predecessor by id " << prev_frame_id;
    return result;
  }

  result.world_name() = world_name;
  result.frame_id() = next_frame->GetId();
  result.alive_cells() = next_frame->LivingCells();

  return result;
}

// TODO: Consider optimizing all command result usage with std::move.

// Given a frame id, get it's cells.
// TODO: Implement the logic of this call.
CommandResult ProcessGetFrame(Object params, std::stringstream *err,
                              Value::AllocatorType &) {
  CommandResult result;

  auto frame_id = GetMember<int>(params, "frame_id", err);
  if (HasErrorMessage(err)) return result;

  auto world_name = GetMember<std::string>(params, "world_name", err);
  if (HasErrorMessage(err)) return result;

  auto &map = WorldMap();
  auto entry = map.find(world_name);
  if (entry == map.end()) {
    entry = InsertWorld(world_name, map);
  }

  auto *world = entry->second.get();
  auto *frame = world->GetFrameById(frame_id);
  if (!frame) {
    (*err) << "No such frame by id " << frame_id;
    return result;
  }

  result.frame_id() = frame_id;
  result.world_name() = world_name;
  result.alive_cells() = frame->LivingCells();

  return result;
}

CommandResult ProcessNewFrame(Object params, std::stringstream *err,
                              Value::AllocatorType &allocator) {
  CommandResult result;

  auto frame_id = GetMember<int>(params, "frame_id", err);
  if (HasErrorMessage(err)) return result;

  auto world_name = GetMember<std::string>(params, "world_name", err);
  if (HasErrorMessage(err)) return result;

  auto live_cells = GetArrayMember(params, "live_cells", err, allocator);
  if (HasErrorMessage(err)) return result;

  std::vector<std::pair<int, int>> cell_list;
  for (auto it = live_cells->Begin(); it != live_cells->End(); ++it) {
    auto x = GetMember<int>(it->GetObject(), "x", err);
    if (HasErrorMessage(err)) return result;
    auto y = GetMember<int>(it->GetObject(), "y", err);
    if (HasErrorMessage(err)) return result;

    cell_list.emplace_back(x, y);
  }

  auto &map = WorldMap();
  auto entry = map.find(world_name);
  if (entry == map.end()) {
    entry = InsertWorld(world_name, map);
  }

  auto *world = entry->second.get();
  auto *frame = world->MakeFrameWithIdFromCells(frame_id + 1,
                                                std::move(cell_list));

  result.world_name() = world_name;
  result.frame_id() = frame->GetId();
  result.alive_cells() = frame->LivingCells();
  
  return result;
}

CommandResult ProcessNewWorld(Object params, std::stringstream *err,
                              Value::AllocatorType &) {
  CommandResult result;

  auto world_name = GetMember<std::string>(params, "world_name", err);
  if (HasErrorMessage(err)) return result;

  auto &map = WorldMap();
  auto entry = InsertWorld(world_name, map);
  
  auto *world = entry->second.get();
  auto *frame = world->GetBlankFrame(0);

  // Assuming the above works.
  result.world_name() = world_name;
  result.frame_id() = frame->GetId();
  result.alive_cells() = frame->LivingCells();

  return result;
}

// typedef for command processor function type.
using CommandProcessor = CommandResult (*)(Object, std::stringstream *,
                                           Value::AllocatorType &);
// Retrieves a reference to the static command router map.
const std::unordered_map<std::string, CommandProcessor> &GetCommandRouter() {
  static const auto *command_router =
    new std::unordered_map<std::string, CommandProcessor>({
        { "get_next", &ProcessGetNext },
        { "get_frame", &ProcessGetFrame },
        { "new_frame", &ProcessNewFrame },
        { "new_world", &ProcessNewWorld },
      });

  return *command_router;
}
}  // anonymous namespace

// This method takes any JSON command string from the GUI, preforms the
// specified action, and returns a pair (return pipe name, JSON result).
// RapdidJSON is currently used at the JSON parsing/generation engine.
// NOTE: If the command was quit, return value is ("", "quit").
std::pair<std::string, std::string> Dispatch(const std::string &json_command) {
  Document command_doc;
  auto &allocator = command_doc.GetAllocator();
  // Parse the json command with all default flags.
  command_doc.Parse(json_command.c_str());

  // Stores any generated errors.
  std::stringstream err_stream;

  if (command_doc.HasParseError()) {
    const char *json_err_str = GetParseError_En(command_doc.GetParseError());
    err_stream << "JSON Parse Error: " << json_err_str
               << "\n  Ocurred at offset " << command_doc.GetErrorOffset()
               << " in:\n  " << json_command;
    return BuildErrorResponse(err_stream);
  }
  if (!command_doc.IsObject()) {
    err_stream << "Expected a top-level object";
    return BuildErrorResponse(err_stream);
  }

  Object command_obj = command_doc.GetObject();

  auto command = GetMember<std::string>(command_obj, "command", &err_stream);
  if (HasErrorMessage(&err_stream)) 
    return BuildErrorResponse(err_stream);

  if (command == "quit")
    return std::make_pair("", "quit");

  auto return_loc = GetMember<std::string>(command_obj, "return_loc",
                                           &err_stream);
  if (HasErrorMessage(&err_stream))
    return BuildErrorResponse(err_stream);

  auto params = GetObjectMember(command_obj, "params", &err_stream, allocator);
  if (HasErrorMessage(&err_stream))
    return BuildErrorResponse(err_stream, return_loc);
  
  // Route the command and process it.
  const auto &router = GetCommandRouter();
  auto entry = router.find(command);
  CommandResult result;
  if (entry != router.end()) {
    result = entry->second(params->GetObject(), &err_stream, allocator);
  } else {
    err_stream << "Command '" << command << "' not recognized";
  }

  if (HasErrorMessage(&err_stream))
    return BuildErrorResponse(err_stream, return_loc);

  return BuildResponse(return_loc, result);
}
}  // namespace execution
