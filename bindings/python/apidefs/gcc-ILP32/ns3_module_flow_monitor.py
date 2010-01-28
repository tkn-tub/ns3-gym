from pybindgen import Module, FileCodeSink, param, retval, cppclass, typehandlers

def register_types(module):
    root_module = module.get_root()
    
    ## histogram.h: ns3::Histogram [class]
    module.add_class('Histogram')
