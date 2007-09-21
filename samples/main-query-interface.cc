/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2007 University of Washington
 * Authors:  Tom Henderson, Craig Dowell
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

#include "ns3/log.h"
#include "ns3/object.h"
#include "ns3/component-manager.h"

using namespace ns3;

//
// This sample file shows examples of how to use QueryInterface.
//
// QueryInterface is a templated method of class Object, defined in 
// src/core/object.h.  ns-3 objects that derive from class Object
// can have QueryInterface invoked on them.
//
// QueryInterface is a type-safe way to ask an object, at run-time, 
// "Do you support the interface identified by the given InterfaceId?"  
// It avoids deprecated techniques of having to downcast pointers to 
// an object to ask questions about its type.  One or more interfaces
// may be associated with a given object.
// 
// QueryInterface is of most use when working with base class
// pointers of objects that may be subclassed.  For instance,
// one may have a pointer to a Node, but not know whether it has
// an IPv4 stack.  Another example might be to determine whether
// a Node has an EnergyModel, to which calls to decrement energy
// from the node's battery might be made.
//


//
// Object is the base class for ns-3 node-related objects used at 
// the public API.  Object provides reference counting implementations
// and the QueryInterface.
// 
// A common design paradigm for an ns-3 node object, such as a Queue,
// is that we provide an abstract base class that inherits from
// Object.  This class is assigned an interface ID (iid) and 
// contains the basic API for objects in this class and subclasses.
// This base class is specialized to provide implementations of
// the object in question (such as a DropTailQueue).
// 
// The design pattern commonly used is known as the "non-virtual
// public interface" pattern, whereby the public API for this
// object is a set of public non-virtual functions that forward
// to private virtual functions.  The forwarding functions can
// impose pre- and post-conditions on the forwarding call at
// the base class level.
// 
// We'll call this base class "AnInterface" in the example below.
//
// 
class AnInterface : public Object
{
public:
  static const InterfaceId iid;
  void methodA (void);
private:
  virtual void domethodA (void) = 0;
};

void
AnInterface::methodA (void)
{
  NS_LOG_FUNCTION;
  // pre-dispatch asserts
  NS_LOG_LOGIC ("pre-condition");
  domethodA ();
  NS_LOG_LOGIC ("post-condition");
  // post-dispatch asserts
}

//
// The below assignment assigns the InterfaceId of the class AnInterface,
// and declares that the parent iid is that of class Object.
//
const InterfaceId AnInterface::iid = MakeInterfaceId ("AnInterface", Object::iid);

//
// AnImplementation is an implementation of the abstract base class
// defined above. It provides implementation for the virtual functions 
// in the base class.  It defines one ClassId for each constructor, 
// and can also provide an interface itself (in this example, 
// a methodImpl is available)
// 
class AnImplementation : public AnInterface
{
public:
  static const InterfaceId iid;
  static const ClassId cid;

  AnImplementation ();
  void methodImpl (void);
private:
  virtual void domethodA (void);
};

void
AnImplementation::methodImpl (void)
{
  NS_LOG_FUNCTION;
}


AnImplementation::AnImplementation (void)
{
  NS_LOG_FUNCTION;
  // enable our interface
  SetInterfaceId (AnImplementation::iid);
}

void
AnImplementation::domethodA () 
{
  NS_LOG_FUNCTION;
}

//
// The below assignment assigns the InterfaceId of the class AnImplementation,
// and declares that the parent iid is that of class Object.
//
const InterfaceId AnImplementation::iid = 
  MakeInterfaceId ("AnImplementation", AnInterface::iid);

//
// The next few lines are used by the component manager.  They
// state that the component manager can create a new object 
// AnImplementation and return an interface corresponding to 
// the AnImplementation iid.
//
const ClassId AnImplementation::cid = 
  MakeClassId<AnImplementation> 
  ("AnImplementation", AnImplementation::iid);


//
// Extending interfaces
// ==================
// What if AnInterface doesn't provide enough API for your
// object type?
// - if you aren't concerned about backward compatibility and
//   don't mind recompiling, you just add new methods to AnInterface
//   and recompile.
// - if you want to address backward compatibiliy, or allow part
//   of the system to use the old interface, you have to do more.
//   You have to declare a new interface with the new functionality.
//

class AnExtendedInterface : public AnInterface
{
public:
  static const InterfaceId iid;
  void methodB (void);
private:
  virtual void domethodB (void) = 0;
};

const InterfaceId AnExtendedInterface::iid = 
  MakeInterfaceId ("AnExtendedInterface", AnInterface::iid);

//
// Then you need provide an implementation for the virtual 
// methods.  If you are providing a new implementation for 
// everything, the answer is straightforward
//

class ANewImplementation : public AnExtendedInterface
{
public:
  static const InterfaceId iid;
  static const ClassId cid;

  ANewImplementation ();
  void methodImpl (void);
private:
  virtual void domethodA (void) { /* new-implementation-behavior ();  */}
  virtual void domethodB (void) { /* new-implementation-behavior (); */}
};

ANewImplementation::ANewImplementation (void)
{
  // enable our interface
  SetInterfaceId (ANewImplementation::iid);
}

void
ANewImplementation::methodImpl (void)
{
  NS_LOG_FUNCTION;
}

const InterfaceId ANewImplementation::iid = 
  MakeInterfaceId ("ANewImplementation", AnExtendedInterface::iid);

//
// If you want to extend an existing implementation, you can use 
// the existing class to instantiate an implementation of its 
// methods (hasa) and do the following if you can use stuff from 
// the existing class.
//

class AnExtendedImplementation : public AnExtendedInterface
{
public:
  static const InterfaceId iid;
  static const ClassId cid;

  AnExtendedImplementation ();
  void methodImpl (void) { /* pImpl->methodImpl (); */ }
  void methodExtendedImpl (void);
private:
  virtual void domethodA (void) { /* new-implementation-behavior (); */}
  virtual void domethodB (void) { /* new-implementation-behavior (); */}
  Ptr<AnImplementation> pImpl;
};

AnExtendedImplementation::AnExtendedImplementation (void)
{
  pImpl = Create<AnImplementation> (); 
  SetInterfaceId (AnExtendedImplementation::iid);
}

void
AnExtendedImplementation::methodExtendedImpl (void)
{
  NS_LOG_FUNCTION;
}

const InterfaceId AnExtendedImplementation::iid = 
  MakeInterfaceId ("AnExtendedImplementation", AnExtendedInterface::iid);

//
// Inheriting from an existing implementation (isa) and an extended
// interface is tricky, because of the diamond multiple inheritance
// problem.  If the pImpl method above is not desirable, it may
// be that the implementation extension could be aggregated.
// 
// The extension will not have access to the base implementation,
// so this design pattern may be more appropriate if the extension
// is very modular (e.g., add an EnergyModel to a wireless interface)
//
// EXAMPLE NOT YET PROVIDED

int main (int argc, char *argv[])
{

  Ptr<AnInterface> aBase = ComponentManager::Create<AnImplementation> 
    (AnImplementation::cid, AnInterface::iid);
  NS_ASSERT (aBase != 0);

  aBase->methodA ();
  //aBase->methodImpl (); // XXX won't compile, aBase not right ptr type
  
  Ptr<AnImplementation> aBaseImplPtr = 
    aBase-> QueryInterface<AnImplementation> (AnImplementation::iid);
  aBaseImplPtr->methodImpl ();
  aBaseImplPtr->methodA();

  // Test symmetric property of QueryInterface 
  Ptr<AnInterface> aBase2 = 
    aBaseImplPtr-> QueryInterface<AnInterface> (AnInterface::iid);
  aBase2->methodA ();

  return 0;
}
