from pybindgen import Module, FileCodeSink, param, retval, cppclass, typehandlers

def register_types(module):
    root_module = module.get_root()
    
    ## event-id.h: ns3::EventId [class]
    module.add_class('EventId')
    ## high-precision-128.h: ns3::HighPrecision [class]
    module.add_class('HighPrecision')
    ## simulator.h: ns3::Simulator [class]
    module.add_class('Simulator', is_singleton=True)
    ## nstime.h: ns3::TimeUnit<-1> [class]
    module.add_class('TimeInvert')
    ## nstime.h: ns3::TimeUnit<0> [class]
    module.add_class('Scalar')
    ## nstime.h: ns3::TimeUnit<1> [class]
    module.add_class('Time')
    ## nstime.h: ns3::TimeUnit<2> [class]
    module.add_class('TimeSquare')
    ## timer.h: ns3::Timer [class]
    module.add_class('Timer')
    ## timer.h: ns3::Timer::DestroyPolicy [enumeration]
    module.add_enum('DestroyPolicy', ['CANCEL_ON_DESTROY', 'REMOVE_ON_DESTROY', 'CHECK_ON_DESTROY'], outer_class=root_module['ns3::Timer'])
    ## timer.h: ns3::Timer::State [enumeration]
    module.add_enum('State', ['RUNNING', 'EXPIRED', 'SUSPENDED'], outer_class=root_module['ns3::Timer'])
    ## timer-impl.h: ns3::TimerImpl [class]
    module.add_class('TimerImpl', allow_subclassing=True)
    ## watchdog.h: ns3::Watchdog [class]
    module.add_class('Watchdog')
