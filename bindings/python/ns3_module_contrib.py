from pybindgen import Module, FileCodeSink, write_preamble, param, retval

def register_types(module):
    root_module = module.get_root()
    
    ## gnuplot.h: ns3::Gnuplot [class]
    module.add_class('Gnuplot')
    ## event-garbage-collector.h: ns3::EventGarbageCollector [class]
    module.add_class('EventGarbageCollector')
    ## gnuplot.h: ns3::GnuplotDataset [class]
    module.add_class('GnuplotDataset')
    ## gnuplot.h: ns3::GnuplotDataset::Style [enumeration]
    module.add_enum('Style', ['LINES', 'POINTS', 'LINES_POINTS', 'DOTS', 'IMPULSES', 'STEPS', 'FSTEPS', 'HISTEPS'], outer_class=root_module['ns3::GnuplotDataset'])
    ## gnuplot.h: ns3::GnuplotDataset::ErrorBars [enumeration]
    module.add_enum('ErrorBars', ['NONE', 'X', 'Y', 'XY'], outer_class=root_module['ns3::GnuplotDataset'])
    ## gtk-config-store.h: ns3::GtkConfigStore [class]
    module.add_class('GtkConfigStore')
    ## config-store.h: ns3::ConfigStore [class]
    module.add_class('ConfigStore', allow_subclassing=True, parent=root_module['ns3::ObjectBase'])
    ## delay-jitter-estimation.h: ns3::DelayJitterEstimation [class]
    module.add_class('DelayJitterEstimation')
    
    ## Register a nested module for the namespace internal
    
    nested_module = module.add_cpp_namespace('internal')
    register_types_ns3_internal(nested_module)
    
    
    ## Register a nested module for the namespace TimeStepPrecision
    
    nested_module = module.add_cpp_namespace('TimeStepPrecision')
    register_types_ns3_TimeStepPrecision(nested_module)
    
    
    ## Register a nested module for the namespace Config
    
    nested_module = module.add_cpp_namespace('Config')
    register_types_ns3_Config(nested_module)
    
    
    ## Register a nested module for the namespace olsr
    
    nested_module = module.add_cpp_namespace('olsr')
    register_types_ns3_olsr(nested_module)
    

def register_types_ns3_internal(module):
    root_module = module.get_root()
    

def register_types_ns3_TimeStepPrecision(module):
    root_module = module.get_root()
    

def register_types_ns3_Config(module):
    root_module = module.get_root()
    

def register_types_ns3_olsr(module):
    root_module = module.get_root()
    

def register_methods(root_module):
    register_Ns3Gnuplot_methods(root_module, root_module['ns3::Gnuplot'])
    register_Ns3EventGarbageCollector_methods(root_module, root_module['ns3::EventGarbageCollector'])
    register_Ns3GnuplotDataset_methods(root_module, root_module['ns3::GnuplotDataset'])
    register_Ns3GtkConfigStore_methods(root_module, root_module['ns3::GtkConfigStore'])
    register_Ns3ConfigStore_methods(root_module, root_module['ns3::ConfigStore'])
    register_Ns3DelayJitterEstimation_methods(root_module, root_module['ns3::DelayJitterEstimation'])
    return

def register_Ns3Gnuplot_methods(root_module, cls):
    ## gnuplot.h: ns3::Gnuplot::Gnuplot(std::string pngFilename) [constructor]
    cls.add_constructor([param('std::string', 'pngFilename')], visibility='public')
    ## gnuplot.h: void ns3::Gnuplot::SetLegend(std::string xLegend, std::string yLegend) [member function]
    cls.add_method('SetLegend', 'void', [param('std::string', 'xLegend'), param('std::string', 'yLegend')])
    ## gnuplot.h: void ns3::Gnuplot::AddDataset(ns3::GnuplotDataset const & dataset) [member function]
    cls.add_method('AddDataset', 'void', [param('ns3::GnuplotDataset&', 'dataset', is_const=True)])
    ## gnuplot.h: void ns3::Gnuplot::GenerateOutput(std::ostream & os) [member function]
    cls.add_method('GenerateOutput', 'void', [param('std::ostream&', 'os')])
    return

def register_Ns3EventGarbageCollector_methods(root_module, cls):
    ## event-garbage-collector.h: ns3::EventGarbageCollector::EventGarbageCollector() [constructor]
    cls.add_constructor([], visibility='public')
    ## event-garbage-collector.h: void ns3::EventGarbageCollector::Track(ns3::EventId event) [member function]
    cls.add_method('Track', 'void', [param('ns3::EventId', 'event')])
    return

def register_Ns3GnuplotDataset_methods(root_module, cls):
    ## gnuplot.h: ns3::GnuplotDataset::GnuplotDataset() [constructor]
    cls.add_constructor([], visibility='public')
    ## gnuplot.h: ns3::GnuplotDataset::GnuplotDataset(std::string title) [constructor]
    cls.add_constructor([param('std::string', 'title')], visibility='public')
    ## gnuplot.h: void ns3::GnuplotDataset::SetStyle(ns3::GnuplotDataset::Style style) [member function]
    cls.add_method('SetStyle', 'void', [param('ns3::GnuplotDataset::Style', 'style')])
    ## gnuplot.h: void ns3::GnuplotDataset::SetErrorBars(ns3::GnuplotDataset::ErrorBars errorBars) [member function]
    cls.add_method('SetErrorBars', 'void', [param('ns3::GnuplotDataset::ErrorBars', 'errorBars')])
    ## gnuplot.h: void ns3::GnuplotDataset::Add(double x, double y) [member function]
    cls.add_method('Add', 'void', [param('double', 'x'), param('double', 'y')])
    ## gnuplot.h: void ns3::GnuplotDataset::Add(double x, double y, double errorDelta) [member function]
    cls.add_method('Add', 'void', [param('double', 'x'), param('double', 'y'), param('double', 'errorDelta')])
    return

def register_Ns3GtkConfigStore_methods(root_module, cls):
    ## gtk-config-store.h: ns3::GtkConfigStore::GtkConfigStore() [constructor]
    cls.add_constructor([], visibility='public')
    ## gtk-config-store.h: void ns3::GtkConfigStore::Configure() [member function]
    cls.add_method('Configure', 'void', [])
    return

def register_Ns3ConfigStore_methods(root_module, cls):
    ## config-store.h: static ns3::TypeId ns3::ConfigStore::GetTypeId() [member function]
    cls.add_method('GetTypeId', 'ns3::TypeId', [], is_static=True)
    ## config-store.h: ns3::TypeId ns3::ConfigStore::GetInstanceTypeId() const [member function]
    cls.add_method('GetInstanceTypeId', 'ns3::TypeId', [], is_const=True, is_virtual=True)
    ## config-store.h: ns3::ConfigStore::ConfigStore() [constructor]
    cls.add_constructor([], visibility='public')
    ## config-store.h: void ns3::ConfigStore::Configure() [member function]
    cls.add_method('Configure', 'void', [])
    return

def register_Ns3DelayJitterEstimation_methods(root_module, cls):
    ## delay-jitter-estimation.h: ns3::DelayJitterEstimation::DelayJitterEstimation() [constructor]
    cls.add_constructor([], visibility='public')
    ## delay-jitter-estimation.h: static void ns3::DelayJitterEstimation::PrepareTx(ns3::Ptr<const ns3::Packet> packet) [member function]
    cls.add_method('PrepareTx', 'void', [param('ns3::Ptr< const ns3::Packet >', 'packet')], is_static=True)
    ## delay-jitter-estimation.h: void ns3::DelayJitterEstimation::RecordRx(ns3::Ptr<const ns3::Packet> packet) [member function]
    cls.add_method('RecordRx', 'void', [param('ns3::Ptr< const ns3::Packet >', 'packet')])
    ## delay-jitter-estimation.h: ns3::Time ns3::DelayJitterEstimation::GetLastDelay() const [member function]
    cls.add_method('GetLastDelay', 'ns3::Time', [], is_const=True)
    ## delay-jitter-estimation.h: ns3::Time ns3::DelayJitterEstimation::GetLastJitter() const [member function]
    cls.add_method('GetLastJitter', 'ns3::Time', [], is_const=True)
    return

def register_functions(root_module):
    module = root_module
    register_functions_ns3_internal(module.get_submodule('internal'), root_module)
    register_functions_ns3_TimeStepPrecision(module.get_submodule('TimeStepPrecision'), root_module)
    register_functions_ns3_Config(module.get_submodule('Config'), root_module)
    register_functions_ns3_olsr(module.get_submodule('olsr'), root_module)
    return

def register_functions_ns3_internal(module, root_module):
    return

def register_functions_ns3_TimeStepPrecision(module, root_module):
    return

def register_functions_ns3_Config(module, root_module):
    return

def register_functions_ns3_olsr(module, root_module):
    return

