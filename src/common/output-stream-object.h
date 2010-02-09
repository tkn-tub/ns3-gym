/* -*- Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2010 University of Washington
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

#ifndef OUTPUT_STREAM_OBJECT_H
#define OUTPUT_STREAM_OBJECT_H

#include <ostream>
#include "ns3/object.h"
#include "ns3/ptr.h"
#include "ns3/simple-ref-count.h"

namespace ns3 {

/*
 * @brief A class encapsulating an STL output stream.
 *
 * One of the common issues users run into when trying to use tracing in ns-3
 * is actually due to a design decision made in the C++ stream library.  
 *
 * It is not widely known, but copy and assignment of iostreams is forbidden by 
 * std::basic_ios<>.  This is because it is not possible to predict the 
 * semantics of the stream desired by a user.
 *
 * When writing traced information to a file, the tempting ns-3 idiom is to 
 * create a bound callback with an ofstream as the bound object.  Unfortunately,
 * this implies a copy construction in order to get the ofstream object into the
 * callback.  This operation, as mentioned above, is forbidden by the STL.  One
 * could use a global ostream and pass a pointer to it, but that is pretty ugly.  
 * One could also create an ostream on the stack and pass a pointer to it in the
 * callback, but object lifetime issues will quickly rear their ugly heads.  It
 * turns out that ns-3 has a nifty reference counted object that can solve the 
 * value semantics and object lifetime issues, so we provide one of those to 
 * carry the stream around.
 *
 * The experienced C++ wizard may ask, "what's the point of such a simple class."
 * but this isn't oriented toward them.  We use this to make a simple thing 
 * simple to do for new users; and also as a simple example of ns-3 objects at a
 * high level.  We know of a couple of high-powered experienced developers that 
 * have been tripped up by this and decided that it must be an ns-3 bug.
 *
 * One could imagine having this object inherit from stream to get the various
 * overloaded operator<< defined, but we're going to be using a
 * Ptr<OutputStreamObject> when passing this object around.  In this case, the 
 * Ptr<> wouldn't understand the operators and we would have to dereference it
 * to access the underlying object methods.  Since we would have to dereference
 * the Ptr<>, we don't bother and just expect the user to Get a saved pointer
 * to an ostream and dereference it him or herself.  As in:
 *
 * \verbatim
 *   void 
 *   TraceSink (Ptr<OutputStreamObject> streamObject, Ptr<const Packet> packet)
 *   {
 *     std::ostream *stream = streamObject->GetStream ();
 *     *stream << "got packet" << std::endl;
 *   }
 * \endverbatim
 */
class OutputStreamObject : public SimpleRefCount<OutputStreamObject>
{
public:
  static TypeId GetTypeId (void);

  OutputStreamObject ();
  ~OutputStreamObject ();

  void SetStream (std::ostream *ostream);
  std::ostream *GetStream (void);
  
private:
  std::ostream *m_ostream;
};

} //namespace ns3

#endif // OUTPUT_STREAM_OBJECT_H
