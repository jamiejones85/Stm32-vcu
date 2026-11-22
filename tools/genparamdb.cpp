/*
 * Generator for OpenInverter parameter database (params.json)
 * Built on host at build-time.
 */

#include "errormessage.h"
#include "my_string.h"
#include <iomanip>
#include <iostream>
#include <string>

#define EXPANDED_LIST                                                          \
  ERROR_MESSAGE_ENTRY(NONE, ERROR_DISPLAY) ERROR_MESSAGE_LIST
#define ERROR_MESSAGE_ENTRY(id, type) __COUNTER__ = id,
const char *errorListString = STRINGIFY(EXPANDED_LIST);
#undef ERROR_MESSAGE_ENTRY

#include "param_prj.h"

int main() {
  std::cout << "{\n";
  int index = 0;
  bool first = true;

#define PARAM_ENTRY(cat, name, unit, min, max, def, id)                        \
  if (!first)                                                                  \
    std::cout << ",\n";                                                        \
  first = false;                                                               \
  std::cout << "   \"" #name "\": {\"unit\":"                                  \
            << std::quoted(std::string(unit)) << ",\"id\":" << id              \
            << ",\"value\":" << std::fixed << std::setprecision(2)             \
            << (double)(def) << ",\"isparam\":true"                            \
            << ",\"minimum\":" << std::fixed << std::setprecision(2)           \
            << (double)(min) << ",\"maximum\":" << std::fixed                  \
            << std::setprecision(2) << (double)(max)                           \
            << ",\"default\":" << std::fixed << std::setprecision(2)           \
            << (double)(def)                                                   \
            << ",\"category\":" << std::quoted(std::string(cat))               \
            << ",\"i\":" << index++ << "}";

#define TESTP_ENTRY(cat, name, unit, min, max, def, id)                        \
  PARAM_ENTRY(cat, name, unit, min, max, def, id)

#define VALUE_ENTRY(name, unit, id)                                            \
  if (!first)                                                                  \
    std::cout << ",\n";                                                        \
  first = false;                                                               \
  std::cout << "   \"" #name "\": {\"unit\":"                                  \
            << std::quoted(std::string(unit)) << ",\"id\":" << id              \
            << ",\"value\":0.00"                                               \
            << ",\"isparam\":false"                                            \
            << ",\"minimum\":0.00"                                             \
            << ",\"maximum\":0.00"                                             \
            << ",\"default\":0.00"                                             \
            << ",\"category\":\"\""                                            \
            << ",\"i\":" << index++ << "}";

  PARAM_LIST

#undef PARAM_ENTRY
#undef TESTP_ENTRY
#undef VALUE_ENTRY

  std::cout << "\n}\n";
  return 0;
}
