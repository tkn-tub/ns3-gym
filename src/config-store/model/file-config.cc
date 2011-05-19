#include "file-config.h"

namespace ns3 {

FileConfig::~FileConfig ()
{
}

NoneFileConfig::NoneFileConfig ()
{
}
NoneFileConfig::~NoneFileConfig ()
{
}
void 
NoneFileConfig::SetFilename (std::string filename)
{}
void 
NoneFileConfig::Default (void)
{}
void 
NoneFileConfig::Global (void)
{}
void
NoneFileConfig::Attributes (void)
{}

} // namespace ns3
