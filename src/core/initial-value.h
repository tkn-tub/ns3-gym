#ifndef INITIAL_VALUE_H
#define INITIAL_VALUE_H

#include <string>
#include <vector>
#include "ptr.h"
#include "value.h"


namespace ns3 {

class InitialValue
{
  typedef std::vector<InitialValue *> Vector;
public:
  typedef Vector::const_iterator Iterator;

  InitialValue (std::string name, std::string help,
		PValue initialValue,
		Ptr<const AttributeChecker> checker);

  std::string GetName (void) const;
  std::string GetHelp (void) const;
  PValue GetInitialValue (void) const;
  Ptr<const AttributeChecker> GetChecker (void) const;
  
  void SetInitialValue (PValue value);

  static void Bind (std::string name, PValue value);

  static Iterator Begin (void);
  static Iterator End (void);
private:
  static Vector *GetVector (void);
  std::string m_name;
  std::string m_help;
  PValue m_initialValue;
  Ptr<const AttributeChecker> m_checker;
};

} // namespace ns3

#endif /* INITIAL_VALUE_H */
