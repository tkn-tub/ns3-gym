from pybindgen import Module, FileCodeSink, param, retval, cppclass, typehandlers

def register_types(module):
    root_module = module.get_root()
    
    ## average.h: ns3::Average<double> [class]
    module.add_class('Average', template_parameters=['double'])
    ## delay-jitter-estimation.h: ns3::DelayJitterEstimation [class]
    module.add_class('DelayJitterEstimation')
    ## event-garbage-collector.h: ns3::EventGarbageCollector [class]
    module.add_class('EventGarbageCollector')
    ## gnuplot.h: ns3::Gnuplot [class]
    module.add_class('Gnuplot')
    ## gnuplot.h: ns3::GnuplotCollection [class]
    module.add_class('GnuplotCollection')
    ## gnuplot.h: ns3::GnuplotDataset [class]
    module.add_class('GnuplotDataset')
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
    
    
    ## Register a nested module for the namespace FatalImpl
    
    nested_module = module.add_cpp_namespace('FatalImpl')
    register_types_ns3_FatalImpl(nested_module)
    
    
    ## Register a nested module for the namespace addressUtils
    
    nested_module = module.add_cpp_namespace('addressUtils')
    register_types_ns3_addressUtils(nested_module)
    
    
    ## Register a nested module for the namespace aodv
    
    nested_module = module.add_cpp_namespace('aodv')
    register_types_ns3_aodv(nested_module)
    
    
    ## Register a nested module for the namespace dot11s
    
    nested_module = module.add_cpp_namespace('dot11s')
    register_types_ns3_dot11s(nested_module)
    
    
    ## Register a nested module for the namespace dsdv
    
    nested_module = module.add_cpp_namespace('dsdv')
    register_types_ns3_dsdv(nested_module)
    
    
    ## Register a nested module for the namespace flame
    
    nested_module = module.add_cpp_namespace('flame')
    register_types_ns3_flame(nested_module)
    
    
    ## Register a nested module for the namespace internal
    
    nested_module = module.add_cpp_namespace('internal')
    register_types_ns3_internal(nested_module)
    
    
    ## Register a nested module for the namespace olsr
    
    nested_module = module.add_cpp_namespace('olsr')
    register_types_ns3_olsr(nested_module)
    

def register_types_ns3_Config(module):
    root_module = module.get_root()
    

def register_types_ns3_FatalImpl(module):
    root_module = module.get_root()
    

def register_types_ns3_addressUtils(module):
    root_module = module.get_root()
    

def register_types_ns3_aodv(module):
    root_module = module.get_root()
    

def register_types_ns3_dot11s(module):
    root_module = module.get_root()
    

def register_types_ns3_dsdv(module):
    root_module = module.get_root()
    

def register_types_ns3_flame(module):
    root_module = module.get_root()
    

def register_types_ns3_internal(module):
    root_module = module.get_root()
    

def register_types_ns3_olsr(module):
    root_module = module.get_root()
    

def register_methods(root_module):
    register_Ns3Average__Double_methods(root_module, root_module['ns3::Average< double >'])
    register_Ns3DelayJitterEstimation_methods(root_module, root_module['ns3::DelayJitterEstimation'])
    register_Ns3EventGarbageCollector_methods(root_module, root_module['ns3::EventGarbageCollector'])
    register_Ns3Gnuplot_methods(root_module, root_module['ns3::Gnuplot'])
    register_Ns3GnuplotCollection_methods(root_module, root_module['ns3::GnuplotCollection'])
    register_Ns3GnuplotDataset_methods(root_module, root_module['ns3::GnuplotDataset'])
    register_Ns3Gnuplot2dDataset_methods(root_module, root_module['ns3::Gnuplot2dDataset'])
    register_Ns3Gnuplot2dFunction_methods(root_module, root_module['ns3::Gnuplot2dFunction'])
    register_Ns3Gnuplot3dDataset_methods(root_module, root_module['ns3::Gnuplot3dDataset'])
    register_Ns3Gnuplot3dFunction_methods(root_module, root_module['ns3::Gnuplot3dFunction'])
    return

def register_Ns3Average__Double_methods(root_module, cls):
    ## average.h: ns3::Average<double>::Average(ns3::Average<double> const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::Average< double > const &', 'arg0')])
    ## average.h: ns3::Average<double>::Average() [constructor]
    cls.add_constructor([])
    ## average.h: double ns3::Average<double>::Avg() const [member function]
    cls.add_method('Avg', 
                   'double', 
                   [], 
                   is_const=True)
    ## average.h: uint32_t ns3::Average<double>::Count() const [member function]
    cls.add_method('Count', 
                   'uint32_t', 
                   [], 
                   is_const=True)
    ## average.h: double ns3::Average<double>::Error90() const [member function]
    cls.add_method('Error90', 
                   'double', 
                   [], 
                   is_const=True)
    ## average.h: double ns3::Average<double>::Error95() const [member function]
    cls.add_method('Error95', 
                   'double', 
                   [], 
                   is_const=True)
    ## average.h: double ns3::Average<double>::Error99() const [member function]
    cls.add_method('Error99', 
                   'double', 
                   [], 
                   is_const=True)
    ## average.h: double ns3::Average<double>::Max() const [member function]
    cls.add_method('Max', 
                   'double', 
                   [], 
                   is_const=True)
    ## average.h: double ns3::Average<double>::Mean() const [member function]
    cls.add_method('Mean', 
                   'double', 
                   [], 
                   is_const=True)
    ## average.h: double ns3::Average<double>::Min() const [member function]
    cls.add_method('Min', 
                   'double', 
                   [], 
                   is_const=True)
    ## average.h: void ns3::Average<double>::Reset() [member function]
    cls.add_method('Reset', 
                   'void', 
                   [])
    ## average.h: double ns3::Average<double>::Stddev() const [member function]
    cls.add_method('Stddev', 
                   'double', 
                   [], 
                   is_const=True)
    ## average.h: void ns3::Average<double>::Update(double const & x) [member function]
    cls.add_method('Update', 
                   'void', 
                   [param('double const &', 'x')])
    ## average.h: double ns3::Average<double>::Var() const [member function]
    cls.add_method('Var', 
                   'double', 
                   [], 
                   is_const=True)
    return

def register_Ns3DelayJitterEstimation_methods(root_module, cls):
    ## delay-jitter-estimation.h: ns3::DelayJitterEstimation::DelayJitterEstimation(ns3::DelayJitterEstimation const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::DelayJitterEstimation const &', 'arg0')])
    ## delay-jitter-estimation.h: ns3::DelayJitterEstimation::DelayJitterEstimation() [constructor]
    cls.add_constructor([])
    ## delay-jitter-estimation.h: ns3::Time ns3::DelayJitterEstimation::GetLastDelay() const [member function]
    cls.add_method('GetLastDelay', 
                   'ns3::Time', 
                   [], 
                   is_const=True)
    ## delay-jitter-estimation.h: uint64_t ns3::DelayJitterEstimation::GetLastJitter() const [member function]
    cls.add_method('GetLastJitter', 
                   'uint64_t', 
                   [], 
                   is_const=True)
    ## delay-jitter-estimation.h: static void ns3::DelayJitterEstimation::PrepareTx(ns3::Ptr<ns3::Packet const> packet) [member function]
    cls.add_method('PrepareTx', 
                   'void', 
                   [param('ns3::Ptr< ns3::Packet const >', 'packet')], 
                   is_static=True)
    ## delay-jitter-estimation.h: void ns3::DelayJitterEstimation::RecordRx(ns3::Ptr<ns3::Packet const> packet) [member function]
    cls.add_method('RecordRx', 
                   'void', 
                   [param('ns3::Ptr< ns3::Packet const >', 'packet')])
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

def register_Ns3Gnuplot_methods(root_module, cls):
    ## gnuplot.h: ns3::Gnuplot::Gnuplot(ns3::Gnuplot const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::Gnuplot const &', 'arg0')])
    ## gnuplot.h: ns3::Gnuplot::Gnuplot(std::string const & outputFilename="", std::string const & title="") [constructor]
    cls.add_constructor([param('std::string const &', 'outputFilename', default_value='""'), param('std::string const &', 'title', default_value='""')])
    ## gnuplot.h: void ns3::Gnuplot::AddDataset(ns3::GnuplotDataset const & dataset) [member function]
    cls.add_method('AddDataset', 
                   'void', 
                   [param('ns3::GnuplotDataset const &', 'dataset')])
    ## gnuplot.h: void ns3::Gnuplot::AppendExtra(std::string const & extra) [member function]
    cls.add_method('AppendExtra', 
                   'void', 
                   [param('std::string const &', 'extra')])
    ## gnuplot.h: static std::string ns3::Gnuplot::DetectTerminal(std::string const & filename) [member function]
    cls.add_method('DetectTerminal', 
                   'std::string', 
                   [param('std::string const &', 'filename')], 
                   is_static=True)
    ## gnuplot.h: void ns3::Gnuplot::GenerateOutput(std::ostream & os) const [member function]
    cls.add_method('GenerateOutput', 
                   'void', 
                   [param('std::ostream &', 'os')], 
                   is_const=True)
    ## gnuplot.h: void ns3::Gnuplot::SetExtra(std::string const & extra) [member function]
    cls.add_method('SetExtra', 
                   'void', 
                   [param('std::string const &', 'extra')])
    ## gnuplot.h: void ns3::Gnuplot::SetLegend(std::string const & xLegend, std::string const & yLegend) [member function]
    cls.add_method('SetLegend', 
                   'void', 
                   [param('std::string const &', 'xLegend'), param('std::string const &', 'yLegend')])
    ## gnuplot.h: void ns3::Gnuplot::SetTerminal(std::string const & terminal) [member function]
    cls.add_method('SetTerminal', 
                   'void', 
                   [param('std::string const &', 'terminal')])
    ## gnuplot.h: void ns3::Gnuplot::SetTitle(std::string const & title) [member function]
    cls.add_method('SetTitle', 
                   'void', 
                   [param('std::string const &', 'title')])
    return

def register_Ns3GnuplotCollection_methods(root_module, cls):
    ## gnuplot.h: ns3::GnuplotCollection::GnuplotCollection(ns3::GnuplotCollection const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::GnuplotCollection const &', 'arg0')])
    ## gnuplot.h: ns3::GnuplotCollection::GnuplotCollection(std::string const & outputFilename) [constructor]
    cls.add_constructor([param('std::string const &', 'outputFilename')])
    ## gnuplot.h: void ns3::GnuplotCollection::AddPlot(ns3::Gnuplot const & plot) [member function]
    cls.add_method('AddPlot', 
                   'void', 
                   [param('ns3::Gnuplot const &', 'plot')])
    ## gnuplot.h: void ns3::GnuplotCollection::GenerateOutput(std::ostream & os) const [member function]
    cls.add_method('GenerateOutput', 
                   'void', 
                   [param('std::ostream &', 'os')], 
                   is_const=True)
    ## gnuplot.h: ns3::Gnuplot & ns3::GnuplotCollection::GetPlot(unsigned int id) [member function]
    cls.add_method('GetPlot', 
                   'ns3::Gnuplot &', 
                   [param('unsigned int', 'id')])
    ## gnuplot.h: void ns3::GnuplotCollection::SetTerminal(std::string const & terminal) [member function]
    cls.add_method('SetTerminal', 
                   'void', 
                   [param('std::string const &', 'terminal')])
    return

def register_Ns3GnuplotDataset_methods(root_module, cls):
    ## gnuplot.h: ns3::GnuplotDataset::GnuplotDataset(ns3::GnuplotDataset const & original) [copy constructor]
    cls.add_constructor([param('ns3::GnuplotDataset const &', 'original')])
    ## gnuplot.h: static void ns3::GnuplotDataset::SetDefaultExtra(std::string const & extra) [member function]
    cls.add_method('SetDefaultExtra', 
                   'void', 
                   [param('std::string const &', 'extra')], 
                   is_static=True)
    ## gnuplot.h: void ns3::GnuplotDataset::SetExtra(std::string const & extra) [member function]
    cls.add_method('SetExtra', 
                   'void', 
                   [param('std::string const &', 'extra')])
    ## gnuplot.h: void ns3::GnuplotDataset::SetTitle(std::string const & title) [member function]
    cls.add_method('SetTitle', 
                   'void', 
                   [param('std::string const &', 'title')])
    ## gnuplot.h: ns3::GnuplotDataset::GnuplotDataset(ns3::GnuplotDataset::Data * data) [constructor]
    cls.add_constructor([param('ns3::GnuplotDataset::Data *', 'data')], 
                        visibility='protected')
    return

def register_Ns3Gnuplot2dDataset_methods(root_module, cls):
    ## gnuplot.h: ns3::Gnuplot2dDataset::Gnuplot2dDataset(ns3::Gnuplot2dDataset const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::Gnuplot2dDataset const &', 'arg0')])
    ## gnuplot.h: ns3::Gnuplot2dDataset::Gnuplot2dDataset(std::string const & title="Untitled") [constructor]
    cls.add_constructor([param('std::string const &', 'title', default_value='"Untitled"')])
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
    ## gnuplot.h: static void ns3::Gnuplot2dDataset::SetDefaultErrorBars(ns3::Gnuplot2dDataset::ErrorBars errorBars) [member function]
    cls.add_method('SetDefaultErrorBars', 
                   'void', 
                   [param('ns3::Gnuplot2dDataset::ErrorBars', 'errorBars')], 
                   is_static=True)
    ## gnuplot.h: static void ns3::Gnuplot2dDataset::SetDefaultStyle(ns3::Gnuplot2dDataset::Style style) [member function]
    cls.add_method('SetDefaultStyle', 
                   'void', 
                   [param('ns3::Gnuplot2dDataset::Style', 'style')], 
                   is_static=True)
    ## gnuplot.h: void ns3::Gnuplot2dDataset::SetErrorBars(ns3::Gnuplot2dDataset::ErrorBars errorBars) [member function]
    cls.add_method('SetErrorBars', 
                   'void', 
                   [param('ns3::Gnuplot2dDataset::ErrorBars', 'errorBars')])
    ## gnuplot.h: void ns3::Gnuplot2dDataset::SetStyle(ns3::Gnuplot2dDataset::Style style) [member function]
    cls.add_method('SetStyle', 
                   'void', 
                   [param('ns3::Gnuplot2dDataset::Style', 'style')])
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
    ## gnuplot.h: void ns3::Gnuplot3dDataset::Add(double x, double y, double z) [member function]
    cls.add_method('Add', 
                   'void', 
                   [param('double', 'x'), param('double', 'y'), param('double', 'z')])
    ## gnuplot.h: void ns3::Gnuplot3dDataset::AddEmptyLine() [member function]
    cls.add_method('AddEmptyLine', 
                   'void', 
                   [])
    ## gnuplot.h: static void ns3::Gnuplot3dDataset::SetDefaultStyle(std::string const & style) [member function]
    cls.add_method('SetDefaultStyle', 
                   'void', 
                   [param('std::string const &', 'style')], 
                   is_static=True)
    ## gnuplot.h: void ns3::Gnuplot3dDataset::SetStyle(std::string const & style) [member function]
    cls.add_method('SetStyle', 
                   'void', 
                   [param('std::string const &', 'style')])
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
    register_functions_ns3_FatalImpl(module.get_submodule('FatalImpl'), root_module)
    register_functions_ns3_addressUtils(module.get_submodule('addressUtils'), root_module)
    register_functions_ns3_aodv(module.get_submodule('aodv'), root_module)
    register_functions_ns3_dot11s(module.get_submodule('dot11s'), root_module)
    register_functions_ns3_dsdv(module.get_submodule('dsdv'), root_module)
    register_functions_ns3_flame(module.get_submodule('flame'), root_module)
    register_functions_ns3_internal(module.get_submodule('internal'), root_module)
    register_functions_ns3_olsr(module.get_submodule('olsr'), root_module)
    return

def register_functions_ns3_Config(module, root_module):
    return

def register_functions_ns3_FatalImpl(module, root_module):
    return

def register_functions_ns3_addressUtils(module, root_module):
    return

def register_functions_ns3_aodv(module, root_module):
    return

def register_functions_ns3_dot11s(module, root_module):
    return

def register_functions_ns3_dsdv(module, root_module):
    return

def register_functions_ns3_flame(module, root_module):
    return

def register_functions_ns3_internal(module, root_module):
    return

def register_functions_ns3_olsr(module, root_module):
    return

