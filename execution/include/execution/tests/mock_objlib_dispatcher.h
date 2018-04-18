#ifndef EXECUTION_TESTS_MOCK_OBJLIB_DISPATCHER_H_
#define EXECUTION_TESTS_MOCK_OBJLIB_DISPATCHER_H_

#include "gmock/gmock.h"
#include "ObjDispatcher.hpp"

namespace ol {

  class MockObjDispatcher : public ObjDispatcher {

   public:
    ~MockObjDispatcher() override = default;

    MOCK_METHOD5(Single, int(Ops, Obj_Type, int, std::string, std::vector<std::pair<int, int>>*));
  };
}


#endif