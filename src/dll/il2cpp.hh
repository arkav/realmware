#pragma once

#include <cstdint>
#include <string>
namespace rw::il {
struct __attribute__((aligned(8))) _String_Fields {
  int32_t len;
  uint16_t firstChar;
};
struct String {
  void *klass;
  void *monitor;
  _String_Fields fields;
};

std::string copy_string_utf8(String *str);
} // namespace rw::il
