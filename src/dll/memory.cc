#include "memory.hh"

#include "pch.h"

#include <cstdio>
#include <wincon.h>

namespace rw {
namespace mem {
void *void_ptr(ptr_t ptr) { return reinterpret_cast<void *>(ptr); }
WINAPI module_info_t get_module_info(const char *modname) {
  auto begin = (ptr_t)GetModuleHandle(modname);
  MODULEINFO miModInfo;
  GetModuleInformation(GetCurrentProcess(), (HMODULE)begin, &miModInfo, sizeof(MODULEINFO));
  auto end = begin + miModInfo.SizeOfImage;
  return module_info_t{begin, end};
}
} // namespace mem
namespace debug {
bool alloc_console() {
  FreeConsole();
  if (!AllocConsole())
    return false;

  FILE *output = nullptr;
  return (freopen_s(&output, "CONOUT$", "w", stdout) == 0);
}
} // namespace debug
} // namespace rw
