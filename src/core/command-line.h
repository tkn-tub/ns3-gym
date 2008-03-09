#ifndef CMD_LINE_H
#define CMD_LINE_H

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

#endif /* CMD_LINE_H */
