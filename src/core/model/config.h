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
#include <vector>

/**
 * \file
 * \ingroup config
 * Declaration of the various ns3::Config functions and classes.
 */

namespace ns3 {

class AttributeValue;
class Object;
class CallbackBase;

/**
 * \ingroup core
 * \defgroup config Configuration
 * \brief Configuration of simulation parameters and tracing.
 */

/**
 * \ingroup config
 * Namespace for the various functions implementing the Config system.
 */
namespace Config {

/**
 * \ingroup config
 * Reset the initial value of every attribute as well as the value of every
 * global to what they were before any call to SetDefault and SetGlobal.
 */
void Reset (void);

/**
 * \ingroup config
 * \param path a path to match attributes.
 * \param value the value to set in all matching attributes.
 *
 * This function will attempt to find attributes which
 * match the input path and will then set their value to the input
 * value.
 */
void Set (std::string path, const AttributeValue &value);
/**
 * \ingroup config
 * \param name the full name of the attribute
 * \param value the value to set.
 *
 * This method overrides the initial value of the 
 * matching attribute. This method cannot fail: it will
 * crash if the input attribute name or value is invalid.
 */
void SetDefault (std::string name, const AttributeValue &value);
/**
 * \ingroup config
 * \param name the full name of the attribute
 * \param value the value to set.
 * \returns true if the value was set successfully, false otherwise.
 *
 * This method overrides the initial value of the 
 * matching attribute. 
 */
bool SetDefaultFailSafe (std::string name, const AttributeValue &value);
/**
 * \ingroup config
 * \param name the name of the requested GlobalValue.
 * \param value the value to set
 *
 * This method is equivalent to GlobalValue::Bind
 */
void SetGlobal (std::string name, const AttributeValue &value);
/**
 * \ingroup config
 * \param name the name of the requested GlobalValue.
 * \param value the value to set
 * \return \c true if the GlobalValue could be set.
 *
 * This method is equivalent to GlobalValue::BindFailSafe
 */
bool SetGlobalFailSafe (std::string name, const AttributeValue &value);
/**
 * \ingroup config
 * \param path a path to match trace sources.
 * \param cb the callback to connect to the matching trace sources.
 *
 * This function will attempt to find all trace sources which
 * match the input path and will then connect the input callback
 * to them.
 */
void ConnectWithoutContext (std::string path, const CallbackBase &cb);
/**
 * \ingroup config
 * \param path a path to match trace sources.
 * \param cb the callback to disconnect to the matching trace sources.
 *
 * This function undoes the work of Config::Connect.
 */
void DisconnectWithoutContext (std::string path, const CallbackBase &cb);
/**
 * \ingroup config
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
 * \ingroup config
 * \param path a path to match trace sources.
 * \param cb the callback to connect to the matching trace sources.
 *
 * This function undoes the work of Config::ConnectWithContext.
 */
void Disconnect (std::string path, const CallbackBase &cb);

/**
 * \ingroup config
 * \brief hold a set of objects which match a specific search string.
 *
 * This class also allows you to perform a set of configuration operations
 * on the set of matching objects stored in the container. Specifically,
 * it is possible to perform bulk Connects and Sets.
 */
class MatchContainer
{
public:
  /** Const iterator over the objects in this container. */
  typedef std::vector<Ptr<Object> >::const_iterator Iterator;
  MatchContainer ();
  /**
   * Constructor used only by implementation.
   *
   * \param [in] objects The vector of objects to store in this container.
   * \param [in] contexts The corresponding contexts.
   * \param [in] path The path used for object matching.
   */
  MatchContainer (const std::vector<Ptr<Object> > &objects, 
                  const std::vector<std::string> &contexts, 
                  std::string path);

  /**
   * \returns an iterator which points to the first item in the container
   */
  MatchContainer::Iterator Begin (void) const;
  /**
   * \returns an iterator which points to the last item in the container
   */
  MatchContainer::Iterator End (void) const;
  /**
   * \returns the number of items in the container
   */
  uint32_t GetN (void) const;
  /**
   * \param i index of item to lookup ([0,n[)
   * \returns the item requested.
   */
  Ptr<Object> Get (uint32_t i) const;
  /**
   * \param i index of item to lookup ([0,n[)
   * \returns the fully-qualified matching path associated
   *          to the requested item.
   *
   * The matching patch uniquely identifies the requested object.
   */
  std::string GetMatchedPath (uint32_t i) const;
  /**
   * \returns the path used to perform the object matching.
   */
  std::string GetPath (void) const;

  /**
   * \param name name of attribute to set
   * \param value value to set to the attribute
   *
   * Set the specified attribute value to all the objects stored in this
   * container.
   * \sa ns3::Config::Set
   */
  void Set (std::string name, const AttributeValue &value);
  /**
   * \param name the name of the trace source to connect to
   * \param cb the sink to connect to the trace source
   *
   * Connect the specified sink to all the objects stored in this
   * container.
   * \sa ns3::Config::Connect
   */
  void Connect (std::string name, const CallbackBase &cb);
  /**
   * \param name the name of the trace source to connect to
   * \param cb the sink to connect to the trace source
   *
   * Connect the specified sink to all the objects stored in this
   * container.
   * \sa ns3::Config::ConnectWithoutContext
   */
  void ConnectWithoutContext (std::string name, const CallbackBase &cb);
  /**
   * \param name the name of the trace source to disconnect from
   * \param cb the sink to disconnect from the trace source
   *
   * Disconnect the specified sink from all the objects stored in this
   * container.
   * \sa ns3::Config::Disconnect
   */
  void Disconnect (std::string name, const CallbackBase &cb);
  /**
   * \param name the name of the trace source to disconnect from
   * \param cb the sink to disconnect from the trace source
   *
   * Disconnect the specified sink from all the objects stored in this
   * container.
   * \sa ns3::Config::DisconnectWithoutContext
   */
  void DisconnectWithoutContext (std::string name, const CallbackBase &cb);
  
private:
  /** The list of objects in this container. */
  std::vector<Ptr<Object> > m_objects;
  /** The context for each object. */
  std::vector<std::string> m_contexts;
  /** The path used to perform the object matching. */
  std::string m_path;
};

/**
 * \ingroup config
 * \param path the path to perform a match against
 * \returns a container which contains all the objects which match the input
 *          path.
 */
MatchContainer LookupMatches (std::string path);

/**
 * \ingroup config
 * \param obj a new root object
 *
 * Each root object is used during path matching as
 * the root of the path by Config::Connect, and Config::Set.
 */
void RegisterRootNamespaceObject (Ptr<Object> obj);
/**
 * \ingroup config
 * \param obj a new root object
 *
 * This function undoes the work of Config::RegisterRootNamespaceObject.
 */
void UnregisterRootNamespaceObject (Ptr<Object> obj);

/**
 * \ingroup config
 * \returns the number of registered root namespace objects.
 */
uint32_t GetRootNamespaceObjectN (void);

/**
 * \ingroup config
 * \param i the index of the requested object.
 * \returns the requested root namespace object
 */
Ptr<Object> GetRootNamespaceObject (uint32_t i);

} // namespace Config

} // namespace ns3

#endif /* CONFIG_H */
