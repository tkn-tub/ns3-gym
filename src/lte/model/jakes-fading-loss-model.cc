/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
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

#include <cmath>
#include <ns3/log.h>
#include "jakes-fading-loss-model.h"
#include <ns3/simulator.h>
#include <stdint.h>
#include "stdlib.h"
#include "lte-phy.h"
#include <ns3/mobility-model.h>

#include "JakesTraces/multipath_v0_M6.h"
#include "JakesTraces/multipath_v0_M8.h"
#include "JakesTraces/multipath_v0_M10.h"
#include "JakesTraces/multipath_v0_M12.h"

#include "JakesTraces/multipath_v3_M6.h"
#include "JakesTraces/multipath_v3_M8.h"
#include "JakesTraces/multipath_v3_M10.h"
#include "JakesTraces/multipath_v3_M12.h"

#include "JakesTraces/multipath_v30_M6.h"
#include "JakesTraces/multipath_v30_M8.h"
#include "JakesTraces/multipath_v30_M10.h"
#include "JakesTraces/multipath_v30_M12.h"

#include "JakesTraces/multipath_v120_M6.h"
#include "JakesTraces/multipath_v120_M8.h"
#include "JakesTraces/multipath_v120_M10.h"
#include "JakesTraces/multipath_v120_M12.h"


NS_LOG_COMPONENT_DEFINE ("JakesFadingLossModel");

namespace ns3 {


NS_OBJECT_ENSURE_REGISTERED (JakesFadingLossModel);

JakesFadingLossModel::JakesFadingLossModel ()
  : m_nbOfPaths (1, 4),
    m_startJakes (1, 2000),
    m_phy (0)
{
  NS_LOG_FUNCTION (this);
  SetSamplingPeriod (0.5); // default value
}


TypeId
JakesFadingLossModel::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::JakesFadingLossModel")
    .SetParent<DiscreteTimeLossModel> ()
    .AddConstructor<JakesFadingLossModel> ()
  ;
  return tid;
}


JakesFadingLossModel::~JakesFadingLossModel ()
{
  m_phy  = 0;
}


void
JakesFadingLossModel::SetPhy (Ptr<LtePhy> phy)
{
  NS_LOG_FUNCTION (this);
  m_phy = phy;

  SetValue ();
}


Ptr<LtePhy>
JakesFadingLossModel::GetPhy (void)
{
  NS_LOG_FUNCTION (this);
  return m_phy;
}


void
JakesFadingLossModel::SetValue (void)
{
  NS_LOG_FUNCTION (this);

  m_multipath.clear ();

  int downlinkSubChannels = GetPhy ()->GetDownlinkSubChannels ().size ();

  Ptr<MobilityModel> mobility = GetPhy ()->GetDownlinkSpectrumPhy ()->GetMobility ()->GetObject<MobilityModel> ();
  Vector speedVector = mobility->GetVelocity ();

  double speed = sqrt (pow (speedVector.x,2) +  pow (speedVector.y,2));

  NS_LOG_FUNCTION (this << mobility << speedVector << speed);


  /*
   * Several 3GPP standards propose a simulation scenario to use duirng the 
   * LTE performance evaluation. In particular they suggest to consider these
   * user speeds: 0, 3, 30, 120 km/h. To this aim, we should map user speed
   * into one of the suggested values. 
   */
  if (speed < 3.)
    {
      speed = 0;
    }
  else if (speed < 30.)
    {
      speed = 3.;
    }
  else if (speed < 120.)
    {
      speed = 30.;
    }
  else
    {
      speed = 120;
    }

  NS_LOG_FUNCTION (this << mobility << speedVector << speed);


  /*
   * Jackes Model.
   * Jakes popularised a model for Rayleigh fading based on summing sinusoids
   * William C. Jakes, Editor (February 1, 1975).
   * Microwave Mobile Communications.
   * New York: John Wiley & Sons Inc. ISBN 0-471-43720-4
   */

  // number of path = M
  // x = 1 -> M=6, x = 2 -> M=8, x = 3 -> M=10, x = 4 -> M=12
  int x = static_cast<int> (m_nbOfPaths.GetValue ());

  for (int i = 0; i < downlinkSubChannels; i++)
    {
      // StartJakes allow us to select a window of 0.5ms into the Jakes realization lasting 3s.
      int startJakes = static_cast<int> (m_startJakes.GetValue ());

      MultipathForTimeDomain multipathForTimeDomain;

      if (x == 1)
        {
          // SELECTED 6 MULTIPLE PATH FOR JAKES MODEL
          if (speed == 0)
            {
              for (int j = 0; j < 500; j++)
                {
                  multipathForTimeDomain.push_back (multipath_M6_v_0 [j + startJakes]);
                }
            }
          if (speed == 3)
            {
              for (int j = 0; j < 500; j++)
                {
                  multipathForTimeDomain.push_back (multipath_M6_v_3 [j + startJakes]);
                }
            }
          if (speed == 30)
            {
              for (int j = 0; j < 500; j++)
                {
                  multipathForTimeDomain.push_back (multipath_M6_v_30 [j + startJakes]);
                }
            }
          if (speed == 120)
            {
              for (int j = 0; j < 500; j++)
                {
                  multipathForTimeDomain.push_back (multipath_M6_v_120 [j + startJakes]);
                }
            }
        }

      else if (x == 2)
        {
          // SELECTED 6 MULTIPLE PATH FOR JAKES MODEL
          if (speed == 0)
            {
              for (int j = 0; j < 500; j++)
                {
                  multipathForTimeDomain.push_back (multipath_M8_v_0 [j + startJakes]);
                }
            }
          if (speed == 3)
            {
              for (int j = 0; j < 500; j++)
                {
                  multipathForTimeDomain.push_back (multipath_M8_v_3 [j + startJakes]);
                }
            }
          if (speed == 30)
            {
              for (int j = 0; j < 500; j++)
                {
                  multipathForTimeDomain.push_back (multipath_M8_v_30 [j + startJakes]);
                }
            }
          if (speed == 120)
            {
              for (int j = 0; j < 500; j++)
                {
                  multipathForTimeDomain.push_back (multipath_M8_v_120 [j + startJakes]);
                }
            }
        }

      else if (x == 3)
        {
          // SELECTED 6 MULTIPLE PATH FOR JAKES MODEL
          if (speed == 0)
            {
              for (int j = 0; j < 500; j++)
                {
                  multipathForTimeDomain.push_back (multipath_M10_v_0 [j + startJakes]);
                }
            }
          if (speed == 3)
            {
              for (int j = 0; j < 500; j++)
                {
                  multipathForTimeDomain.push_back (multipath_M10_v_3 [j + startJakes]);
                }
            }
          if (speed == 30)
            {
              for (int j = 0; j < 500; j++)
                {
                  multipathForTimeDomain.push_back (multipath_M10_v_30 [j + startJakes]);
                }
            }
          if (speed == 120)
            {
              for (int j = 0; j < 500; j++)
                {
                  multipathForTimeDomain.push_back (multipath_M10_v_120 [j + startJakes]);
                }
            }
        }

      else if (x == 4)
        {
          // SELECTED 6 MULTIPLE PATH FOR JAKES MODEL
          if (speed == 0)
            {
              for (int j = 0; j < 500; j++)
                {
                  multipathForTimeDomain.push_back (multipath_M12_v_0 [j + startJakes]);
                }
            }
          if (speed == 3)
            {
              for (int j = 0; j < 500; j++)
                {
                  multipathForTimeDomain.push_back (multipath_M12_v_3 [j + startJakes]);
                }
            }
          if (speed == 30)
            {
              for (int j = 0; j < 500; j++)
                {
                  multipathForTimeDomain.push_back (multipath_M12_v_30 [j + startJakes]);
                }
            }
          if (speed == 120)
            {
              for (int j = 0; j < 500; j++)
                {
                  multipathForTimeDomain.push_back (multipath_M12_v_120 [j + startJakes]);
                }
            }
        }
      else
        {
          std::cout << " ERROR: Jaks's Model, incorrect M value" << std::endl;
        }


      m_multipath.push_back (multipathForTimeDomain);
    }

  SetLastUpdate ();
}

double
JakesFadingLossModel::GetValue (int subChannel)
{
  NS_LOG_FUNCTION (this);
  if (NeedForUpdate ())
    {
      SetValue ();
      SetLastUpdate ();
    }

  int now_ms = static_cast<int> (Simulator::Now ().GetSeconds () * 1000);
  int lastUpdate_ms = static_cast<int> (GetLastUpdate ().GetSeconds () * 1000);
  int index = now_ms - lastUpdate_ms;

  NS_LOG_FUNCTION (this << subChannel << now_ms
                        << lastUpdate_ms << index << m_multipath.at (subChannel).at (index));

  return m_multipath.at (subChannel).at (index);
}



} // namespace ns3
