/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2008 INRIA
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation;
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * Authors: Mathieu Lacage <mathieu.lacage@sophia.inria.fr>
 */
#ifndef CONFIG_H
#define CONFIG_H

#include "ptr.h"
#include <string>

namespace ns3 {

class AttributeValue;
class Object;
class CallbackBase;

namespace Config {

/**
 * \param path a path to match attributes.
 * \param value the value to set in all matching attributes.
 *
 * This function will attempt to find attributes which
 * match the input path and will then set their value to the input
 * value.
 */
void Set (std::string path, const AttributeValue &value);
/**
 * \param name the full name of the attribute
 * \param value the value to set.
 *
 * This method overrides the initial value of the 
 * matching attribute. This method cannot fail: it will
 * crash if the input attribute name or value is invalid.
 */
void SetDefault (std::string name, const AttributeValue &value);
/**
 * \param name the full name of the attribute
 * \param value the value to set.
 * \returns true if the value was set successfully, false otherwise.
 *
 * This method overrides the initial value of the 
 * matching attribute. 
 */
bool SetDefaultFailSafe (std::string name, const AttributeValue &value);
/**
 * \param name the name of the requested GlobalValue.
 * \param value the value to set
 *
 * This method is equivalent to GlobalValue::Bind
 */
void SetGlobal (std::string name, const AttributeValue &value);
/**
 * \param name the name of the requested GlobalValue.
 * \param value the value to set
 *
 * This method is equivalent to GlobalValue::BindFailSafe
 */
bool SetGlobalFailSafe (std::string name, const AttributeValue &value);
/**
 * \param path a path to match trace sources.
 * \param cb the callback to connect to the matching trace sources.
 *
 * This function will attempt to find all trace sources which
 * match the input path and will then connect the input callback
 * to them.
 */
void ConnectWithoutContext (std::string path, const CallbackBase &cb);
/**
 * \param path a path to match trace sources.
 * \param cb the callback to disconnect to the matching trace sources.
 *
 * This function undoes the work of Config::Connect.
 */
void DisconnectWithoutContext (std::string path, const CallbackBase &cb);
/**
 * \param path a path to match trace sources.
 * \param cb the callback to connect to the matching trace sources.
 *
 * This function will attempt to find all trace sources which
 * match the input path and will then connect the input callback
 * to them in such a way that the callback will receive an extra
 * context string upon trace event notification.
 */
void Connect (std::string path, const CallbackBase &cb);
/**
 * \param path a path to match trace sources.
 * \param cb the callback to connect to the matching trace sources.
 *
 * This function undoes the work of Config::ConnectWithContext.
 */
void Disconnect (std::string path, const CallbackBase &cb);

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

/**
 * \returns the number of registered root namespace objects.
 */
uint32_t GetRootNamespaceObjectN (void);

/**
 * \param i the index of the requested object.
 * \returns the requested root namespace object
 */
Ptr<Object> GetRootNamespaceObject (uint32_t i);

} // namespace Config

} // namespace ns3

#endif /* CONFIG_H */
