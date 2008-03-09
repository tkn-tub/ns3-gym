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
#ifndef COMMAND_LINE_H
#define COMMAND_LINE_H

#include <string>
#include <sstream>
#include <list>

namespace ns3 {

class CommandLine
{
public:
  ~CommandLine ();

  template <typename T>
  void AddValue (const std::string &name,
		 const std::string &help,
		 T &value);

  void Parse (int &argc, char *argv[]) const;
private:
  class Item 
  {
  public:
    std::string m_name;
    std::string m_help;
    virtual ~Item ();
    virtual bool Parse (std::string value) = 0;
  };
  template <typename T>
  class UserItem : public Item
  {
  public:
    virtual bool Parse (std::string value);
    T *m_valuePtr;
  };
  void HandleArgument (std::string name, std::string value) const;
  void PrintHelp (void) const;
  void PrintGlobals (void) const;
  void PrintAttributes (std::string type) const;
  void PrintGroup (std::string group) const;
  void PrintTypeIds (void) const;
  void PrintGroups (void) const;

  typedef std::list<Item *> Items;
  Items m_items;
};

} // namespace ns3

namespace ns3 {

template <typename T>
void 
CommandLine::AddValue (const std::string &name,
		   const std::string &help,
		   T &value)
{
  UserItem<T> *item = new UserItem<T> ();
  item->m_name = name;
  item->m_help = help;
  item->m_valuePtr = &value;
  m_items.push_back (item);
}

template <typename T>
bool 
CommandLine::UserItem<T>::Parse (std::string value)
{
  std::istringstream iss;
  iss.str (value);
  iss >> (*m_valuePtr);
  return !iss.bad () && !iss.fail ();
}

} // namespace ns3

#endif /* COMMAND_LINE_H */
