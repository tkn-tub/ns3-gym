/* -*- Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2009 University of Washington
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
 */

#ifndef OBJECT_NAMES_H
#define OBJECT_NAMES_H

#include "ns3/ptr.h"
#include "ns3/object.h"

namespace ns3 {

/**
 * \brief A directory of name and Ptr<Object> associations that allows us to
 * give any ns3 Object a name.
 */
class Names
{
public:

  /**
   * Add the association between the string "name" and the Ptr<Object> obj
   * at the root of the "/Names" name space.  This can be seen as equivalent
   * to adding a Pointer Attribute called "name" to the to the root name 
   * space object and then assigning the value obj to that attribute.  The 
   * config facility will see it that way.
   *
   * \param name The name of the object you want to associate.
   * \param obj A smart pointer to the object itself.
   */
  static bool Add (std::string name, Ptr<Object> obj);

  /**
   * Add the association between the string "name" and the Ptr<Object> obj
   * in the object context given by the Ptr<Object> context.  This can be
   * seen as equivalent to adding a Pointer Attribute called "name" to the 
   * object given by "context" and then assigning the value obj to that
   * attribute.  The config facility will see it that way.
   *
   * \param context A spart pointer to an object under which you want this
   *                name to be defined.
   * \param name The name of the object you want to associate.
   * \param obj A smart pointer to the object itself.
   */
  static bool Add (Ptr<Object> context, std::string name, Ptr<Object> object);

  /**
   * Syntactic sugar around the Object context Name method.  Allows you to 
   * specify the context with a string instead of the pointer.  If the first
   * parameter (context) is "/Names" this turns into a call into Name at the
   * root of the name space.  Otherwise it does a FindObjectFromFullNameInternal
   * on the context and adds the name to a subspace.
   *
   * \param context A fully qualified name describing a previously named object.
   *                under which you want this name to be defined.
   * \param name The name of the object you want to associate.
   * \param obj A smart pointer to the object itself.
   */
  static bool Add (std::string context, std::string name, Ptr<Object> object);

  /**
   * Given a pointer to an object, look to see if that object has a name
   * associated with it and return the shortname for the object.
   *
   * The fullname of an object is a fully qualified namespace name, for example
   * if you have a device that you have previously named "eth0" under a node
   * you have named "client", the fullname of the device will then be
   * "/Names/client/eth0".
   *
   * The shortname of an object is the name of the object in its parent name
   * space.  Using the example above, asking for the shortname of the device
   * will result in "eth0" being returned.
   *
   * \param object A spart pointer to an object for which you want to find
   *               its shortname.
   */
  static std::string FindShortName (Ptr<Object> object);

  /**
   * Given a pointer to an object, look to see if that object has a name
   * associated with it and return the fully qualified namespace name
   * for the object.
   *
   * The fullname of an object is a fully qualified namespace name, for example
   * if you have a device that you have previously named "eth0" under a node
   * you have named "client", the fullname of the device will then be
   * "/Names/client/eth0".
   *
   * The shortname of an object is the name of the object in its parent name
   * space.  Using the example above, asking for the shortname of the device
   * will result in "eth0" being returned.
   *
   * \param object A spart pointer to an object for which you want to find
   *               its fullname.
   */
  static std::string FindFullName (Ptr<Object> object);

  /**
   * Given a fullname string, look to see if there's an object in the system
   * with a that associated with it.  If there is, do a QueryObject on the 
   * resulting object to convert it to the requested typename.  
   * 
   * The fullname of an object is a fully qualified namespace name, for example
   * if you have a device that you have previously named "eth0" under a node
   * you have named "client", the fullname of the device will then be
   * "/Names/client/eth0".
   *
   * \param name A string containing a fully qualified name space name 
   *             used to locate the object.
   */
  template <typename T>
  static Ptr<T> FindObjectFromFullName (std::string name);

  /**
   * Given a fullname string, look to see if there's an object in the system
   * with a that associated with it.  If there is, do a QueryObject on the 
   * resulting object to convert it to the requested typename.  
   * 
   * The fullname of an object is a fully qualified namespace name, for example
   * if you have a device that you have previously named "eth0" under a node
   * you have named "client", the fullname of the device will then be
   * "/Names/client/eth0".
   *
   * \param name A string containing a fully qualified name space name 
   *             used to locate the object.
   *
   * @comment This method is identical to FindObjectFromFullName, but has a
   * short signature since it is a common use and we want it to be easy to 
   * type.
   */
  template <typename T>
  static Ptr<T> Find (std::string name);

  /**
   * Given an object context and a shortname string, look through the names 
   * associated with the namespace defined by the context object to see if 
   * there's an object there with the given shortname.
   *
   * The fullname of an object is a fully qualified namespace name, for example
   * if you have a device that you have previously named "eth0" under a node
   * you have named "client", the fullname of the device will then be
   * "/Names/client/eth0".
   *
   * The shortname of an object is the name of the object in its parent name
   * space.  Using the example above, asking for the shortname of the device
   * will result in "eth0" being returned.
   *
   * The context object provides a namespace context, in the case of the example
   * it would be the "client" object under which we look for the short name.
   * In the example above, the context pointer would be the Ptr<Object> to the 
   * client node, and the name would be the shortname "eth0"  
   *
   * \param context A spart pointer to an object under which you want to look 
   *                for the provided name.
   * \param name A string containing a shortname to look for.
   */
  template <typename T>
  static Ptr<T> FindObjectFromShortName (Ptr<Object> context, std::string name);

private:

  /**
   * \internal
   *
   * \brief Non-templated internal version of FindObjectFromLongName
   *
   * \param name A string containing a longname to look for.
   */
  static Ptr<Object> FindObjectFromFullNameInternal (std::string name);

  /**
   * \internal
   *
   * \brief Non-templated internal version of FindObjectFromShortName
   *
   * \param context A spart pointer to an object under which you want to look 
   *                for the provided name.
   * \param name A string containing a shortname to look for.
   */
  static Ptr<Object> FindObjectFromShortNameInternal (Ptr<Object> context, std::string name);
};

/**
 * \brief Template definition of corresponding template declaration found in class Names.
 */
template <typename T>
Ptr<T> 
Names::Find (std::string name)
{
  return FindObjectFromFullName<T> (name);
}

/**
 * \brief Template definition of corresponding template declaration found in class Names.
 */
template <typename T>
Ptr<T> 
Names::FindObjectFromFullName (std::string name)
{
  Ptr<Object> obj = FindObjectFromFullNameInternal (name);
  if (obj)
    {
      return obj->GetObject<T> ();
    }
  else
    {
      return 0;
    }
}

/**
 * \brief Template definition of corresponding template declaration found in class Names.
 */
template <typename T>
Ptr<T> 
Names::FindObjectFromShortName (Ptr<Object> context, std::string name)
{
  Ptr<Object> obj = FindObjectFromShortNameInternal (context, name);
  if (obj)
    {
      return obj->GetObject<T> ();
    }
  else
    {
      return 0;
    }
}

}//namespace ns3

#endif /* OBJECT_NAMES_H */
