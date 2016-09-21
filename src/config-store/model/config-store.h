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

#ifndef CONFIG_STORE_H
#define CONFIG_STORE_H

#include "ns3/object-base.h"
#include "file-config.h"

namespace ns3 {

/**
 * \defgroup configstore Configuration Store/Load
 *
 * \brief Store and load simulation attribute configuration
 *
 * While it is possible to generate a sample config file and lightly
 * edit it to change a couple of values, there are cases where this
 * process will not work because the same value on the same object
 * can appear multiple times in the same automatically-generated 
 * configuration file under different configuration paths.
 *
 * As such, the best way to use this class is to use it to generate
 * an initial configuration file, extract from that configuration
 * file only the strictly necessary elements, and move these minimal
 * elements to a new configuration file which can then safely
 * be edited. Another option is to use the ns3::GtkConfigStore class
 * which will allow you to edit the parameters and will generate 
 * configuration files where all the instances of the same parameter
 * are changed.
 */

/**
 * \ingroup configstore
 *
 */
class ConfigStore : public ObjectBase
{
public:
  enum Mode {
    LOAD,
    SAVE,
    NONE
  };
  enum FileFormat {
    XML,
    RAW_TEXT
  };
  static TypeId GetTypeId (void);
  virtual TypeId GetInstanceTypeId (void) const;

  ConfigStore ();
  ~ConfigStore ();

  void SetMode (enum Mode mode);
  void SetFileFormat (enum FileFormat format);
  void SetFilename (std::string filename);

  void ConfigureDefaults (void);
  void ConfigureAttributes (void);

private:
  enum Mode m_mode;
  enum FileFormat m_fileFormat;
  std::string m_filename;
  FileConfig *m_file;
};

/**
 * @{
 * \ingroup configstore
 */
std::ostream & operator << (std::ostream & os, ConfigStore::Mode & mode);
std::ostream & operator << (std::ostream & os, ConfigStore::FileFormat & format);
/**@}*/

}  // namespace ns3

#endif /* CONFIG_STORE_H */
