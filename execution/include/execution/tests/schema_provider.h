#ifndef EXECUTION_TESTS_SCHEMA_PROVIDER_H_
#define EXECUTION_TESTS_SCHEMA_PROVIDER_H_

#include <map>
#include <string>
#include <memory>

#include "rapidjson/document.h"
#include "rapidjson/rapidjson.h"
#include "rapidjson/schema.h"

namespace execution {
// Fetches schemas from the local filesystem.
// Expects paths to be relative to the json/ directory.
class LocalSchemaProvider : public rapidjson::IRemoteSchemaDocumentProvider {
 public:
  ~LocalSchemaProvider() override = default;

  const rapidjson::SchemaDocument *GetRemoteDocument(
      const char *uri, rapidjson::SizeType len) override;

  // Convenience Method.
  const rapidjson::SchemaDocument *GetRemoteDocument(const std::string &uri);
  
 private:
  rapidjson::Document doc_;
  char buff_[1U << 15];
  std::map<std::string,
    std::unique_ptr<rapidjson::SchemaDocument>> schema_docs_;
};
}  // namespace execution

#endif  // EXECUTION_TESTS_SCHEMA_PROVIDER_H_
