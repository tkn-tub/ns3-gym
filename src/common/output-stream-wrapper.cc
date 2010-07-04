/* -*- Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2010 University of Washington
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
 */

#include "output-stream-wrapper.h"
#include "ns3/log.h"
#include "ns3/fatal-impl.h"
#include "ns3/abort.h"
#include <fstream>

NS_LOG_COMPONENT_DEFINE ("OutputStreamWrapper");

namespace ns3 {

OutputStreamWrapper::OutputStreamWrapper (std::string filename, std::ios::openmode filemode)
  : m_ostream (new std::ofstream ())
{
  FatalImpl::RegisterStream (m_ostream);
  m_ostream->open (filename.c_str (), filemode);
  NS_ABORT_MSG_UNLESS (m_ostream->is_open (), "AsciiTraceHelper::CreateFileStream():  " <<
                       "Unable to Open " << filename << " for mode " << filemode);
}

OutputStreamWrapper::~OutputStreamWrapper ()
{
  FatalImpl::UnregisterStream (m_ostream);
  delete m_ostream;
  m_ostream = 0;
}

std::ostream *
OutputStreamWrapper::GetStream (void)
{
  return m_ostream;
}

} //namespace ns3
