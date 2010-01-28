from pybindgen import Module, FileCodeSink, param, retval, cppclass, typehandlers

def register_types(module):
    root_module = module.get_root()
    
    ## olsr-state.h: ns3::OlsrState [class]
    module.add_class('OlsrState')
