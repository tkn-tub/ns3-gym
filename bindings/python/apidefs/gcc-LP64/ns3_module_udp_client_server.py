from pybindgen import Module, FileCodeSink, param, retval, cppclass, typehandlers

def register_types(module):
    root_module = module.get_root()
    
    ## packet-loss-counter.h: ns3::PacketLossCounter [class]
    module.add_class('PacketLossCounter')
