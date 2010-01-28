from pybindgen import Module, FileCodeSink, param, retval, cppclass, typehandlers

def register_types(module):
    root_module = module.get_root()
    
    ## csma-channel.h: ns3::WireState [enumeration]
    module.add_enum('WireState', ['IDLE', 'TRANSMITTING', 'PROPAGATING'])
    ## backoff.h: ns3::Backoff [class]
    module.add_class('Backoff')
    ## csma-channel.h: ns3::CsmaDeviceRec [class]
    module.add_class('CsmaDeviceRec')
