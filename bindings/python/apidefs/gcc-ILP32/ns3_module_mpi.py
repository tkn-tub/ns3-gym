from pybindgen import Module, FileCodeSink, param, retval, cppclass, typehandlers

def register_types(module):
    root_module = module.get_root()
    
    ## distributed-simulator-impl.h: ns3::LbtsMessage [class]
    module.add_class('LbtsMessage')
    ## mpi-interface.h: ns3::MpiInterface [class]
    module.add_class('MpiInterface')
    ## mpi-net-device.h: ns3::MpiNetDevice [class]
    module.add_class('MpiNetDevice', allow_subclassing=True)
    ## mpi-interface.h: ns3::SentBuffer [class]
    module.add_class('SentBuffer')
    ## distributed-simulator-impl.h: ns3::DistributedSimulatorImpl [class]
    module.add_class('DistributedSimulatorImpl', parent=root_module['ns3::SimulatorImpl'])
    
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
    register_Ns3LbtsMessage_methods(root_module, root_module['ns3::LbtsMessage'])
    register_Ns3MpiInterface_methods(root_module, root_module['ns3::MpiInterface'])
    register_Ns3MpiNetDevice_methods(root_module, root_module['ns3::MpiNetDevice'])
    register_Ns3SentBuffer_methods(root_module, root_module['ns3::SentBuffer'])
    register_Ns3DistributedSimulatorImpl_methods(root_module, root_module['ns3::DistributedSimulatorImpl'])
    return

def register_Ns3LbtsMessage_methods(root_module, cls):
    ## distributed-simulator-impl.h: ns3::LbtsMessage::LbtsMessage(ns3::LbtsMessage const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::LbtsMessage const &', 'arg0')])
    ## distributed-simulator-impl.h: ns3::LbtsMessage::LbtsMessage() [constructor]
    cls.add_constructor([])
    ## distributed-simulator-impl.h: ns3::LbtsMessage::LbtsMessage(uint32_t rxc, uint32_t txc, uint32_t id, ns3::Time const & t) [constructor]
    cls.add_constructor([param('uint32_t', 'rxc'), param('uint32_t', 'txc'), param('uint32_t', 'id'), param('ns3::Time const &', 't')])
    ## distributed-simulator-impl.h: uint32_t ns3::LbtsMessage::GetMyId() [member function]
    cls.add_method('GetMyId', 
                   'uint32_t', 
                   [])
    ## distributed-simulator-impl.h: uint32_t ns3::LbtsMessage::GetRxCount() [member function]
    cls.add_method('GetRxCount', 
                   'uint32_t', 
                   [])
    ## distributed-simulator-impl.h: ns3::Time ns3::LbtsMessage::GetSmallestTime() [member function]
    cls.add_method('GetSmallestTime', 
                   'ns3::Time', 
                   [])
    ## distributed-simulator-impl.h: uint32_t ns3::LbtsMessage::GetTxCount() [member function]
    cls.add_method('GetTxCount', 
                   'uint32_t', 
                   [])
    return

def register_Ns3MpiInterface_methods(root_module, cls):
    ## mpi-interface.h: ns3::MpiInterface::MpiInterface() [constructor]
    cls.add_constructor([])
    ## mpi-interface.h: ns3::MpiInterface::MpiInterface(ns3::MpiInterface const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::MpiInterface const &', 'arg0')])
    ## mpi-interface.h: static void ns3::MpiInterface::Destroy() [member function]
    cls.add_method('Destroy', 
                   'void', 
                   [], 
                   is_static=True)
    ## mpi-interface.h: static void ns3::MpiInterface::Enable(int * pargc, char * * * pargv) [member function]
    cls.add_method('Enable', 
                   'void', 
                   [param('int *', 'pargc'), param('char * * *', 'pargv')], 
                   is_static=True)
    ## mpi-interface.h: static uint32_t ns3::MpiInterface::GetRxCount() [member function]
    cls.add_method('GetRxCount', 
                   'uint32_t', 
                   [], 
                   is_static=True)
    ## mpi-interface.h: static uint32_t ns3::MpiInterface::GetSize() [member function]
    cls.add_method('GetSize', 
                   'uint32_t', 
                   [], 
                   is_static=True)
    ## mpi-interface.h: static uint32_t ns3::MpiInterface::GetSystemId() [member function]
    cls.add_method('GetSystemId', 
                   'uint32_t', 
                   [], 
                   is_static=True)
    ## mpi-interface.h: static uint32_t ns3::MpiInterface::GetTxCount() [member function]
    cls.add_method('GetTxCount', 
                   'uint32_t', 
                   [], 
                   is_static=True)
    ## mpi-interface.h: static bool ns3::MpiInterface::IsEnabled() [member function]
    cls.add_method('IsEnabled', 
                   'bool', 
                   [], 
                   is_static=True)
    ## mpi-interface.h: static void ns3::MpiInterface::ReceiveMessages() [member function]
    cls.add_method('ReceiveMessages', 
                   'void', 
                   [], 
                   is_static=True)
    ## mpi-interface.h: static void ns3::MpiInterface::SendPacket(ns3::Ptr<ns3::Packet> p, ns3::Time const & rxTime, uint32_t node, uint32_t dev) [member function]
    cls.add_method('SendPacket', 
                   'void', 
                   [param('ns3::Ptr< ns3::Packet >', 'p'), param('ns3::Time const &', 'rxTime'), param('uint32_t', 'node'), param('uint32_t', 'dev')], 
                   is_static=True)
    ## mpi-interface.h: static void ns3::MpiInterface::TestSendComplete() [member function]
    cls.add_method('TestSendComplete', 
                   'void', 
                   [], 
                   is_static=True)
    return

def register_Ns3MpiNetDevice_methods(root_module, cls):
    ## mpi-net-device.h: ns3::MpiNetDevice::MpiNetDevice() [constructor]
    cls.add_constructor([])
    ## mpi-net-device.h: ns3::MpiNetDevice::MpiNetDevice(ns3::MpiNetDevice const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::MpiNetDevice const &', 'arg0')])
    ## mpi-net-device.h: void ns3::MpiNetDevice::MpiReceive(ns3::Ptr<ns3::Packet> p) [member function]
    cls.add_method('MpiReceive', 
                   'void', 
                   [param('ns3::Ptr< ns3::Packet >', 'p')])
    ## mpi-net-device.h: void ns3::MpiNetDevice::DoMpiReceive(ns3::Ptr<ns3::Packet> p) [member function]
    cls.add_method('DoMpiReceive', 
                   'void', 
                   [param('ns3::Ptr< ns3::Packet >', 'p')], 
                   is_pure_virtual=True, visibility='protected', is_virtual=True)
    return

def register_Ns3SentBuffer_methods(root_module, cls):
    ## mpi-interface.h: ns3::SentBuffer::SentBuffer(ns3::SentBuffer const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::SentBuffer const &', 'arg0')])
    ## mpi-interface.h: ns3::SentBuffer::SentBuffer() [constructor]
    cls.add_constructor([])
    ## mpi-interface.h: uint8_t * ns3::SentBuffer::GetBuffer() [member function]
    cls.add_method('GetBuffer', 
                   'uint8_t *', 
                   [])
    ## mpi-interface.h: MPI_Request * ns3::SentBuffer::GetRequest() [member function]
    cls.add_method('GetRequest', 
                   'MPI_Request *', 
                   [])
    ## mpi-interface.h: void ns3::SentBuffer::SetBuffer(uint8_t * buffer) [member function]
    cls.add_method('SetBuffer', 
                   'void', 
                   [param('uint8_t *', 'buffer')])
    return

def register_Ns3DistributedSimulatorImpl_methods(root_module, cls):
    ## distributed-simulator-impl.h: ns3::DistributedSimulatorImpl::DistributedSimulatorImpl(ns3::DistributedSimulatorImpl const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::DistributedSimulatorImpl const &', 'arg0')])
    ## distributed-simulator-impl.h: ns3::DistributedSimulatorImpl::DistributedSimulatorImpl() [constructor]
    cls.add_constructor([])
    ## distributed-simulator-impl.h: void ns3::DistributedSimulatorImpl::Cancel(ns3::EventId const & ev) [member function]
    cls.add_method('Cancel', 
                   'void', 
                   [param('ns3::EventId const &', 'ev')], 
                   is_virtual=True)
    ## distributed-simulator-impl.h: void ns3::DistributedSimulatorImpl::Destroy() [member function]
    cls.add_method('Destroy', 
                   'void', 
                   [], 
                   is_virtual=True)
    ## distributed-simulator-impl.h: uint32_t ns3::DistributedSimulatorImpl::GetContext() const [member function]
    cls.add_method('GetContext', 
                   'uint32_t', 
                   [], 
                   is_const=True, is_virtual=True)
    ## distributed-simulator-impl.h: ns3::Time ns3::DistributedSimulatorImpl::GetDelayLeft(ns3::EventId const & id) const [member function]
    cls.add_method('GetDelayLeft', 
                   'ns3::Time', 
                   [param('ns3::EventId const &', 'id')], 
                   is_const=True, is_virtual=True)
    ## distributed-simulator-impl.h: ns3::Time ns3::DistributedSimulatorImpl::GetMaximumSimulationTime() const [member function]
    cls.add_method('GetMaximumSimulationTime', 
                   'ns3::Time', 
                   [], 
                   is_const=True, is_virtual=True)
    ## distributed-simulator-impl.h: uint32_t ns3::DistributedSimulatorImpl::GetSystemId() const [member function]
    cls.add_method('GetSystemId', 
                   'uint32_t', 
                   [], 
                   is_const=True, is_virtual=True)
    ## distributed-simulator-impl.h: static ns3::TypeId ns3::DistributedSimulatorImpl::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## distributed-simulator-impl.h: bool ns3::DistributedSimulatorImpl::IsExpired(ns3::EventId const & ev) const [member function]
    cls.add_method('IsExpired', 
                   'bool', 
                   [param('ns3::EventId const &', 'ev')], 
                   is_const=True, is_virtual=True)
    ## distributed-simulator-impl.h: bool ns3::DistributedSimulatorImpl::IsFinished() const [member function]
    cls.add_method('IsFinished', 
                   'bool', 
                   [], 
                   is_const=True, is_virtual=True)
    ## distributed-simulator-impl.h: ns3::Time ns3::DistributedSimulatorImpl::Next() const [member function]
    cls.add_method('Next', 
                   'ns3::Time', 
                   [], 
                   is_const=True, is_virtual=True)
    ## distributed-simulator-impl.h: ns3::Time ns3::DistributedSimulatorImpl::Now() const [member function]
    cls.add_method('Now', 
                   'ns3::Time', 
                   [], 
                   is_const=True, is_virtual=True)
    ## distributed-simulator-impl.h: void ns3::DistributedSimulatorImpl::Remove(ns3::EventId const & ev) [member function]
    cls.add_method('Remove', 
                   'void', 
                   [param('ns3::EventId const &', 'ev')], 
                   is_virtual=True)
    ## distributed-simulator-impl.h: void ns3::DistributedSimulatorImpl::Run() [member function]
    cls.add_method('Run', 
                   'void', 
                   [], 
                   is_virtual=True)
    ## distributed-simulator-impl.h: void ns3::DistributedSimulatorImpl::RunOneEvent() [member function]
    cls.add_method('RunOneEvent', 
                   'void', 
                   [], 
                   is_virtual=True)
    ## distributed-simulator-impl.h: ns3::EventId ns3::DistributedSimulatorImpl::Schedule(ns3::Time const & time, ns3::EventImpl * event) [member function]
    cls.add_method('Schedule', 
                   'ns3::EventId', 
                   [param('ns3::Time const &', 'time'), param('ns3::EventImpl *', 'event')], 
                   is_virtual=True)
    ## distributed-simulator-impl.h: ns3::EventId ns3::DistributedSimulatorImpl::ScheduleDestroy(ns3::EventImpl * event) [member function]
    cls.add_method('ScheduleDestroy', 
                   'ns3::EventId', 
                   [param('ns3::EventImpl *', 'event')], 
                   is_virtual=True)
    ## distributed-simulator-impl.h: ns3::EventId ns3::DistributedSimulatorImpl::ScheduleNow(ns3::EventImpl * event) [member function]
    cls.add_method('ScheduleNow', 
                   'ns3::EventId', 
                   [param('ns3::EventImpl *', 'event')], 
                   is_virtual=True)
    ## distributed-simulator-impl.h: void ns3::DistributedSimulatorImpl::ScheduleWithContext(uint32_t context, ns3::Time const & time, ns3::EventImpl * event) [member function]
    cls.add_method('ScheduleWithContext', 
                   'void', 
                   [param('uint32_t', 'context'), param('ns3::Time const &', 'time'), param('ns3::EventImpl *', 'event')], 
                   is_virtual=True)
    ## distributed-simulator-impl.h: void ns3::DistributedSimulatorImpl::SetScheduler(ns3::ObjectFactory schedulerFactory) [member function]
    cls.add_method('SetScheduler', 
                   'void', 
                   [param('ns3::ObjectFactory', 'schedulerFactory')], 
                   is_virtual=True)
    ## distributed-simulator-impl.h: void ns3::DistributedSimulatorImpl::Stop() [member function]
    cls.add_method('Stop', 
                   'void', 
                   [], 
                   is_virtual=True)
    ## distributed-simulator-impl.h: void ns3::DistributedSimulatorImpl::Stop(ns3::Time const & time) [member function]
    cls.add_method('Stop', 
                   'void', 
                   [param('ns3::Time const &', 'time')], 
                   is_virtual=True)
    ## distributed-simulator-impl.h: void ns3::DistributedSimulatorImpl::DoDispose() [member function]
    cls.add_method('DoDispose', 
                   'void', 
                   [], 
                   visibility='private', is_virtual=True)
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

