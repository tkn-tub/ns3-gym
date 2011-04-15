/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2011 Centre Tecnologic de Telecomunicacions de Catalunya (CTTC)
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
 * Author: Manuel Requena <manuel.requena@cttc.es> : Based on lte-sinr-chunk-processor code
 *         Nicola Baldo <nbaldo@cttc.es>
 */

#ifndef LENA_TEST_SINR_CHUNK_PROCESSOR_H
#define LENA_TEST_SINR_CHUNK_PROCESSOR_H

#include "ns3/lte-sinr-chunk-processor.h"

namespace ns3 {


/**
 * This custom LteSinrChunkProcessor averages the calculated SINR over time.
 * It is plugged on the receiving SpectrumPhy and therefore
 * receives the SINR SpectrumValue calculated by the LteInterference module.
 */
class LenaTestSinrChunkProcessor : public LteSinrChunkProcessor
{
public:
  LenaTestSinrChunkProcessor (Ptr<LtePhy> p);
  virtual ~LenaTestSinrChunkProcessor ();

  virtual void Start ();
  virtual void EvaluateSinrChunk (const SpectrumValue& sinr, Time duration);
  virtual void End ();

  SpectrumValue GetSinr ();

private:
  Ptr<SpectrumValue> m_sumSinr;
  Time m_totDuration;
  Ptr<LtePhy> m_phy;
};


} // namespace ns3

#endif /* LENA_TEST_SINR_CHUNK_PROCESSOR_H */
