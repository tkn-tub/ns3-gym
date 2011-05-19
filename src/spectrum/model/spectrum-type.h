/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
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
 */

#ifndef SPECTRUM_TYPE_H
#define SPECTRUM_TYPE_H

#include <ns3/log.h>
#include <iostream>
#include <vector>
#include <string>


namespace ns3 {


/**
 * \ingroup spectrum
 *
 * This class represent a type of signal that can be transmitted by
 * SpectrumPhy instances over a SpectrumChannel. By means of this
 * class a SpectrumPhy is able to recognize which type of signals it
 * is able to decode (i.e., receive) and which are to be considered
 * only as a source of interference. Note that this distinction of
 * signal types is an abstraction which is introduced only for
 * simulation purposes: in the real world a device needs to infer
 * whether a signal is of a known type by examining at properties of the
 * signal, such as preamble, CRC fields, etc.
 *
 */
class SpectrumType
{
  friend class SpectrumTypeFactory;

public:
  uint32_t GetUid () const;
  std::string GetName () const;

private:
  SpectrumType (uint32_t m_uid);
  uint32_t m_uid;
};


bool operator== (const SpectrumType& lhs, const SpectrumType& rhs);
bool operator!= (const SpectrumType& lhs, const SpectrumType& rhs);
std::ostream& operator<< (std::ostream& os, const SpectrumType& rhs);



/**
 * \ingroup spectrum
 *
 */
class SpectrumTypeFactory
{

public:
  static SpectrumType Create (std::string name);
  static std::string GetNameByUid (uint32_t uid);

private:
  static std::vector<std::string> m_names;
};


} // namespace ns3


#endif /*  SPECTRUM_TYPE_H */
