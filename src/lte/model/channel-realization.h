/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2010 TELEMATICS LAB, DEE - Politecnico di Bari
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
 * Author: Giuseppe Piro  <g.piro@poliba.it>
 */

#ifndef CHANNEL_REALIZATION_H
#define CHANNEL_REALIZATION_H


#include <ns3/nstime.h>
#include "ns3/object.h"
#include "jakes-fading-loss-model.h"
#include "path-loss-model.h"
#include "shadowing-loss-model.h"
#include "penetration-loss-model.h"


namespace ns3 {


/**
 * \ingroup lte
 *
 * \brief the ChannelRealization class implements a complete propagation model
 * used by the channel to compute the loss due to the propagation of the signal.
 * A ChannelRealization object is created for each couple of UE - eNB.
 */
class ChannelRealization : public Object
{

public:
  ChannelRealization ();
  virtual ~ChannelRealization ();

  static TypeId GetTypeId (void);

  /**
   * \brief Set the multipath loss model
   * \param l the multipath loss model
   */
  void SetJakesFadingLossModel (Ptr<JakesFadingLossModel> l);
  /**
   * \brief Set the path loss model
   * \param l the path loss model
   */
  void SetPathLossModel (Ptr<PathLossModel> l);
  /**
   * \brief Set the shadowing loss model
   * \param l the shadowing loss model
   */
  void SetShadowingLossModel (Ptr<ShadowingLossModel> l);
  /**
   * \brief Set the penetration loss model
   * \param l the penetration loss model
   */
  void SetPenetrationLossModel (Ptr<PenetrationLossModel> l);


  /**
   * \brief Get the multipath loss model
   * \return  the pointer to the multipath loss model
   */
  Ptr<JakesFadingLossModel> GetJakesFadingLossModel (void);
  /**
   * \brief Get the path loss model
   * \return  the pointer to the path loss model
   */
  Ptr<PathLossModel> GetPathLossModel (void);
  /**
   * \brief Get the shadowing loss model
   * \return  the pointer to the shadowing loss model
   */
  Ptr<ShadowingLossModel> GetShadowingLossModel (void);
  /**
   * \brief Get the penetration loss model
   * \return  the pointer to the penetration loss model
   */
  Ptr<PenetrationLossModel> GetPenetrationLossModel (void);


private:
  Ptr<ShadowingLossModel> m_shadowing;
  Ptr<PathLossModel> m_pathloss;
  Ptr<JakesFadingLossModel> m_multipath;
  Ptr<PenetrationLossModel> m_penetration;

};


} // namespace ns3

#endif /* CHANNEL_REALIZATION_H */


