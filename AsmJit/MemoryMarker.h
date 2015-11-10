
#ifndef _ASMJIT_MEMORYMARKER_H
#define _ASMJIT_MEMORYMARKER_H

#include "Build.h"
#include "Defs.h"

#include "ApiBegin.h"  // [Api-Begin]

namespace AsmJit {

struct ASMJIT_API MemoryMarker
{
  MemoryMarker() ASMJIT_NOTHROW;
  virtual ~MemoryMarker() ASMJIT_NOTHROW;
  //
  virtual void mark(const void* ptr, sysuint_t size) ASMJIT_NOTHROW = 0;

private:
  ASMJIT_DISABLE_COPY(MemoryMarker)
};

} // AsmJit namespace

#include "ApiEnd.h"    // [Api-End]

#endif // _ASMJIT_MEMORYMARKER_H
