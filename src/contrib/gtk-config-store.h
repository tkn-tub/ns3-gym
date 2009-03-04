#ifndef GTK_CONFIG_STORE_H
#define GTK_CONFIG_STORE_H

namespace ns3 {

/**
 * \brief A class that provides a GTK-based front end to ns3::ConfigStore 
 */
class GtkConfigStore
{
public:
  GtkConfigStore ();

  void ConfigureEarly (void);
  void ConfigureLate (void);
};


} // namespace ns3

#endif /* GTK_CONFIG_STORE_H */
