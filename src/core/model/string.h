#ifndef NS3_STRING_H
#define NS3_STRING_H

#include <string>
#include "attribute-helper.h"

namespace ns3 {

/**
 * \ingroup attribute
 *
 * \class ns3::StringValue
 * \brief hold variables of type string
 *
 * This class can be used to hold variables of type string,
 * that is, either char * or std::string.
 */

ATTRIBUTE_VALUE_DEFINE_WITH_NAME (std::string, String);
ATTRIBUTE_ACCESSOR_DEFINE (String);
ATTRIBUTE_CHECKER_DEFINE (String);

} // namespace ns3


#endif /* NS3_STRING_H */
