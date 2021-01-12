#pragma once
#include <MinHook.h>
#include <cstdint>

#define RWHOOK void *__stdcall

namespace rw {
namespace mem {

// uintptr64
typedef unsigned long long ptr_t;
constexpr ptr_t null = 0;
void *void_ptr(ptr_t ptr);

struct module_info_t {
  ptr_t m_begin;
  ptr_t m_end;
};

module_info_t get_module_info(const char *modname);

// wrapper for minhook, just a bit type safe :^)
namespace hook {
template <typename... Args> class detour {
private:
  using FnPtr = void *(__stdcall *)(Args...);
  ptr_t m_src = null;
  void *m_trampoline = nullptr;

public:
  detour() = default;
  ~detour() = default;
  void m_install(ptr_t src, FnPtr dest) {
    m_src = src;
    MH_CreateHook(void_ptr(m_src), reinterpret_cast<void *>(dest), &m_trampoline);
    MH_EnableHook(void_ptr(m_src));
  }
  void m_remove() { MH_RemoveHook(void_ptr(m_src)); };
  FnPtr m_getTrampoline() { return reinterpret_cast<FnPtr>(m_trampoline); }
};
} // namespace hook
} // namespace mem
namespace debug {
bool alloc_console();
} // namespace debug
} // namespace rw
