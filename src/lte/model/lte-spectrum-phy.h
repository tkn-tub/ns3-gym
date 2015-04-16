/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2009 CTTC
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
 *         Giuseppe Piro  <g.piro@poliba.it>
 * Modified by: Marco Miozzo <mmiozzo@cttc.es> (introduce physical error model)
 */

#ifndef LTE_SPECTRUM_PHY_H
#define LTE_SPECTRUM_PHY_H

#include <ns3/event-id.h>
#include <ns3/spectrum-value.h>
#include <ns3/mobility-model.h>
#include <ns3/packet.h>
#include <ns3/nstime.h>
#include <ns3/net-device.h>
#include <ns3/spectrum-phy.h>
#include <ns3/spectrum-channel.h>
#include <ns3/spectrum-interference.h>
#include <ns3/data-rate.h>
#include <ns3/generic-phy.h>
#include <ns3/packet-burst.h>
#include <ns3/lte-interference.h>
#include "ns3/random-variable-stream.h"
#include <map>
#include <ns3/ff-mac-common.h>
#include <ns3/lte-harq-phy.h>
#include <ns3/lte-common.h>

namespace ns3 {

struct TbId_t
{
  uint16_t m_rnti;
  uint8_t m_layer;
  
  public:
  TbId_t ();
  TbId_t (const uint16_t a, const uint8_t b);
  
  friend bool operator == (const TbId_t &a, const TbId_t &b);
  friend bool operator < (const TbId_t &a, const TbId_t &b);
};

  
struct tbInfo_t
{
  uint8_t ndi;
  uint16_t size;
  uint8_t mcs;
  std::vector<int> rbBitmap;
  uint8_t harqProcessId;
  uint8_t rv;
  double mi;
  bool downlink;
  bool corrupt;
  bool harqFeedbackSent;
};

typedef std::map<TbId_t, tbInfo_t> expectedTbs_t;


class LteNetDevice;
class AntennaModel;
class LteControlMessage;
struct LteSpectrumSignalParametersDataFrame;
struct LteSpectrumSignalParametersDlCtrlFrame;
struct LteSpectrumSignalParametersUlSrsFrame;


/**
* this method is invoked by the LteSpectrumPhy to notify the PHY that the
* transmission of a given packet has been completed.
*
* @param packet the Packet whose TX has been completed.
*/
typedef Callback< void, Ptr<const Packet> > LtePhyTxEndCallback;

/**
* This method is used by the LteSpectrumPhy to notify the PHY that a
* previously started RX attempt has terminated without success
*/
typedef Callback< void > LtePhyRxDataEndErrorCallback;
/**
* This method is used by the LteSpectrumPhy to notify the PHY that a
* previously started RX attempt has been successfully completed.
*
* @param packet the received Packet
*/
typedef Callback< void, Ptr<Packet> > LtePhyRxDataEndOkCallback;


/**
* This method is used by the LteSpectrumPhy to notify the PHY that a
* previously started RX of a control frame attempt has been 
* successfully completed.
*
* @param packet the received Packet
*/
typedef Callback< void, std::list<Ptr<LteControlMessage> > > LtePhyRxCtrlEndOkCallback;

/**
* This method is used by the LteSpectrumPhy to notify the PHY that a
* previously started RX of a control frame attempt has terminated 
* without success.
*/
typedef Callback< void > LtePhyRxCtrlEndErrorCallback;

/**
* This method is used by the LteSpectrumPhy to notify the UE PHY that a
* PSS has been received
*/
typedef Callback< void, uint16_t, Ptr<SpectrumValue> > LtePhyRxPssCallback;


/**
* This method is used by the LteSpectrumPhy to notify the PHY about
* the status of a certain DL HARQ process
*/
typedef Callback< void, DlInfoListElement_s > LtePhyDlHarqFeedbackCallback;

/**
* This method is used by the LteSpectrumPhy to notify the PHY about
* the status of a certain UL HARQ process
*/
typedef Callback< void, UlInfoListElement_s > LtePhyUlHarqFeedbackCallback;



/**
 * \ingroup lte
 *
 * The LteSpectrumPhy models the physical layer of LTE
 *
 * It supports a single antenna model instance which is
 * used for both transmission and reception.  
 */
class LteSpectrumPhy : public SpectrumPhy
{

public:
  LteSpectrumPhy ();
  virtual ~LteSpectrumPhy ();

  /**
   *  PHY states
   */
  enum State
  {
    IDLE, TX_DL_CTRL, TX_DATA, TX_UL_SRS, RX_DL_CTRL, RX_DATA, RX_UL_SRS
  };

  // inherited from Object
  static TypeId GetTypeId (void);
  virtual void DoDispose ();

  // inherited from SpectrumPhy
  void SetChannel (Ptr<SpectrumChannel> c);
  void SetMobility (Ptr<MobilityModel> m);
  void SetDevice (Ptr<NetDevice> d);
  Ptr<MobilityModel> GetMobility ();
  Ptr<NetDevice> GetDevice ();
  Ptr<const SpectrumModel> GetRxSpectrumModel () const;
  Ptr<AntennaModel> GetRxAntenna ();
  void StartRx (Ptr<SpectrumSignalParameters> params);
  void StartRxData (Ptr<LteSpectrumSignalParametersDataFrame> params);
  void StartRxDlCtrl (Ptr<LteSpectrumSignalParametersDlCtrlFrame> lteDlCtrlRxParams);
  void StartRxUlSrs (Ptr<LteSpectrumSignalParametersUlSrsFrame> lteUlSrsRxParams);

  void SetHarqPhyModule (Ptr<LteHarqPhy> harq);

  /**
   * set the Power Spectral Density of outgoing signals in W/Hz.
   *
   * @param txPsd
   */
  void SetTxPowerSpectralDensity (Ptr<SpectrumValue> txPsd);

  /**
   * \brief set the noise power spectral density
   * @param noisePsd the Noise Power Spectral Density in power units
   * (Watt, Pascal...) per Hz.
   */
  void SetNoisePowerSpectralDensity (Ptr<const SpectrumValue> noisePsd);

  /** 
   * reset the internal state
   * 
   */
  void Reset ();
 
  /**
   * set the AntennaModel to be used
   * 
   * \param a the Antenna Model
   */
  void SetAntenna (Ptr<AntennaModel> a);
  
  /**
  * Start a transmission of data frame in DL and UL
  *
  *
  * @param pb the burst of packets to be transmitted in PDSCH/PUSCH
  * @param ctrlMsgList the list of LteControlMessage to send
  * @param duration the duration of the data frame 
  *
  * @return true if an error occurred and the transmission was not
  * started, false otherwise.
  */
  bool StartTxDataFrame (Ptr<PacketBurst> pb, std::list<Ptr<LteControlMessage> > ctrlMsgList, Time duration);
  
  /**
  * Start a transmission of control frame in DL
  *
  *
  * @param ctrlMsgList the burst of control messages to be transmitted
  * @param pss the flag for transmitting the primary synchronization signal
  *
  * @return true if an error occurred and the transmission was not
  * started, false otherwise.
  */
  bool StartTxDlCtrlFrame (std::list<Ptr<LteControlMessage> > ctrlMsgList, bool pss);
  
  
  /**
  * Start a transmission of control frame in UL
  *
  * @return true if an error occurred and the transmission was not
  * started, false otherwise.
  */
  bool StartTxUlSrsFrame ();


  /**
   * set the callback for the end of a TX, as part of the
   * interconnections between the PHY and the MAC
   *
   * @param c the callback
   */
  void SetLtePhyTxEndCallback (LtePhyTxEndCallback c);

  /**
   * set the callback for the end of a RX in error, as part of the
   * interconnections between the PHY and the MAC
   *
   * @param c the callback
   */
  void SetLtePhyRxDataEndErrorCallback (LtePhyRxDataEndErrorCallback c);

  /**
   * set the callback for the successful end of a RX, as part of the
   * interconnections between the PHY and the MAC
   *
   * @param c the callback
   */
  void SetLtePhyRxDataEndOkCallback (LtePhyRxDataEndOkCallback c);
  
  /**
  * set the callback for the successful end of a RX ctrl frame, as part 
  * of the interconnections between the LteSpectrumPhy and the PHY
  *
  * @param c the callback
  */
  void SetLtePhyRxCtrlEndOkCallback (LtePhyRxCtrlEndOkCallback c);
  
  /**
  * set the callback for the erroneous end of a RX ctrl frame, as part 
  * of the interconnections between the LteSpectrumPhy and the PHY
  *
  * @param c the callback
  */
  void SetLtePhyRxCtrlEndErrorCallback (LtePhyRxCtrlEndErrorCallback c);

  /**
  * set the callback for the reception of the PSS as part
  * of the interconnections between the LteSpectrumPhy and the UE PHY
  *
  * @param c the callback
  */
  void SetLtePhyRxPssCallback (LtePhyRxPssCallback c);

  /**
  * set the callback for the DL HARQ feedback as part of the 
  * interconnections between the LteSpectrumPhy and the PHY
  *
  * @param c the callback
  */
  void SetLtePhyDlHarqFeedbackCallback (LtePhyDlHarqFeedbackCallback c);

  /**
  * set the callback for the UL HARQ feedback as part of the
  * interconnections between the LteSpectrumPhy and the PHY
  *
  * @param c the callback
  */
  void SetLtePhyUlHarqFeedbackCallback (LtePhyUlHarqFeedbackCallback c);

  /**
   * \brief Set the state of the phy layer
   * \param newState the state
   */
  void SetState (State newState);

  /** 
   * 
   * 
   * \param cellId the Cell Identifier
   */
  void SetCellId (uint16_t cellId);


  /**
  *
  *
  * \param p the new LteChunkProcessor to be added to the RS power
  *          processing chain
  */
  void AddRsPowerChunkProcessor (Ptr<LteChunkProcessor> p);
  
  /**
  *
  *
  * \param p the new LteChunkProcessor to be added to the Data Channel power
  *          processing chain
  */
  void AddDataPowerChunkProcessor (Ptr<LteChunkProcessor> p);

  /** 
  * 
  * 
  * \param p the new LteChunkProcessor to be added to the data processing chain
  */
  void AddDataSinrChunkProcessor (Ptr<LteChunkProcessor> p);

  /**
  *  LteChunkProcessor devoted to evaluate interference + noise power
  *  in control symbols of the subframe
  *
  * \param p the new LteChunkProcessor to be added to the data processing chain
  */
  void AddInterferenceCtrlChunkProcessor (Ptr<LteChunkProcessor> p);

  /**
  *  LteChunkProcessor devoted to evaluate interference + noise power
  *  in data symbols of the subframe
  *
  * \param p the new LteChunkProcessor to be added to the data processing chain
  */
  void AddInterferenceDataChunkProcessor (Ptr<LteChunkProcessor> p);
  
  
  /** 
  * 
  * 
  * \param p the new LteChunkProcessor to be added to the ctrl processing chain
  */
  void AddCtrlSinrChunkProcessor (Ptr<LteChunkProcessor> p);
  
  /** 
  * 
  * 
  * \param rnti the rnti of the source of the TB
  * \param ndi new data indicator flag
  * \param size the size of the TB
  * \param mcs the MCS of the TB
  * \param map the map of RB(s) used
  * \param layer the layer (in case of MIMO tx)
  * \param harqId the id of the HARQ process (valid only for DL)
  * \param downlink true when the TB is for DL
  */
  void AddExpectedTb (uint16_t  rnti, uint8_t ndi, uint16_t size, uint8_t mcs, std::vector<int> map, uint8_t layer, uint8_t harqId, uint8_t rv, bool downlink);


  /** 
  * 
  * 
  * \param sinr vector of sinr perceived per each RB
  */
  void UpdateSinrPerceived (const SpectrumValue& sinr);
  
  /** 
  * 
  * 
  * \param txMode UE transmission mode (SISO, MIMO tx diversity, ...)
  */
  void SetTransmissionMode (uint8_t txMode);
  

  /** 
   * 
   * \return the previously set channel
   */
  Ptr<SpectrumChannel> GetChannel ();

  friend class LteUePhy;
  
 /**
  * Assign a fixed random variable stream number to the random variables
  * used by this model.  Return the number of streams (possibly zero) that
  * have been assigned.
  *
  * \param stream first stream index to use
  * \return the number of stream indices assigned by this model
  */
  int64_t AssignStreams (int64_t stream);

private:
  void ChangeState (State newState);
  void EndTxData ();
  void EndTxDlCtrl ();
  void EndTxUlSrs ();
  void EndRxData ();
  void EndRxDlCtrl ();
  void EndRxUlSrs ();
  
  void SetTxModeGain (uint8_t txMode, double gain);
  

  Ptr<MobilityModel> m_mobility;
  Ptr<AntennaModel> m_antenna;
  Ptr<NetDevice> m_device;

  Ptr<SpectrumChannel> m_channel;

  Ptr<const SpectrumModel> m_rxSpectrumModel;
  Ptr<SpectrumValue> m_txPsd;
  Ptr<PacketBurst> m_txPacketBurst;
  std::list<Ptr<PacketBurst> > m_rxPacketBurstList;
  
  std::list<Ptr<LteControlMessage> > m_txControlMessageList;
  std::list<Ptr<LteControlMessage> > m_rxControlMessageList;
  
  
  State m_state;
  Time m_firstRxStart;
  Time m_firstRxDuration;

  TracedCallback<Ptr<const PacketBurst> > m_phyTxStartTrace;
  TracedCallback<Ptr<const PacketBurst> > m_phyTxEndTrace;
  TracedCallback<Ptr<const PacketBurst> > m_phyRxStartTrace;
  TracedCallback<Ptr<const Packet> > m_phyRxEndOkTrace;
  TracedCallback<Ptr<const Packet> > m_phyRxEndErrorTrace;

  LtePhyRxDataEndErrorCallback   m_ltePhyRxDataEndErrorCallback;
  LtePhyRxDataEndOkCallback      m_ltePhyRxDataEndOkCallback;
  
  LtePhyRxCtrlEndOkCallback     m_ltePhyRxCtrlEndOkCallback;
  LtePhyRxCtrlEndErrorCallback  m_ltePhyRxCtrlEndErrorCallback;
  LtePhyRxPssCallback  m_ltePhyRxPssCallback;

  Ptr<LteInterference> m_interferenceData;
  Ptr<LteInterference> m_interferenceCtrl;

  uint16_t m_cellId;
  
  expectedTbs_t m_expectedTbs;
  SpectrumValue m_sinrPerceived;

  /// Provides uniform random variables.
  Ptr<UniformRandomVariable> m_random;
  bool m_dataErrorModelEnabled; // when true (default) the phy error model is enabled
  bool m_ctrlErrorModelEnabled; // when true (default) the phy error model is enabled for DL ctrl frame
  
  uint8_t m_transmissionMode; // for UEs: store the transmission mode
  uint8_t m_layersNum;
  std::vector <double> m_txModeGain; // duplicate value of LteUePhy

  Ptr<LteHarqPhy> m_harqPhyModule;
  LtePhyDlHarqFeedbackCallback m_ltePhyDlHarqFeedbackCallback;
  LtePhyUlHarqFeedbackCallback m_ltePhyUlHarqFeedbackCallback;


  /**
   * Trace information regarding PHY stats from DL Rx perspective
   * PhyReceptionStatParameters (see lte-common.h)
   */
  TracedCallback<PhyReceptionStatParameters> m_dlPhyReception;

  
  /**
   * Trace information regarding PHY stats from UL Rx perspective
   * PhyReceptionStatParameters (see lte-common.h)
   */
  TracedCallback<PhyReceptionStatParameters> m_ulPhyReception;

  EventId m_endTxEvent;
  EventId m_endRxDataEvent;
  EventId m_endRxDlCtrlEvent;
  EventId m_endRxUlSrsEvent;
  

};






}

#endif /* LTE_SPECTRUM_PHY_H */
