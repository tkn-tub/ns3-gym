/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2009 INRIA
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
 * Author: Mathieu Lacage <mathieu.lacage@cutebugs.net>
 */

#ifndef RAW_TEXT_CONFIG_H
#define RAW_TEXT_CONFIG_H

#include <string>
#include <fstream>
#include "file-config.h"

namespace ns3 {

/**
 * \ingroup configstore
 *
 */
class RawTextConfigSave : public FileConfig
{
public:
  RawTextConfigSave ();
  virtual ~RawTextConfigSave ();
  virtual void SetFilename (std::string filename);
  virtual void Default (void);
  virtual void Global (void);
  virtual void Attributes (void);
private:
  std::ofstream *m_os;
};

/**
 * \ingroup configstore
 *
 */
class RawTextConfigLoad : public FileConfig
{
public:
  RawTextConfigLoad ();
  virtual ~RawTextConfigLoad ();
  virtual void SetFilename (std::string filename);
  virtual void Default (void);
  virtual void Global (void);
  virtual void Attributes (void);
private:
  std::string Strip (std::string value);
  std::ifstream *m_is;
};

} // namespace ns3

#endif /* RAW_TEXT_CONFIG_H */
