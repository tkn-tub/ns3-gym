#ifndef INITIAL_VALUE_H
#define INITIAL_VALUE_H

#include <string>
#include <vector>
#include "ptr.h"
#include "attribute.h"


namespace ns3 {

class InitialValue
{
  typedef std::vector<InitialValue *> Vector;
public:
  typedef Vector::const_iterator Iterator;

  InitialValue (std::string name, std::string help,
		Attribute initialValue,
		Ptr<const AttributeChecker> checker);

  std::string GetName (void) const;
  std::string GetHelp (void) const;
  Attribute GetValue (void) const;
  Ptr<const AttributeChecker> GetChecker (void) const;
  
  void SetValue (Attribute value);

  static void Bind (std::string name, Attribute value);

  static Iterator Begin (void);
  static Iterator End (void);
private:
  static Vector *GetVector (void);
  std::string m_name;
  std::string m_help;
  Attribute m_initialValue;
  Ptr<const AttributeChecker> m_checker;
};

} // namespace ns3

#endif /* INITIAL_VALUE_H */
