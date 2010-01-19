/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
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

#ifndef ASCII_TRACE_HELPER_H
#define ASCII_TRACE_HELPER_H

#include "ns3/net-device-container.h"
#include "ns3/node-container.h"
#include "ns3/simulator.h"
#include "ns3/output-stream-object.h"

namespace ns3 {

/**
 * \brief Manage ASCII trace files for device models
 *
 * Handling ascii trace files is a common operation for ns-3 devices.  It is 
 * useful to provide a common base class for dealing with these ops.
 */

class AsciiTraceHelper
{
public:
  /**
   * @brief Create an ascii trace helper.
   */
  AsciiTraceHelper ();

  /**
   * @brief Destroy an ascii trace helper.
   */
  ~AsciiTraceHelper ();

  /**
   * @brief Let the ascii trace helper figure out a reasonable filename to use
   * for the ascii trace file.
   */
  std::string GetFilename (std::string prefix, Ptr<NetDevice> device, bool useObjectNames = true);

  /**
   * @brief Create and initialize an output stream object we'll use to write the 
   * traced bits.
   *
   * One of the common issues users run into when trying to use tracing in ns-3
   * is actually a design decision made in the C++ stream library.  It is not 
   * widely known that copy and assignment of iostreams is forbidden by 
   * std::basic_ios<>.  This is because it is not possible to predict the 
   * semantics of the stream desired by a user.
   *
   * The tempting ns-3 idiom when tracing to a file is to create a bound callback
   * with an ofstream as the bound object.  Unfortunately, this implies a copy 
   * construction in order to get the ofstream object into the callback.  This 
   * operation, as mentioned above, is forbidden by the STL.  You could use a 
   * global ostream and pass a pointer to it, but that is pretty ugly.  You 
   * could create an ostream on the stack and pass a pointer to it, but you may
   * run into object lifetime issues.  Ns-3 has a nice reference counted object
   * that can solve the problem so we use one of those to carry the stream
   * around and deal with the lifetime issues.
   */
  Ptr<OutputStreamObject> CreateFileStream (std::string filename, std::string filemode);

  /**
   * @brief Hook a trace source to the default enqueue operation trace sink
   */
  template <typename T> void HookDefaultEnqueueSink (Ptr<T> object, std::string traceName, Ptr<OutputStreamObject> stream);

  /**
   * @brief Hook a trace source to the default drop operation trace sink
   */
  template <typename T> void HookDefaultDropSink (Ptr<T> object, std::string traceName, Ptr<OutputStreamObject> stream);

  /**
   * @brief Hook a trace source to the default dequeue operation trace sink
   */
  template <typename T> void HookDefaultDequeueSink (Ptr<T> object, std::string traceName, Ptr<OutputStreamObject> stream);

  /**
   * @brief Hook a trace source to the default receive operation trace sink
   */
  template <typename T> void HookDefaultReceiveSink (Ptr<T> object, std::string traceName, Ptr<OutputStreamObject> stream);

private:
  static void DefaultEnqueueSink (Ptr<OutputStreamObject> file, Ptr<const Packet> p);
  static void DefaultDropSink (Ptr<OutputStreamObject> file, Ptr<const Packet> p);
  static void DefaultDequeueSink (Ptr<OutputStreamObject> file, Ptr<const Packet> p);
  static void DefaultReceiveSink (Ptr<OutputStreamObject> file, Ptr<const Packet> p);
};

template <typename T> void
AsciiTraceHelper::HookDefaultEnqueueSink (Ptr<T> object, std::string tracename, Ptr<OutputStreamObject> file)
{
  object->TraceConnectWithoutContext (tracename.c_str (), MakeBoundCallback (&DefaultEnqueueSink, file));
}

template <typename T> void
AsciiTraceHelper::HookDefaultDropSink (Ptr<T> object, std::string tracename, Ptr<OutputStreamObject> file)
{
  object->TraceConnectWithoutContext (tracename.c_str (), MakeBoundCallback (&DefaultDropSink, file));
}

template <typename T> void
AsciiTraceHelper::HookDefaultDequeueSink (Ptr<T> object, std::string tracename, Ptr<OutputStreamObject> file)
{
  object->TraceConnectWithoutContext (tracename.c_str (), MakeBoundCallback (&DefaultDequeueSink, file));
}

template <typename T> void
AsciiTraceHelper::HookDefaultReceiveSink (Ptr<T> object, std::string tracename, Ptr<OutputStreamObject> file)
{
  object->TraceConnectWithoutContext (tracename.c_str (), MakeBoundCallback (&DefaultReceiveSink, file));
}

} // namespace ns3

#endif /* ASCIITRACE_HELPER_H */
