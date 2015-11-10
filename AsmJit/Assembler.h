
#ifndef _ASMJIT_ASSEMBLER_H
#define _ASMJIT_ASSEMBLER_H

#include "Build.h"

namespace AsmJit {

struct Logger;
struct MemoryManager;
struct EInstruction;

}

#if defined(ASMJIT_X86) || defined(ASMJIT_X64)
#include "AssemblerX86X64.h"
#endif

#endif // _ASMJIT_ASSEMBLER_H
