/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2009, 2010 Centre Tecnologic de Telecomunicacions de Catalunya (CTTC)
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


#ifndef LTE_SINR_CHUNK_PROCESSOR_H
#define LTE_SINR_CHUNK_PROCESSOR_H

#include <ns3/spectrum-value.h>
#include <ns3/ptr.h>
#include <ns3/packet.h>
#include <ns3/nstime.h>
#include <ns3/object.h>
#include <ns3/lte-phy.h>
#include <ns3/lte-spectrum-phy.h>

namespace ns3 {



/** 
 * This abstract class is used to process the time-vs-frequency SINR chunk
 * of a received LTE signal which was calculated by the
 * LteInterference object.
 * 
 */
class LteSinrChunkProcessor : public SimpleRefCount<LteSinrChunkProcessor> 
{
public:
  virtual ~LteSinrChunkProcessor ();
  virtual void Start () = 0;
  virtual void EvaluateSinrChunk (const SpectrumValue& sinr, Time duration) = 0;
  virtual void End () = 0;
};




/** 
 * This SinrProcessor averages the calculated SINR over time.
 * 
 */
class LteCqiSinrChunkProcessor : public LteSinrChunkProcessor
{
public:
  virtual ~LteCqiSinrChunkProcessor ();
  LteCqiSinrChunkProcessor (Ptr<LtePhy> p);
  virtual void Start ();
  virtual void EvaluateSinrChunk (const SpectrumValue& sinr, Time duration);
  virtual void End ();
private:
  Ptr<SpectrumValue> m_sumSinr;
  Time m_totDuration;
  Ptr<LtePhy> m_phy;
};



/** 
* This SinrProcessor averages the calculated SINR over time for Phy error model
* 
*/
class LtePemSinrChunkProcessor : public LteSinrChunkProcessor
{
  public:
    virtual ~LtePemSinrChunkProcessor ();
    LtePemSinrChunkProcessor (Ptr<LteSpectrumPhy> p);
    virtual void Start ();
    virtual void EvaluateSinrChunk (const SpectrumValue& sinr, Time duration);
    virtual void End ();
  private:
    Ptr<SpectrumValue> m_sumSinr;
    Time m_totDuration;
    Ptr<LteSpectrumPhy> m_phy;
};





} // namespace ns3



#endif /* LTE_SINR_PROCESSOR_H */
