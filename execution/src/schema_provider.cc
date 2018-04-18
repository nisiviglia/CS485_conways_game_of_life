#include "execution/tests/schema_provider.h"

#include <cstdio>
#include <cstring>
#include <utility>

#include "rapidjson/filereadstream.h"

using rapidjson::FileReadStream;
using rapidjson::SchemaDocument;
using rapidjson::SizeType;

namespace execution {
const SchemaDocument *LocalSchemaProvider::GetRemoteDocument(const char *uri,
                                                             SizeType len) {
  // A local schema reference should start with file:
  if (len < 5 || memcmp(uri, "file:", 5)) {
    return nullptr;
  }

  // Look in cache first, and return if found.
  std::string filename(uri + 5, len - 5);
  auto entry = schema_docs_.find(filename);
  if (entry != schema_docs_.end()) {
    return entry->second.get();
  }

  // Try to read schema from file.
  std::string fullpath = REPO_ROOT_DIR "json/";
  fullpath += filename;
  
  FILE *fp = fopen(fullpath.c_str(), "r");
  if (!fp) return nullptr;

  FileReadStream stream(fp, buff_, sizeof(buff_));
  doc_.ParseStream(stream);
  if (doc_.HasParseError()) return nullptr;
  fclose(fp);

  // Create new cache entry, and return value.
  std::unique_ptr<SchemaDocument> schema_doc(new SchemaDocument(doc_, this));
  SchemaDocument *ret = schema_doc.get();
  schema_docs_.emplace(filename, std::move(schema_doc));

  return ret;
}

const SchemaDocument *LocalSchemaProvider::GetRemoteDocument(
    const std::string &uri) {
  return GetRemoteDocument(uri.c_str(), uri.size());
}
}  // namespace execution
