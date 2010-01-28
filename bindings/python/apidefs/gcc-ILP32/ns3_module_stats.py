from pybindgen import Module, FileCodeSink, param, retval, cppclass, typehandlers

def register_types(module):
    root_module = module.get_root()
    
    ## data-output-interface.h: ns3::DataOutputCallback [class]
    module.add_class('DataOutputCallback', allow_subclassing=True)
    ## data-calculator.h: ns3::StatisticalSummary [class]
    module.add_class('StatisticalSummary', allow_subclassing=True)
