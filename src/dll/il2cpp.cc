#include "il2cpp.hh"

#include <codecvt>
#include <locale>

namespace rw::il {
std::string copy_string_utf8(String *str) {
  std::u16string u16(reinterpret_cast<const char16_t *>(&str->fields.firstChar));
  return std::wstring_convert<std::codecvt_utf8<char16_t>, char16_t>{}.to_bytes(u16);
} // namespace rw::il
} // namespace rw::il
