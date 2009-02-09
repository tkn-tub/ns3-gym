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
   * \brief Add the association between the string "name" and the Ptr<Object> obj.
   *
   * The name may begin either with "/Names" to explicitly call out the fact 
   * that the name provided is installed under the root of the name space, 
   * or it may begin with the short name of the first obejct in the path.
   * For example, Add ("/Names/client", obj) and Add ("client", obj) accomplish
   * exactly the same thing.  Names at a given level in the name space must be
   * unique.  It would be illegal to try and associate a different object with
   * the same name: "client."
   *
   * As well as providing a single shortname, the name parameter can contain a 
   * path that qualifies the shortname.  For example, you could make the call
   * Add ("/Names/client/eth0", obj) if you had previously associated a name 
   * "client" in the root of the name space.  Note that Add ("client/eth0", obj)
   * would accomplish exactly the same thing.
   *
   * \param name The name of the object you want to associate.
   * \param obj A smart pointer to the object itself.
   */
  static bool Add (std::string name, Ptr<Object> obj);

  /**
   * \brief Rename a previously associated name.
   *
   * The name may begin either with "/Names" to explicitly call out the fact 
   * that the name provided is installed under the root of the name space, 
   * or it may begin with the short name of the first obejct in the path.
   * For example, Rename ("/Names/client", "newname") and 
   * Rename ("client", "newname") will accomplish exactly the same thing --
   * to find an object at the root of the name space called "client" and change
   * its name to "newname".  Names at a given level in the name space must be
   * unique.  It would be illegal to try and rename the association to another
   * name that already exists at the namespace level indicated by the path.
   *
   * As well as providing a single shortname, the name parameter can contain a 
   * path that qualifies the shortname you want to change.  For example, you 
   * could make the call Rename ("/Names/client/eth0", "ath0") if "ath0" has
   * not been previously used at this level (under "client").  Note that 
   * Rename ("client/eth0", "ath0") would accomplish exactly the same thing.
   *
   * \param oldname The current name of the object you want to change.
   * \param newname The new name of the object you want to change.
   *
   * \returns true if the name change was successfully completed, false otherwise
   */
  static bool Rename (std::string oldname, std::string newname);

  /**
   * \brief An intermediate form of Add allowing you to provide a context in the 
   * form of a name string.
   *
   * \see Add (Ptr<Object> context, std::string name, Ptr<Object> object);
   *
   * \param context A fully qualified name describing a previously named object.
   *                under which you want this name to be defined.
   * \param name The name of the object you want to associate.
   * \param obj A smart pointer to the object itself.
   *
   * \returns true if the association was successfully completed, false otherwise
   */
  static bool Add (std::string context, std::string name, Ptr<Object> object);

  /**
   * \brief An intermediate form of Rename allowing you to provide a context in
   * the form of a name string.
   *
   * \see Rename (Ptr<Object> context, std::string oldname, std::string newname);
   *
   * \param context A fully qualified name describing a previously named object.
   *                under which you want this name to be changed.
   * \param oldname The current shortname of the object you want to change.
   * \param newname The new shortname of the object you want to change.
   *
   * \returns true if the name change was successfully completed, false otherwise
   */
  static bool Rename (std::string context, std::string oldname, std::string newname);

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
   *
   * \returns true if the association was successfully completed, false otherwise
   */
  static bool Add (Ptr<Object> context, std::string name, Ptr<Object> object);

  /**
   * \brief Rename a previously associated shortname.
   *
   * The context for the name -- the namespace level must be provided by passing
   * a context object.  The oldname is then interpreted as a shortname which is
   * defined "under" this context.  For example, if you have previously done an
   * Add ("/Names/client", obj), you have associated the object obj with the
   * name "client."  If you create an "eth0" under "client", then the obj that
   * you associated with "client" becomes the context for "eth0" object.  If 
   * you want to rename "eth0", you would provide that obj as the context for
   * the Rename.  This would look like Rename (obj, "eth0", "ath0").
   *
   * \param context A spart pointer to an object under which to look for the
   *                oldname.
   * \param oldname The current shortname of the object you want to change.
   * \param newname The new shortname of the object you want to change.
   *
   * \returns true if the name change was successfully completed, false otherwise
   */
  static bool Rename (Ptr<Object> context, std::string oldname, std::string newname);

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
   *
   * \returns a string containing the shortname of the object.
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
   *
   * \returns a string containing the fullname of the object.
   */
  static std::string FindFullName (Ptr<Object> object);

  /**
   * Given a fullname string, look to see if there's an object in the system
   * with a that associated with it.  If there is, do a GetObject on the 
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
   * \returns a smart pointer to the named object converted to the requested
   *          type.
   */
  template <typename T>
  static Ptr<T> FindObjectFromFullName (std::string name);

  /**
   * Given a fullname string, look to see if there's an object in the system
   * with a that associated with it.  If there is, do a GetObject on the 
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
   *
   * \returns a smart pointer to the named object converted to the requested
   *          type.
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
   *
   * \returns a smart pointer to the named object converted to the requested
   *          type.
   */
  template <typename T>
  static Ptr<T> FindObjectFromShortName (Ptr<Object> context, std::string name);

  /**
   * Clean up all of the data structures of the implementation and delete the
   * underlying singleton.  Used to get valgrind-clean runs if the simulator
   * is not run.  Normally singleton cleanup is scheduled on Simulator::Destroy.
   */
  static void Delete (void);

private:
  /**
   * \internal
   *
   * \brief Non-templated internal version of FindObjectFromLongName
   *
   * \param name A string containing a longname to look for.
   *
   * \returns a smart pointer to the named object.
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
   *
   * \returns a smart pointer to the named object.
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
