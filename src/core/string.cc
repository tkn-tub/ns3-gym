#include "string.h"

namespace ns3 {

ATTRIBUTE_CHECKER_IMPLEMENT_WITH_NAME (String, "std::string");
ATTRIBUTE_CONVERTER_IMPLEMENT (String);
ATTRIBUTE_VALUE_IMPLEMENT_WITH_NAME (std::string, String);

} // namespace ns3
