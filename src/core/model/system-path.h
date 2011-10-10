/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2008 INRIA
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
 * Authors: Mathieu Lacage <mathieu.lacage@sophia.inria.fr>
 */
#ifndef SYSTEM_PATH
#define SYSTEM_PATH

#include <string>
#include <list>

namespace ns3 {

/**
 * \brief Encapsulate OS-specific functions to manipulate file and directory paths.
 *
 * The functions provided here are used mostly to implement the ns-3 test framework.
 */
namespace SystemPath {

  /**
   * \return the directory in which the currently-executing binary is located
   */
  std::string FindSelfDirectory (void);
  
  /**
   * \param left a path element
   * \param right a path element
   * \return a concatenation of the two input paths
   */
  std::string Append (std::string left, std::string right);

  /**
   * \param path a path
   * \return a list of path elements that can be joined together again with
   *         the Join function.
   * \sa ns3::SystemPath::Join
   */
  std::list<std::string> Split (std::string path);

  /**
   * \param begin iterator to first element to join
   * \param end iterator to last element to join
   * \return a path that is a concatenation of all the input elements.
   */
  std::string Join (std::list<std::string>::const_iterator begin,
		    std::list<std::string>::const_iterator end);
  
  /**
   * \param path a path which identifies a directory
   * \return a list of the filenames which are located in the input directory
   */
  std::list<std::string> ReadFiles (std::string path);

  /**
   * \return a path which identifies a temporary directory.
   *
   * The returned path identifies a directory which does not exist yet
   * Call ns3::SystemPath::MakeDirectories to create it. Yes, there is a
   * well-known security race in this API but we don't care in ns-3.
   */
  std::string MakeTemporaryDirectoryName (void);

  /**
   * \param path a path to a directory
   *
   * Create all the directories leading to path.
   */
  void MakeDirectories (std::string path);

} // namespace SystemPath


} // namespace ns3


#endif /* SYSTEM_PATH */
