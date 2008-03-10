#ifndef CONFIG_H
#define CONFIG_H

#include "attribute.h"
#include "ptr.h"
#include "object.h"
#include <string>

namespace ns3 {

namespace Config {

/**
 * \param path a path to match attributes.
 * \param value the value to set in all matching attributes.
 *
 * This function will attempt to find attributes which
 * match the input path and will then set their value to the input
 * value.
 */
void Set (std::string path, Attribute value);
/**
 * \param name the full name of the attribute
 * \param value the value to set.
 *
 * This method overrides the initial value of the 
 * matching attribute. This method cannot fail: it will
 * crash if the input attribute name or value is invalid.
 */
void SetDefault (std::string name, Attribute value);
/**
 * \param name the full name of the attribute
 * \param value the value to set.
 * \returns true if the value was set successfully, false otherwise.
 *
 * This method overrides the initial value of the 
 * matching attribute. 
 */
bool SetDefaultFailSafe (std::string name, Attribute value);
/**
 * \param name the name of the requested GlobalValue.
 * \param value the value to set
 *
 * This method is equivalent to GlobalValue::Bind
 */
void SetGlobal (std::string name, Attribute value);
/**
 * \param name the name of the requested GlobalValue.
 * \param value the value to set
 *
 * This method is equivalent to GlobalValue::BindFailSafe
 */
bool SetGlobalFailSafe (std::string name, Attribute value);
/**
 * \param path a path to match trace sources.
 * \param cb the callback to connect to the matching trace sources.
 *
 * This function will attempt to find all trace sources which
 * match the input path and will then connect the input callback
 * to them.
 */
void Connect (std::string path, const CallbackBase &cb);
/**
 * \param path a path to match trace sources.
 * \param cb the callback to disconnect to the matching trace sources.
 *
 * This function undoes the work of Config::Connect.
 */
void Disconnect (std::string path, const CallbackBase &cb);
/**
 * \param path a path to match trace sources.
 * \param cb the callback to connect to the matching trace sources.
 *
 * This function will attempt to find all trace sources which
 * match the input path and will then connect the input callback
 * to them in such a way that the callback will receive an extra
 * context string upon trace event notification.
 */
void ConnectWithContext (std::string path, const CallbackBase &cb);
/**
 * \param path a path to match trace sources.
 * \param cb the callback to connect to the matching trace sources.
 *
 * This function undoes the work of Config::ConnectWithContext.
 */
void DisconnectWithContext (std::string path, const CallbackBase &cb);

/**
 * \param obj a new root object
 *
 * Each root object is used during path matching as
 * the root of the path by Config::Connect, and Config::Set.
 */
void RegisterRootNamespaceObject (Ptr<Object> obj);
/**
 * \param obj a new root object
 *
 * This function undoes the work of Config::RegisterRootNamespaceObject.
 */
void UnregisterRootNamespaceObject (Ptr<Object> obj);

} // namespace Config

} // namespace ns3

#endif /* CONFIG_H */
