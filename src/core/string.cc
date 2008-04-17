#include "string.h"

namespace ns3 {

ATTRIBUTE_CHECKER_IMPLEMENT (String);
ATTRIBUTE_CONVERTER_IMPLEMENT (String);
ATTRIBUTE_VALUE_IMPLEMENT_WITH_NAME (std::string, String);

} // namespace ns3
