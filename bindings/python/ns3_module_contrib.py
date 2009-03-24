from pybindgen import Module, FileCodeSink, param, retval, cppclass

def register_types(module):
    root_module = module.get_root()
    
    ## delay-jitter-estimation.h: ns3::DelayJitterEstimation [class]
    module.add_class('DelayJitterEstimation')
    ## event-garbage-collector.h: ns3::EventGarbageCollector [class]
    module.add_class('EventGarbageCollector')
    ## file-config.h: ns3::FileConfig [class]
    module.add_class('FileConfig', allow_subclassing=True)
    ## gnuplot.h: ns3::Gnuplot [class]
    module.add_class('Gnuplot')
    ## gnuplot.h: ns3::GnuplotCollection [class]
    module.add_class('GnuplotCollection')
    ## gnuplot.h: ns3::GnuplotDataset [class]
    module.add_class('GnuplotDataset')
    ## gtk-config-store.h: ns3::GtkConfigStore [class]
    module.add_class('GtkConfigStore')
    ## file-config.h: ns3::NoneFileConfig [class]
    module.add_class('NoneFileConfig', parent=root_module['ns3::FileConfig'])
    ## config-store.h: ns3::ConfigStore [class]
    module.add_class('ConfigStore', parent=root_module['ns3::ObjectBase'])
    ## config-store.h: ns3::ConfigStore::Mode [enumeration]
    module.add_enum('Mode', ['LOAD', 'SAVE', 'NONE'], outer_class=root_module['ns3::ConfigStore'])
    ## config-store.h: ns3::ConfigStore::FileFormat [enumeration]
    module.add_enum('FileFormat', ['XML', 'RAW_TEXT'], outer_class=root_module['ns3::ConfigStore'])
    ## flow-id-tag.h: ns3::FlowIdTag [class]
    module.add_class('FlowIdTag', parent=root_module['ns3::Tag'])
    ## gnuplot.h: ns3::Gnuplot2dDataset [class]
    module.add_class('Gnuplot2dDataset', parent=root_module['ns3::GnuplotDataset'])
    ## gnuplot.h: ns3::Gnuplot2dDataset::Style [enumeration]
    module.add_enum('Style', ['LINES', 'POINTS', 'LINES_POINTS', 'DOTS', 'IMPULSES', 'STEPS', 'FSTEPS', 'HISTEPS'], outer_class=root_module['ns3::Gnuplot2dDataset'])
    ## gnuplot.h: ns3::Gnuplot2dDataset::ErrorBars [enumeration]
    module.add_enum('ErrorBars', ['NONE', 'X', 'Y', 'XY'], outer_class=root_module['ns3::Gnuplot2dDataset'])
    ## gnuplot.h: ns3::Gnuplot2dFunction [class]
    module.add_class('Gnuplot2dFunction', parent=root_module['ns3::GnuplotDataset'])
    ## gnuplot.h: ns3::Gnuplot3dDataset [class]
    module.add_class('Gnuplot3dDataset', parent=root_module['ns3::GnuplotDataset'])
    ## gnuplot.h: ns3::Gnuplot3dFunction [class]
    module.add_class('Gnuplot3dFunction', parent=root_module['ns3::GnuplotDataset'])
    
    ## Register a nested module for the namespace Config
    
    nested_module = module.add_cpp_namespace('Config')
    register_types_ns3_Config(nested_module)
    
    
    ## Register a nested module for the namespace TimeStepPrecision
    
    nested_module = module.add_cpp_namespace('TimeStepPrecision')
    register_types_ns3_TimeStepPrecision(nested_module)
    
    
    ## Register a nested module for the namespace internal
    
    nested_module = module.add_cpp_namespace('internal')
    register_types_ns3_internal(nested_module)
    
    
    ## Register a nested module for the namespace olsr
    
    nested_module = module.add_cpp_namespace('olsr')
    register_types_ns3_olsr(nested_module)
    

def register_types_ns3_Config(module):
    root_module = module.get_root()
    

def register_types_ns3_TimeStepPrecision(module):
    root_module = module.get_root()
    

def register_types_ns3_internal(module):
    root_module = module.get_root()
    

def register_types_ns3_olsr(module):
    root_module = module.get_root()
    

def register_methods(root_module):
    register_Ns3DelayJitterEstimation_methods(root_module, root_module['ns3::DelayJitterEstimation'])
    register_Ns3EventGarbageCollector_methods(root_module, root_module['ns3::EventGarbageCollector'])
    register_Ns3FileConfig_methods(root_module, root_module['ns3::FileConfig'])
    register_Ns3Gnuplot_methods(root_module, root_module['ns3::Gnuplot'])
    register_Ns3GnuplotCollection_methods(root_module, root_module['ns3::GnuplotCollection'])
    register_Ns3GnuplotDataset_methods(root_module, root_module['ns3::GnuplotDataset'])
    register_Ns3GtkConfigStore_methods(root_module, root_module['ns3::GtkConfigStore'])
    register_Ns3NoneFileConfig_methods(root_module, root_module['ns3::NoneFileConfig'])
    register_Ns3ConfigStore_methods(root_module, root_module['ns3::ConfigStore'])
    register_Ns3FlowIdTag_methods(root_module, root_module['ns3::FlowIdTag'])
    register_Ns3Gnuplot2dDataset_methods(root_module, root_module['ns3::Gnuplot2dDataset'])
    register_Ns3Gnuplot2dFunction_methods(root_module, root_module['ns3::Gnuplot2dFunction'])
    register_Ns3Gnuplot3dDataset_methods(root_module, root_module['ns3::Gnuplot3dDataset'])
    register_Ns3Gnuplot3dFunction_methods(root_module, root_module['ns3::Gnuplot3dFunction'])
    return

def register_Ns3DelayJitterEstimation_methods(root_module, cls):
    ## delay-jitter-estimation.h: ns3::DelayJitterEstimation::DelayJitterEstimation(ns3::DelayJitterEstimation const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::DelayJitterEstimation const &', 'arg0')])
    ## delay-jitter-estimation.h: ns3::DelayJitterEstimation::DelayJitterEstimation() [constructor]
    cls.add_constructor([])
    ## delay-jitter-estimation.h: static void ns3::DelayJitterEstimation::PrepareTx(ns3::Ptr<const ns3::Packet> packet) [member function]
    cls.add_method('PrepareTx', 
                   'void', 
                   [param('ns3::Ptr< ns3::Packet const >', 'packet')], 
                   is_static=True)
    ## delay-jitter-estimation.h: void ns3::DelayJitterEstimation::RecordRx(ns3::Ptr<const ns3::Packet> packet) [member function]
    cls.add_method('RecordRx', 
                   'void', 
                   [param('ns3::Ptr< ns3::Packet const >', 'packet')])
    ## delay-jitter-estimation.h: ns3::Time ns3::DelayJitterEstimation::GetLastDelay() const [member function]
    cls.add_method('GetLastDelay', 
                   'ns3::Time', 
                   [], 
                   is_const=True)
    ## delay-jitter-estimation.h: ns3::Time ns3::DelayJitterEstimation::GetLastJitter() const [member function]
    cls.add_method('GetLastJitter', 
                   'ns3::Time', 
                   [], 
                   is_const=True)
    return

def register_Ns3EventGarbageCollector_methods(root_module, cls):
    ## event-garbage-collector.h: ns3::EventGarbageCollector::EventGarbageCollector(ns3::EventGarbageCollector const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::EventGarbageCollector const &', 'arg0')])
    ## event-garbage-collector.h: ns3::EventGarbageCollector::EventGarbageCollector() [constructor]
    cls.add_constructor([])
    ## event-garbage-collector.h: void ns3::EventGarbageCollector::Track(ns3::EventId event) [member function]
    cls.add_method('Track', 
                   'void', 
                   [param('ns3::EventId', 'event')])
    return

def register_Ns3FileConfig_methods(root_module, cls):
    ## file-config.h: ns3::FileConfig::FileConfig(ns3::FileConfig const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::FileConfig const &', 'arg0')])
    ## file-config.h: ns3::FileConfig::FileConfig() [constructor]
    cls.add_constructor([])
    ## file-config.h: void ns3::FileConfig::SetFilename(std::string filename) [member function]
    cls.add_method('SetFilename', 
                   'void', 
                   [param('std::string', 'filename')], 
                   is_pure_virtual=True, is_virtual=True)
    ## file-config.h: void ns3::FileConfig::Default() [member function]
    cls.add_method('Default', 
                   'void', 
                   [], 
                   is_pure_virtual=True, is_virtual=True)
    ## file-config.h: void ns3::FileConfig::Global() [member function]
    cls.add_method('Global', 
                   'void', 
                   [], 
                   is_pure_virtual=True, is_virtual=True)
    ## file-config.h: void ns3::FileConfig::Attributes() [member function]
    cls.add_method('Attributes', 
                   'void', 
                   [], 
                   is_pure_virtual=True, is_virtual=True)
    return

def register_Ns3Gnuplot_methods(root_module, cls):
    ## gnuplot.h: ns3::Gnuplot::Gnuplot(ns3::Gnuplot const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::Gnuplot const &', 'arg0')])
    ## gnuplot.h: ns3::Gnuplot::Gnuplot(std::string const & outputFilename="", std::string const & title="") [constructor]
    cls.add_constructor([param('std::string const &', 'outputFilename', default_value='""'), param('std::string const &', 'title', default_value='""')])
    ## gnuplot.h: static std::string ns3::Gnuplot::DetectTerminal(std::string const & filename) [member function]
    cls.add_method('DetectTerminal', 
                   'std::string', 
                   [param('std::string const &', 'filename')], 
                   is_static=True)
    ## gnuplot.h: void ns3::Gnuplot::SetTerminal(std::string const & terminal) [member function]
    cls.add_method('SetTerminal', 
                   'void', 
                   [param('std::string const &', 'terminal')])
    ## gnuplot.h: void ns3::Gnuplot::SetTitle(std::string const & title) [member function]
    cls.add_method('SetTitle', 
                   'void', 
                   [param('std::string const &', 'title')])
    ## gnuplot.h: void ns3::Gnuplot::SetLegend(std::string const & xLegend, std::string const & yLegend) [member function]
    cls.add_method('SetLegend', 
                   'void', 
                   [param('std::string const &', 'xLegend'), param('std::string const &', 'yLegend')])
    ## gnuplot.h: void ns3::Gnuplot::SetExtra(std::string const & extra) [member function]
    cls.add_method('SetExtra', 
                   'void', 
                   [param('std::string const &', 'extra')])
    ## gnuplot.h: void ns3::Gnuplot::AppendExtra(std::string const & extra) [member function]
    cls.add_method('AppendExtra', 
                   'void', 
                   [param('std::string const &', 'extra')])
    ## gnuplot.h: void ns3::Gnuplot::AddDataset(ns3::GnuplotDataset const & dataset) [member function]
    cls.add_method('AddDataset', 
                   'void', 
                   [param('ns3::GnuplotDataset const &', 'dataset')])
    ## gnuplot.h: void ns3::Gnuplot::GenerateOutput(std::ostream & os) const [member function]
    cls.add_method('GenerateOutput', 
                   'void', 
                   [param('std::ostream &', 'os')], 
                   is_const=True)
    return

def register_Ns3GnuplotCollection_methods(root_module, cls):
    ## gnuplot.h: ns3::GnuplotCollection::GnuplotCollection(ns3::GnuplotCollection const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::GnuplotCollection const &', 'arg0')])
    ## gnuplot.h: ns3::GnuplotCollection::GnuplotCollection(std::string const & outputFilename) [constructor]
    cls.add_constructor([param('std::string const &', 'outputFilename')])
    ## gnuplot.h: void ns3::GnuplotCollection::SetTerminal(std::string const & terminal) [member function]
    cls.add_method('SetTerminal', 
                   'void', 
                   [param('std::string const &', 'terminal')])
    ## gnuplot.h: void ns3::GnuplotCollection::AddPlot(ns3::Gnuplot const & plot) [member function]
    cls.add_method('AddPlot', 
                   'void', 
                   [param('ns3::Gnuplot const &', 'plot')])
    ## gnuplot.h: ns3::Gnuplot & ns3::GnuplotCollection::GetPlot(unsigned int id) [member function]
    cls.add_method('GetPlot', 
                   'ns3::Gnuplot &', 
                   [param('unsigned int', 'id')])
    ## gnuplot.h: void ns3::GnuplotCollection::GenerateOutput(std::ostream & os) const [member function]
    cls.add_method('GenerateOutput', 
                   'void', 
                   [param('std::ostream &', 'os')], 
                   is_const=True)
    return

def register_Ns3GnuplotDataset_methods(root_module, cls):
    ## gnuplot.h: ns3::GnuplotDataset::GnuplotDataset(ns3::GnuplotDataset const & original) [copy constructor]
    cls.add_constructor([param('ns3::GnuplotDataset const &', 'original')])
    ## gnuplot.h: void ns3::GnuplotDataset::SetTitle(std::string const & title) [member function]
    cls.add_method('SetTitle', 
                   'void', 
                   [param('std::string const &', 'title')])
    ## gnuplot.h: static void ns3::GnuplotDataset::SetDefaultExtra(std::string const & extra) [member function]
    cls.add_method('SetDefaultExtra', 
                   'void', 
                   [param('std::string const &', 'extra')], 
                   is_static=True)
    ## gnuplot.h: void ns3::GnuplotDataset::SetExtra(std::string const & extra) [member function]
    cls.add_method('SetExtra', 
                   'void', 
                   [param('std::string const &', 'extra')])
    ## gnuplot.h: ns3::GnuplotDataset::GnuplotDataset(ns3::GnuplotDataset::Data * data) [constructor]
    cls.add_constructor([param('ns3::GnuplotDataset::Data *', 'data')], 
                        visibility='protected')
    return

def register_Ns3GtkConfigStore_methods(root_module, cls):
    ## gtk-config-store.h: ns3::GtkConfigStore::GtkConfigStore(ns3::GtkConfigStore const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::GtkConfigStore const &', 'arg0')])
    ## gtk-config-store.h: ns3::GtkConfigStore::GtkConfigStore() [constructor]
    cls.add_constructor([])
    ## gtk-config-store.h: void ns3::GtkConfigStore::ConfigureDefaults() [member function]
    cls.add_method('ConfigureDefaults', 
                   'void', 
                   [])
    ## gtk-config-store.h: void ns3::GtkConfigStore::ConfigureAttributes() [member function]
    cls.add_method('ConfigureAttributes', 
                   'void', 
                   [])
    return

def register_Ns3NoneFileConfig_methods(root_module, cls):
    ## file-config.h: ns3::NoneFileConfig::NoneFileConfig(ns3::NoneFileConfig const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::NoneFileConfig const &', 'arg0')])
    ## file-config.h: ns3::NoneFileConfig::NoneFileConfig() [constructor]
    cls.add_constructor([])
    ## file-config.h: void ns3::NoneFileConfig::SetFilename(std::string filename) [member function]
    cls.add_method('SetFilename', 
                   'void', 
                   [param('std::string', 'filename')], 
                   is_virtual=True)
    ## file-config.h: void ns3::NoneFileConfig::Default() [member function]
    cls.add_method('Default', 
                   'void', 
                   [], 
                   is_virtual=True)
    ## file-config.h: void ns3::NoneFileConfig::Global() [member function]
    cls.add_method('Global', 
                   'void', 
                   [], 
                   is_virtual=True)
    ## file-config.h: void ns3::NoneFileConfig::Attributes() [member function]
    cls.add_method('Attributes', 
                   'void', 
                   [], 
                   is_virtual=True)
    return

def register_Ns3ConfigStore_methods(root_module, cls):
    ## config-store.h: ns3::ConfigStore::ConfigStore(ns3::ConfigStore const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::ConfigStore const &', 'arg0')])
    ## config-store.h: static ns3::TypeId ns3::ConfigStore::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## config-store.h: ns3::TypeId ns3::ConfigStore::GetInstanceTypeId() const [member function]
    cls.add_method('GetInstanceTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_const=True, is_virtual=True)
    ## config-store.h: ns3::ConfigStore::ConfigStore() [constructor]
    cls.add_constructor([])
    ## config-store.h: void ns3::ConfigStore::SetMode(ns3::ConfigStore::Mode mode) [member function]
    cls.add_method('SetMode', 
                   'void', 
                   [param('ns3::ConfigStore::Mode', 'mode')])
    ## config-store.h: void ns3::ConfigStore::SetFileFormat(ns3::ConfigStore::FileFormat format) [member function]
    cls.add_method('SetFileFormat', 
                   'void', 
                   [param('ns3::ConfigStore::FileFormat', 'format')])
    ## config-store.h: void ns3::ConfigStore::SetFilename(std::string filename) [member function]
    cls.add_method('SetFilename', 
                   'void', 
                   [param('std::string', 'filename')])
    ## config-store.h: void ns3::ConfigStore::ConfigureDefaults() [member function]
    cls.add_method('ConfigureDefaults', 
                   'void', 
                   [])
    ## config-store.h: void ns3::ConfigStore::ConfigureAttributes() [member function]
    cls.add_method('ConfigureAttributes', 
                   'void', 
                   [])
    return

def register_Ns3FlowIdTag_methods(root_module, cls):
    ## flow-id-tag.h: ns3::FlowIdTag::FlowIdTag(ns3::FlowIdTag const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::FlowIdTag const &', 'arg0')])
    ## flow-id-tag.h: static ns3::TypeId ns3::FlowIdTag::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## flow-id-tag.h: ns3::TypeId ns3::FlowIdTag::GetInstanceTypeId() const [member function]
    cls.add_method('GetInstanceTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_const=True, is_virtual=True)
    ## flow-id-tag.h: uint32_t ns3::FlowIdTag::GetSerializedSize() const [member function]
    cls.add_method('GetSerializedSize', 
                   'uint32_t', 
                   [], 
                   is_const=True, is_virtual=True)
    ## flow-id-tag.h: void ns3::FlowIdTag::Serialize(ns3::TagBuffer buf) const [member function]
    cls.add_method('Serialize', 
                   'void', 
                   [param('ns3::TagBuffer', 'buf')], 
                   is_const=True, is_virtual=True)
    ## flow-id-tag.h: void ns3::FlowIdTag::Deserialize(ns3::TagBuffer buf) [member function]
    cls.add_method('Deserialize', 
                   'void', 
                   [param('ns3::TagBuffer', 'buf')], 
                   is_virtual=True)
    ## flow-id-tag.h: void ns3::FlowIdTag::Print(std::ostream & os) const [member function]
    cls.add_method('Print', 
                   'void', 
                   [param('std::ostream &', 'os')], 
                   is_const=True, is_virtual=True)
    ## flow-id-tag.h: ns3::FlowIdTag::FlowIdTag() [constructor]
    cls.add_constructor([])
    ## flow-id-tag.h: ns3::FlowIdTag::FlowIdTag(uint32_t flowId) [constructor]
    cls.add_constructor([param('uint32_t', 'flowId')])
    ## flow-id-tag.h: void ns3::FlowIdTag::SetFlowId(uint32_t flowId) [member function]
    cls.add_method('SetFlowId', 
                   'void', 
                   [param('uint32_t', 'flowId')])
    ## flow-id-tag.h: uint32_t ns3::FlowIdTag::GetFlowId() const [member function]
    cls.add_method('GetFlowId', 
                   'uint32_t', 
                   [], 
                   is_const=True)
    ## flow-id-tag.h: static uint32_t ns3::FlowIdTag::AllocateFlowId() [member function]
    cls.add_method('AllocateFlowId', 
                   'uint32_t', 
                   [], 
                   is_static=True)
    return

def register_Ns3Gnuplot2dDataset_methods(root_module, cls):
    ## gnuplot.h: ns3::Gnuplot2dDataset::Gnuplot2dDataset(ns3::Gnuplot2dDataset const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::Gnuplot2dDataset const &', 'arg0')])
    ## gnuplot.h: ns3::Gnuplot2dDataset::Gnuplot2dDataset(std::string const & title="Untitled") [constructor]
    cls.add_constructor([param('std::string const &', 'title', default_value='"Untitled"')])
    ## gnuplot.h: static void ns3::Gnuplot2dDataset::SetDefaultStyle(ns3::Gnuplot2dDataset::Style style) [member function]
    cls.add_method('SetDefaultStyle', 
                   'void', 
                   [param('ns3::Gnuplot2dDataset::Style', 'style')], 
                   is_static=True)
    ## gnuplot.h: void ns3::Gnuplot2dDataset::SetStyle(ns3::Gnuplot2dDataset::Style style) [member function]
    cls.add_method('SetStyle', 
                   'void', 
                   [param('ns3::Gnuplot2dDataset::Style', 'style')])
    ## gnuplot.h: static void ns3::Gnuplot2dDataset::SetDefaultErrorBars(ns3::Gnuplot2dDataset::ErrorBars errorBars) [member function]
    cls.add_method('SetDefaultErrorBars', 
                   'void', 
                   [param('ns3::Gnuplot2dDataset::ErrorBars', 'errorBars')], 
                   is_static=True)
    ## gnuplot.h: void ns3::Gnuplot2dDataset::SetErrorBars(ns3::Gnuplot2dDataset::ErrorBars errorBars) [member function]
    cls.add_method('SetErrorBars', 
                   'void', 
                   [param('ns3::Gnuplot2dDataset::ErrorBars', 'errorBars')])
    ## gnuplot.h: void ns3::Gnuplot2dDataset::Add(double x, double y) [member function]
    cls.add_method('Add', 
                   'void', 
                   [param('double', 'x'), param('double', 'y')])
    ## gnuplot.h: void ns3::Gnuplot2dDataset::Add(double x, double y, double errorDelta) [member function]
    cls.add_method('Add', 
                   'void', 
                   [param('double', 'x'), param('double', 'y'), param('double', 'errorDelta')])
    ## gnuplot.h: void ns3::Gnuplot2dDataset::Add(double x, double y, double minY, double maxY) [member function]
    cls.add_method('Add', 
                   'void', 
                   [param('double', 'x'), param('double', 'y'), param('double', 'minY'), param('double', 'maxY')])
    ## gnuplot.h: void ns3::Gnuplot2dDataset::AddEmptyLine() [member function]
    cls.add_method('AddEmptyLine', 
                   'void', 
                   [])
    return

def register_Ns3Gnuplot2dFunction_methods(root_module, cls):
    ## gnuplot.h: ns3::Gnuplot2dFunction::Gnuplot2dFunction(ns3::Gnuplot2dFunction const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::Gnuplot2dFunction const &', 'arg0')])
    ## gnuplot.h: ns3::Gnuplot2dFunction::Gnuplot2dFunction(std::string const & title="Untitled", std::string const & function="") [constructor]
    cls.add_constructor([param('std::string const &', 'title', default_value='"Untitled"'), param('std::string const &', 'function', default_value='""')])
    ## gnuplot.h: void ns3::Gnuplot2dFunction::SetFunction(std::string const & function) [member function]
    cls.add_method('SetFunction', 
                   'void', 
                   [param('std::string const &', 'function')])
    return

def register_Ns3Gnuplot3dDataset_methods(root_module, cls):
    ## gnuplot.h: ns3::Gnuplot3dDataset::Gnuplot3dDataset(ns3::Gnuplot3dDataset const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::Gnuplot3dDataset const &', 'arg0')])
    ## gnuplot.h: ns3::Gnuplot3dDataset::Gnuplot3dDataset(std::string const & title="Untitled") [constructor]
    cls.add_constructor([param('std::string const &', 'title', default_value='"Untitled"')])
    ## gnuplot.h: static void ns3::Gnuplot3dDataset::SetDefaultStyle(std::string const & style) [member function]
    cls.add_method('SetDefaultStyle', 
                   'void', 
                   [param('std::string const &', 'style')], 
                   is_static=True)
    ## gnuplot.h: void ns3::Gnuplot3dDataset::SetStyle(std::string const & style) [member function]
    cls.add_method('SetStyle', 
                   'void', 
                   [param('std::string const &', 'style')])
    ## gnuplot.h: void ns3::Gnuplot3dDataset::Add(double x, double y, double z) [member function]
    cls.add_method('Add', 
                   'void', 
                   [param('double', 'x'), param('double', 'y'), param('double', 'z')])
    ## gnuplot.h: void ns3::Gnuplot3dDataset::AddEmptyLine() [member function]
    cls.add_method('AddEmptyLine', 
                   'void', 
                   [])
    return

def register_Ns3Gnuplot3dFunction_methods(root_module, cls):
    ## gnuplot.h: ns3::Gnuplot3dFunction::Gnuplot3dFunction(ns3::Gnuplot3dFunction const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::Gnuplot3dFunction const &', 'arg0')])
    ## gnuplot.h: ns3::Gnuplot3dFunction::Gnuplot3dFunction(std::string const & title="Untitled", std::string const & function="") [constructor]
    cls.add_constructor([param('std::string const &', 'title', default_value='"Untitled"'), param('std::string const &', 'function', default_value='""')])
    ## gnuplot.h: void ns3::Gnuplot3dFunction::SetFunction(std::string const & function) [member function]
    cls.add_method('SetFunction', 
                   'void', 
                   [param('std::string const &', 'function')])
    return

def register_functions(root_module):
    module = root_module
    register_functions_ns3_Config(module.get_submodule('Config'), root_module)
    register_functions_ns3_TimeStepPrecision(module.get_submodule('TimeStepPrecision'), root_module)
    register_functions_ns3_internal(module.get_submodule('internal'), root_module)
    register_functions_ns3_olsr(module.get_submodule('olsr'), root_module)
    return

def register_functions_ns3_Config(module, root_module):
    return

def register_functions_ns3_TimeStepPrecision(module, root_module):
    return

def register_functions_ns3_internal(module, root_module):
    return

def register_functions_ns3_olsr(module, root_module):
    return

