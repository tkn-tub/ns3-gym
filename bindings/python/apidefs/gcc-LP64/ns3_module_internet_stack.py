from pybindgen import Module, FileCodeSink, param, retval, cppclass, typehandlers

def register_types(module):
    root_module = module.get_root()
    
    ## ipv6-extension-header.h: ns3::OptionField [class]
    module.add_class('OptionField')
