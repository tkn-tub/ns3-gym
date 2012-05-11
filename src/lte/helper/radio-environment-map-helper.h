/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2012 Centre Tecnologic de Telecomunicacions de Catalunya (CTTC)
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
 * Author: Nicola Baldo <nbaldo@cttc.es>
 */


#ifndef RADIO_ENVIRONMENT_MAP_HELPER_H
#define RADIO_ENVIRONMENT_MAP_HELPER_H


#include <ns3/object.h>
#include <fstream>


namespace ns3 {

class RemSpectrumPhy;
class Node;
class NetDevice;
class SpectrumChannel;
class BuildingsMobilityModel;

/** 
 * Generates a 2D map of the SINR from the strongest transmitter in the downlink of an LTE FDD system.
 * 
 */
class RadioEnvironmentMapHelper : public Object
{
public:  

  RadioEnvironmentMapHelper ();
  virtual ~RadioEnvironmentMapHelper ();
  
  // inherited from Object
  virtual void DoDispose (void);
  static TypeId GetTypeId (void);

  /** 
   * \return the bandwidth (in num of RBs) over which SINR is calculated
   */
  uint8_t GetBandwidth () const;

  /** 
   *
   * \param bw  the bandwidth (in num of RBs) over which SINR is calculated
   */
  void SetBandwidth (uint8_t bw);

  /** 
   * Deploy the RemSpectrumPhy objects that generate the map according to the specified settings.
   * 
   */
  void Install ();

private:

  void DelayedInstall ();
  void RunOneIteration (double xMin, double xMax, double yMin, double yMax);
  void PrintAndReset ();
  void Finalize ();


  struct RemPoint 
  {
    Ptr<RemSpectrumPhy> phy;
    Ptr<BuildingsMobilityModel> bmm;
  };

  std::list<RemPoint> m_rem;

  double m_xMin;
  double m_xMax;
  uint16_t m_xRes;
  double m_xStep;

  double m_yMin;
  double m_yMax;
  uint16_t m_yRes;
  double m_yStep;

  uint32_t m_maxPointsPerIteration;

  uint16_t m_earfcn;
  uint16_t m_bandwidth;
 
  double m_z;

  std::string m_channelPath;
  std::string m_outputFile;

  bool m_stopWhenDone;
  
  Ptr<SpectrumChannel> m_channel;

  double m_noisePower;

  std::ofstream m_outFile;

};


}

#endif /* RADIO_ENVIRONMENT_MAP_HELPER_H */
