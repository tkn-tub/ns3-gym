#ifndef CONFIG_H
#define CONFIG_H

#include "attribute.h"
#include "ptr.h"
#include "object.h"
#include <string>

namespace ns3 {

namespace Config {

void Set (std::string path, Attribute value);
void SetDefault (std::string name, Attribute value);
void SetGlobal (std::string name, Attribute value);
void Connect (std::string path, const CallbackBase &cb);
void Disconnect (std::string path, const CallbackBase &cb);
void ConnectWithContext (std::string path, const CallbackBase &cb);

void RegisterRootNamespaceObject (Ptr<Object> obj);
void UnregisterRootNamespaceObject (Ptr<Object> obj);

} // namespace Config

} // namespace ns3

#endif /* CONFIG_H */
