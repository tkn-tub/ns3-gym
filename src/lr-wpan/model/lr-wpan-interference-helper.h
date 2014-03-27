/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2013 Fraunhofer FKIE
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
 * Author:
 *  Sascha Alexander Jopen <jopen@cs.uni-bonn.de>
 */
#ifndef LR_WPAN_LR_WPAN_INTERFERENCE_HELPER_H
#define LR_WPAN_LR_WPAN_INTERFERENCE_HELPER_H

#include <ns3/simple-ref-count.h>
#include <ns3/ptr.h>
#include <set>

namespace ns3 {

class SpectrumValue;
class SpectrumModel;

class LrWpanInterferenceHelper : public SimpleRefCount<LrWpanInterferenceHelper>
{
public:
  LrWpanInterferenceHelper (Ptr<const SpectrumModel> spectrumModel);
  ~LrWpanInterferenceHelper ();

  bool AddSignal (Ptr<const SpectrumValue> signal);
  bool RemoveSignal (Ptr<const SpectrumValue> signal);
  void ClearSignals ();
  Ptr<SpectrumValue> GetSignalPsd () const;
  Ptr<const SpectrumModel> GetSpectrumModel () const;
private:
  Ptr<const SpectrumModel> m_spectrumModel;
  std::set<Ptr<const SpectrumValue> > m_signals;
  mutable Ptr<SpectrumValue> m_signal;
  mutable bool m_dirty;
};

}

#endif /* LR_WPAN_LR_WPAN_INTERFERENCE_HELPER_H */
