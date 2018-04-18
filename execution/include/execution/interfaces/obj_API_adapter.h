#ifndef objApiAdapter_h
#define objApiAdapter_h

#include "frame.h"
#include "../../../../exec-obj/interface/DTOs/FrameDTO.h"


class ObjApiAdapter {

 public
  virtual FrameDTO* toFrameDTO(FrameInterface* frame) = 0;
  virtual FrameInterface* fromFrameDTO(FrameDTO* frameDTO) = 0;
};

#endif
