/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 *  Copyright 2013. Lawrence Livermore National Security, LLC.
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
 * Author: Steven Smith <smith84@llnl.gov>
 *
 */

#ifndef NS3_REMOTE_CHANNEL_BUNDLE_MANAGER
#define NS3_REMOTE_CHANNEL_BUNDLE_MANAGER

#include <ns3/nstime.h>
#include <ns3/ptr.h>
#include <map>

namespace ns3 {

class RemoteChannelBundle;

/*
 * \ingroup mpi
 * 
 * \brief Singleton for managing the RemoteChannelBundles for each process.
 *
 * Manages collective tasks associated with the bundle collection.
 */
class RemoteChannelBundleManager
{

public:
  /**
   * \return remote channel bundle for specified SystemId.
   */
  static Ptr<RemoteChannelBundle> Find (uint32_t systemId);

  /**
   * Add RemoteChannelBundle from this task to MPI task on other side of the link.
   * Can not be invoked after InitializeNullMessageEvents has been invoked.
   */
  static Ptr<RemoteChannelBundle> Add (uint32_t systemId);

  /**
   * \return number of remote channel bundles
   * 
   */
  static uint32_t Size (void);

  /**
   * Setup initial Null Message events for every RemoteChannelBundle.
   * All RemoteChannelBundles should be added before this method is invoked.
   */
  static void InitializeNullMessageEvents (void);

  /**
   * \return safe time across all remote channels.
   */
  static Time GetSafeTime (void);

  /**
   * Destroy the singleton.
   */
  static void Destroy (void);

private:

  /**
   * Private ctor to prevent creation outside of singleton pattern.
   */
  RemoteChannelBundleManager ()
  {
  }

  ~RemoteChannelBundleManager ()
  {
  }

  /*
   * Container for all remote channel bundles for this task.
   *
   * Would be more efficient to use unordered_map when C++11 is adopted for NS3.
   */
  typedef std::map<uint32_t, Ptr<RemoteChannelBundle> > RemoteChannelMap;
  static RemoteChannelMap g_remoteChannelBundles;

  /*
   * Protect manager class from being initialized twice or incorrect
   * ordering of method calls.
   */
  static bool g_initialized;
};

} // namespace ns3

#endif
