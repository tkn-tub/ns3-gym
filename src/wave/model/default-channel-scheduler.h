/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
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
 * Author: Junling Bu <linlinjavaer@gmail.com>
 */
#ifndef DEFAULT_CHANNEL_SCHEDULER_H
#define DEFAULT_CHANNEL_SCHEDULER_H

#include "wave-net-device.h"
namespace ns3 {
class WaveNetDevice;

/**
 * \ingroup wave
 * \brief This class uses a simple mechanism to assign channel access with following features:
 * (1) only in the context of single-PHY device;
 * (2) FCFS (First come First service) strategy, which seems against the description of
 * the standard (preemptive strategy).
 */
class DefaultChannelScheduler : public ChannelScheduler
{
public:
  static TypeId GetTypeId (void);
  DefaultChannelScheduler ();
  virtual ~DefaultChannelScheduler ();

  /**
   * \param device enable channel scheduler associated with WaveNetDevice
   */
  virtual void SetWaveNetDevice (Ptr<WaveNetDevice> device);
  /**
   * \param channelNumber the specified channel number
   * \return  the type of current assigned channel access for the specific channel.
.  */
  virtual enum ChannelAccess GetAssignedAccessType (uint32_t channelNumber) const;

  void NotifyCchSlotStart (Time duration);
  void NotifySchSlotStart (Time duration);
  void NotifyGuardSlotStart (Time duration, bool cchi);
private:
  virtual void DoInitialize (void);
  virtual void DoDispose (void);
  /**
   * \param channelNumber the specific channel
   * \param immediate indicate whether channel switch to channel
   * \return whether the channel access is assigned successfully
   *
   * This method will assign alternating access for SCHs and CCH.
   */
  virtual bool AssignAlternatingAccess (uint32_t channelNumber, bool immediate);
  /**
   * \param channelNumber the specific channel
   * \param immediate indicate whether channel switch to channel
   * \return whether the channel access is assigned successfully
   *
   * This method will assign continuous SCH access CCH.
   */
  virtual bool AssignContinuousAccess (uint32_t channelNumber, bool immediate);
  /**
   * \param channelNumber the specific channel
   * \param immediate indicate whether channel switch to channel
   * \return whether the channel access is assigned successfully
   *
   * This method will assign extended SCH access for SCHs.
   */
  virtual bool AssignExtendedAccess (uint32_t channelNumber, uint32_t extends, bool immediate);
  /**
   * This method will assign default CCH access for CCH.
   */
  virtual bool AssignDefaultCchAccess (void);
  /**
   * \param channelNumber indicating for which channel should release
   * the assigned channel access resource.
   */
  virtual bool ReleaseAccess (uint32_t channelNumber);
  /**
   * \param curChannelNumber switch from MAC activity for current channel
   * \param nextChannelNumber switch to MAC activity for next channel
   */
  void SwitchToNextChannel (uint32_t curChannelNumber, uint32_t nextChannelNumber);

  Ptr<ChannelManager> m_manager;
  Ptr<ChannelCoordinator> m_coordinator;
  Ptr<WifiPhy> m_phy;

  /**
   *  when m_channelAccess is ContinuousAccess, m_channelNumber
   *   is continuous channel number;
   *  when m_channelAccess is AlternatingAccess, m_channelNumber
   *   is SCH channel number, another alternating channel is CCH;
   *  when m_channelAccess is ExtendedAccess, m_channelNumber
   *   is extended access, extends is the number of extends access.
   *  when m_channelAccess is DefaultCchAccess, m_channelNumber is CCH.
   */
  uint32_t m_channelNumber;
  uint32_t m_extend;
  EventId m_extendEvent;
  enum ChannelAccess m_channelAccess;

  EventId m_waitEvent;
  uint32_t m_waitChannelNumber;
  uint32_t m_waitExtend;

  Ptr<ChannelCoordinationListener> m_coordinationListener;
};

}
#endif /* DEFAULT_CHANNEL_SCHEDULER_H */
