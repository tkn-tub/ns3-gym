from pybindgen import Module, FileCodeSink, param, retval, cppclass, typehandlers

def register_types(module):
    root_module = module.get_root()
    
    ## constant-velocity-helper.h: ns3::ConstantVelocityHelper [class]
    module.add_class('ConstantVelocityHelper', allow_subclassing=False)
    ## rectangle.h: ns3::Rectangle [class]
    module.add_class('Rectangle')
    ## rectangle.h: ns3::Rectangle::Side [enumeration]
    module.add_enum('Side', ['RIGHT', 'LEFT', 'TOP', 'BOTTOM'], outer_class=root_module['ns3::Rectangle'])
    ## waypoint.h: ns3::Waypoint [class]
    module.add_class('Waypoint')
