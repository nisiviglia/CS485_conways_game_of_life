#ifndef EXECUTION_IMPL_DISPATCH_H_
#define EXECUTION_IMPL_DISPATCH_H_

#include <string>
#include <utility>

namespace execution {
// The dispatch method takes a json record, processes it, and generates a
// pair (return pipe name, JSON result string).
std::pair<std::string, std::string> Dispatch(const std::string &json_command);
}  // namespace execution;

#endif  // EXECUTION_IMPL_DISPATCH_H_
