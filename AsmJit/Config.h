// This file is designed to be modifyable.

#ifndef _ASMJIT_CONFIG_H
#define _ASMJIT_CONFIG_H

// ============================================================================
// [AsmJit - OS]
// ============================================================================

// #define ASMJIT_WINDOWS 1
// #define ASMJIT_POSIX 2

// ============================================================================
// [AsmJit - Architecture]
// ============================================================================

// #define ASMJIT_X86
// #define ASMJIT_X64

#ifdef PROTECT_X64
#define ASMJIT_X64
#else
#define ASMJIT_X86
#endif

// ============================================================================
// [AsmJit - API]
// ============================================================================

// If you are embedding AsmJit library into your project (statically), undef ASMJIT_API macro.
// ASMJIT_HIDDEN macro can contain visibility (used by GCC)
//   to hide some AsmJit symbols that shouldn't be never exported.

// #define ASMJIT_HIDDEN
#define ASMJIT_API         // ◊¢ Õ∫Ûwindows±‡“Îª·¥ÌŒÛ
// #define ASMJIT_NOTHROW  // ASMJIT_NOTHROW marks functions that never throws

// ============================================================================
// [AsmJit - Memory Management]
// ============================================================================

// #define ASMJIT_MALLOC ::malloc
// #define ASMJIT_REALLOC ::realloc
// #define ASMJIT_FREE ::free

// ============================================================================
// [AsmJit - Debug]
// ============================================================================

// #define ASMJIT_DEBUG
// #define ASMJIT_NO_DEBUG
// #define ASMJIT_ASSERT(exp) do { if (!(exp)) ::AsmJit::assertionFailure(__FILE__, __LINE__, #exp); } while(0)

#endif // _ASMJIT_CONFIG_H
