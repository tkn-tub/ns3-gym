/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2012 Telum (www.telum.ru)
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
 * Author: Kirill Andreev <andreev@telum.ru>
 */
#ifndef PROPAGATION_CACHE_H_
#define PROPAGATION_CACHE_H_

#include "ns3/mobility-model.h"
#include <map>

namespace ns3
{
/**
 * \ingroup propagation
 * \brief Constructs a cache of objects, where each obect is responsible for a single propagation path loss calculations.
 * Propagation path a-->b and b-->a is the same thing. Propagation path is identified by
 * a couple of MobilityModels and a spectrum model UID
 */
template<class T>
class PropagationCache
{
public:
  PropagationCache () {};
  ~PropagationCache () {};
  Ptr<T> GetPathData (Ptr<const MobilityModel> a, Ptr<const MobilityModel> b, uint32_t modelUid)
  {
    PropagationPathIdentifier key = PropagationPathIdentifier (a, b, modelUid);
    typename PathCache::iterator it = m_pathCache.find (key);
    if (it == m_pathCache.end ())
      {
        return 0;
      }
    return it->second;
  };
  void AddPathData (Ptr<T> data, Ptr<const MobilityModel> a, Ptr<const MobilityModel> b, uint32_t modelUid)
  {
    PropagationPathIdentifier key = PropagationPathIdentifier (a, b, modelUid);
    NS_ASSERT (m_pathCache.find (key) == m_pathCache.end ());
    m_pathCache.insert (std::make_pair (key, data)); 
  };
private:
  /// Each path is identified by
  struct PropagationPathIdentifier
  {
    PropagationPathIdentifier (Ptr<const MobilityModel> a, Ptr<const MobilityModel> b, uint32_t modelUid) :
      m_srcMobility (a), m_dstMobility (b), m_spectrumModelUid (modelUid)
    {};
    Ptr<const MobilityModel> m_srcMobility;
    Ptr<const MobilityModel> m_dstMobility;
    uint32_t m_spectrumModelUid;
    bool operator < (const PropagationPathIdentifier & other) const
    {
      if (m_spectrumModelUid != other.m_spectrumModelUid)
        {
          return m_spectrumModelUid < other.m_spectrumModelUid;
        }
      /// Links are supposed to be symmetrical!
      if (std::min (m_dstMobility, m_srcMobility) != std::min (other.m_dstMobility, other.m_srcMobility))
        {
          return std::min (m_dstMobility, m_srcMobility) < std::min (other.m_dstMobility, other.m_srcMobility);
        }
      if (std::max (m_dstMobility, m_srcMobility) != std::max (other.m_dstMobility, other.m_srcMobility))
        {
          return std::max (m_dstMobility, m_srcMobility) < std::max (other.m_dstMobility, other.m_srcMobility);
        }
      return false;
    }
  };
  typedef std::map<PropagationPathIdentifier, Ptr<T> > PathCache;
private:
  PathCache m_pathCache;
};
} // namespace ns3

#endif // PROPAGATION_CACHE_H_
