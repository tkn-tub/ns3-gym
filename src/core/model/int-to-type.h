#ifndef INT_TO_TYPE_H
#define INT_TO_TYPE_H

namespace ns3 {

/**
 * This trivial template is extremely useful, as explained in
 * "Modern C++ Design", p29, section 2.4, 
 * "Mapping Integral Constants to Types"
 */
template <int v>
struct IntToType
{
  enum v_e { value = v};
};

} // namespace ns3

#endif /* INT_TO_TYPE_H */
