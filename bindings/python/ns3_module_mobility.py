from pybindgen import Module, FileCodeSink, param, retval, cppclass

def register_types(module):
    root_module = module.get_root()
    
    ## constant-velocity-helper.h: ns3::ConstantVelocityHelper [class]
    module.add_class('ConstantVelocityHelper', allow_subclassing=False)
    ## rectangle.h: ns3::Rectangle [class]
    module.add_class('Rectangle')
    ## rectangle.h: ns3::Rectangle::Side [enumeration]
    module.add_enum('Side', ['RIGHT', 'LEFT', 'TOP', 'BOTTOM'], outer_class=root_module['ns3::Rectangle'])
    ## vector.h: ns3::Vector [class]
    module.add_class('Vector')
    ## position-allocator.h: ns3::PositionAllocator [class]
    module.add_class('PositionAllocator', parent=root_module['ns3::Object'])
    ## position-allocator.h: ns3::RandomDiscPositionAllocator [class]
    module.add_class('RandomDiscPositionAllocator', parent=root_module['ns3::PositionAllocator'])
    ## position-allocator.h: ns3::RandomRectanglePositionAllocator [class]
    module.add_class('RandomRectanglePositionAllocator', parent=root_module['ns3::PositionAllocator'])
    ## rectangle.h: ns3::RectangleChecker [class]
    module.add_class('RectangleChecker', parent=root_module['ns3::AttributeChecker'])
    ## rectangle.h: ns3::RectangleValue [class]
    module.add_class('RectangleValue', parent=root_module['ns3::AttributeValue'])
    ## vector.h: ns3::VectorChecker [class]
    module.add_class('VectorChecker', parent=root_module['ns3::AttributeChecker'])
    ## vector.h: ns3::VectorValue [class]
    module.add_class('VectorValue', parent=root_module['ns3::AttributeValue'])
    ## position-allocator.h: ns3::GridPositionAllocator [class]
    module.add_class('GridPositionAllocator', parent=root_module['ns3::PositionAllocator'])
    ## position-allocator.h: ns3::GridPositionAllocator::LayoutType [enumeration]
    module.add_enum('LayoutType', ['ROW_FIRST', 'COLUMN_FIRST'], outer_class=root_module['ns3::GridPositionAllocator'])
    ## position-allocator.h: ns3::ListPositionAllocator [class]
    module.add_class('ListPositionAllocator', parent=root_module['ns3::PositionAllocator'])
    ## mobility-model.h: ns3::MobilityModel [class]
    module.add_class('MobilityModel', parent=root_module['ns3::Object'])
    ## random-direction-2d-mobility-model.h: ns3::RandomDirection2dMobilityModel [class]
    module.add_class('RandomDirection2dMobilityModel', parent=root_module['ns3::MobilityModel'])
    ## random-walk-2d-mobility-model.h: ns3::RandomWalk2dMobilityModel [class]
    module.add_class('RandomWalk2dMobilityModel', parent=root_module['ns3::MobilityModel'])
    ## random-walk-2d-mobility-model.h: ns3::RandomWalk2dMobilityModel::Mode [enumeration]
    module.add_enum('Mode', ['MODE_DISTANCE', 'MODE_TIME'], outer_class=root_module['ns3::RandomWalk2dMobilityModel'])
    ## random-waypoint-mobility-model.h: ns3::RandomWaypointMobilityModel [class]
    module.add_class('RandomWaypointMobilityModel', parent=root_module['ns3::MobilityModel'])
    ## constant-acceleration-mobility-model.h: ns3::ConstantAccelerationMobilityModel [class]
    module.add_class('ConstantAccelerationMobilityModel', parent=root_module['ns3::MobilityModel'])
    ## constant-position-mobility-model.h: ns3::ConstantPositionMobilityModel [class]
    module.add_class('ConstantPositionMobilityModel', parent=root_module['ns3::MobilityModel'])
    ## constant-velocity-mobility-model.h: ns3::ConstantVelocityMobilityModel [class]
    module.add_class('ConstantVelocityMobilityModel', parent=root_module['ns3::MobilityModel'])
    ## hierarchical-mobility-model.h: ns3::HierarchicalMobilityModel [class]
    module.add_class('HierarchicalMobilityModel', parent=root_module['ns3::MobilityModel'])
    
    ## Register a nested module for the namespace Config
    
    nested_module = module.add_cpp_namespace('Config')
    register_types_ns3_Config(nested_module)
    
    
    ## Register a nested module for the namespace TimeStepPrecision
    
    nested_module = module.add_cpp_namespace('TimeStepPrecision')
    register_types_ns3_TimeStepPrecision(nested_module)
    
    
    ## Register a nested module for the namespace internal
    
    nested_module = module.add_cpp_namespace('internal')
    register_types_ns3_internal(nested_module)
    
    
    ## Register a nested module for the namespace olsr
    
    nested_module = module.add_cpp_namespace('olsr')
    register_types_ns3_olsr(nested_module)
    

def register_types_ns3_Config(module):
    root_module = module.get_root()
    

def register_types_ns3_TimeStepPrecision(module):
    root_module = module.get_root()
    

def register_types_ns3_internal(module):
    root_module = module.get_root()
    

def register_types_ns3_olsr(module):
    root_module = module.get_root()
    

def register_methods(root_module):
    register_Ns3ConstantVelocityHelper_methods(root_module, root_module['ns3::ConstantVelocityHelper'])
    register_Ns3Rectangle_methods(root_module, root_module['ns3::Rectangle'])
    register_Ns3Vector_methods(root_module, root_module['ns3::Vector'])
    register_Ns3PositionAllocator_methods(root_module, root_module['ns3::PositionAllocator'])
    register_Ns3RandomDiscPositionAllocator_methods(root_module, root_module['ns3::RandomDiscPositionAllocator'])
    register_Ns3RandomRectanglePositionAllocator_methods(root_module, root_module['ns3::RandomRectanglePositionAllocator'])
    register_Ns3RectangleChecker_methods(root_module, root_module['ns3::RectangleChecker'])
    register_Ns3RectangleValue_methods(root_module, root_module['ns3::RectangleValue'])
    register_Ns3VectorChecker_methods(root_module, root_module['ns3::VectorChecker'])
    register_Ns3VectorValue_methods(root_module, root_module['ns3::VectorValue'])
    register_Ns3GridPositionAllocator_methods(root_module, root_module['ns3::GridPositionAllocator'])
    register_Ns3ListPositionAllocator_methods(root_module, root_module['ns3::ListPositionAllocator'])
    register_Ns3MobilityModel_methods(root_module, root_module['ns3::MobilityModel'])
    register_Ns3RandomDirection2dMobilityModel_methods(root_module, root_module['ns3::RandomDirection2dMobilityModel'])
    register_Ns3RandomWalk2dMobilityModel_methods(root_module, root_module['ns3::RandomWalk2dMobilityModel'])
    register_Ns3RandomWaypointMobilityModel_methods(root_module, root_module['ns3::RandomWaypointMobilityModel'])
    register_Ns3ConstantAccelerationMobilityModel_methods(root_module, root_module['ns3::ConstantAccelerationMobilityModel'])
    register_Ns3ConstantPositionMobilityModel_methods(root_module, root_module['ns3::ConstantPositionMobilityModel'])
    register_Ns3ConstantVelocityMobilityModel_methods(root_module, root_module['ns3::ConstantVelocityMobilityModel'])
    register_Ns3HierarchicalMobilityModel_methods(root_module, root_module['ns3::HierarchicalMobilityModel'])
    return

def register_Ns3ConstantVelocityHelper_methods(root_module, cls):
    ## constant-velocity-helper.h: ns3::ConstantVelocityHelper::ConstantVelocityHelper(ns3::ConstantVelocityHelper const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::ConstantVelocityHelper const &', 'arg0')])
    ## constant-velocity-helper.h: ns3::ConstantVelocityHelper::ConstantVelocityHelper() [constructor]
    cls.add_constructor([])
    ## constant-velocity-helper.h: ns3::ConstantVelocityHelper::ConstantVelocityHelper(ns3::Vector const & position) [constructor]
    cls.add_constructor([param('ns3::Vector const &', 'position')])
    ## constant-velocity-helper.h: ns3::ConstantVelocityHelper::ConstantVelocityHelper(ns3::Vector const & position, ns3::Vector const & vel) [constructor]
    cls.add_constructor([param('ns3::Vector const &', 'position'), param('ns3::Vector const &', 'vel')])
    ## constant-velocity-helper.h: void ns3::ConstantVelocityHelper::SetPosition(ns3::Vector const & position) [member function]
    cls.add_method('SetPosition', 
                   'void', 
                   [param('ns3::Vector const &', 'position')])
    ## constant-velocity-helper.h: ns3::Vector ns3::ConstantVelocityHelper::GetCurrentPosition() const [member function]
    cls.add_method('GetCurrentPosition', 
                   'ns3::Vector', 
                   [], 
                   is_const=True)
    ## constant-velocity-helper.h: ns3::Vector ns3::ConstantVelocityHelper::GetVelocity() const [member function]
    cls.add_method('GetVelocity', 
                   'ns3::Vector', 
                   [], 
                   is_const=True)
    ## constant-velocity-helper.h: void ns3::ConstantVelocityHelper::SetVelocity(ns3::Vector const & vel) [member function]
    cls.add_method('SetVelocity', 
                   'void', 
                   [param('ns3::Vector const &', 'vel')])
    ## constant-velocity-helper.h: void ns3::ConstantVelocityHelper::Pause() [member function]
    cls.add_method('Pause', 
                   'void', 
                   [])
    ## constant-velocity-helper.h: void ns3::ConstantVelocityHelper::Unpause() [member function]
    cls.add_method('Unpause', 
                   'void', 
                   [])
    ## constant-velocity-helper.h: void ns3::ConstantVelocityHelper::UpdateWithBounds(ns3::Rectangle const & rectangle) const [member function]
    cls.add_method('UpdateWithBounds', 
                   'void', 
                   [param('ns3::Rectangle const &', 'rectangle')], 
                   is_const=True)
    ## constant-velocity-helper.h: void ns3::ConstantVelocityHelper::Update() const [member function]
    cls.add_method('Update', 
                   'void', 
                   [], 
                   is_const=True)
    return

def register_Ns3Rectangle_methods(root_module, cls):
    cls.add_output_stream_operator()
    ## rectangle.h: ns3::Rectangle::Rectangle(ns3::Rectangle const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::Rectangle const &', 'arg0')])
    ## rectangle.h: ns3::Rectangle::Rectangle(double _xMin, double _xMax, double _yMin, double _yMax) [constructor]
    cls.add_constructor([param('double', '_xMin'), param('double', '_xMax'), param('double', '_yMin'), param('double', '_yMax')])
    ## rectangle.h: ns3::Rectangle::Rectangle() [constructor]
    cls.add_constructor([])
    ## rectangle.h: ns3::Vector ns3::Rectangle::CalculateIntersection(ns3::Vector const & current, ns3::Vector const & speed) const [member function]
    cls.add_method('CalculateIntersection', 
                   'ns3::Vector', 
                   [param('ns3::Vector const &', 'current'), param('ns3::Vector const &', 'speed')], 
                   is_const=True)
    ## rectangle.h: ns3::Rectangle::Side ns3::Rectangle::GetClosestSide(ns3::Vector const & position) const [member function]
    cls.add_method('GetClosestSide', 
                   'ns3::Rectangle::Side', 
                   [param('ns3::Vector const &', 'position')], 
                   is_const=True)
    ## rectangle.h: bool ns3::Rectangle::IsInside(ns3::Vector const & position) const [member function]
    cls.add_method('IsInside', 
                   'bool', 
                   [param('ns3::Vector const &', 'position')], 
                   is_const=True)
    ## rectangle.h: ns3::Rectangle::xMax [variable]
    cls.add_instance_attribute('xMax', 'double', is_const=False)
    ## rectangle.h: ns3::Rectangle::xMin [variable]
    cls.add_instance_attribute('xMin', 'double', is_const=False)
    ## rectangle.h: ns3::Rectangle::yMax [variable]
    cls.add_instance_attribute('yMax', 'double', is_const=False)
    ## rectangle.h: ns3::Rectangle::yMin [variable]
    cls.add_instance_attribute('yMin', 'double', is_const=False)
    return

def register_Ns3Vector_methods(root_module, cls):
    cls.add_output_stream_operator()
    ## vector.h: ns3::Vector::Vector(ns3::Vector const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::Vector const &', 'arg0')])
    ## vector.h: ns3::Vector::Vector(double _x, double _y, double _z) [constructor]
    cls.add_constructor([param('double', '_x'), param('double', '_y'), param('double', '_z')])
    ## vector.h: ns3::Vector::Vector() [constructor]
    cls.add_constructor([])
    ## vector.h: ns3::Vector::x [variable]
    cls.add_instance_attribute('x', 'double', is_const=False)
    ## vector.h: ns3::Vector::y [variable]
    cls.add_instance_attribute('y', 'double', is_const=False)
    ## vector.h: ns3::Vector::z [variable]
    cls.add_instance_attribute('z', 'double', is_const=False)
    return

def register_Ns3PositionAllocator_methods(root_module, cls):
    ## position-allocator.h: ns3::PositionAllocator::PositionAllocator(ns3::PositionAllocator const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::PositionAllocator const &', 'arg0')])
    ## position-allocator.h: static ns3::TypeId ns3::PositionAllocator::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## position-allocator.h: ns3::PositionAllocator::PositionAllocator() [constructor]
    cls.add_constructor([])
    ## position-allocator.h: ns3::Vector ns3::PositionAllocator::GetNext() const [member function]
    cls.add_method('GetNext', 
                   'ns3::Vector', 
                   [], 
                   is_pure_virtual=True, is_const=True, is_virtual=True)
    return

def register_Ns3RandomDiscPositionAllocator_methods(root_module, cls):
    ## position-allocator.h: ns3::RandomDiscPositionAllocator::RandomDiscPositionAllocator(ns3::RandomDiscPositionAllocator const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::RandomDiscPositionAllocator const &', 'arg0')])
    ## position-allocator.h: static ns3::TypeId ns3::RandomDiscPositionAllocator::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## position-allocator.h: ns3::RandomDiscPositionAllocator::RandomDiscPositionAllocator() [constructor]
    cls.add_constructor([])
    ## position-allocator.h: void ns3::RandomDiscPositionAllocator::SetTheta(ns3::RandomVariable theta) [member function]
    cls.add_method('SetTheta', 
                   'void', 
                   [param('ns3::RandomVariable', 'theta')])
    ## position-allocator.h: void ns3::RandomDiscPositionAllocator::SetRho(ns3::RandomVariable rho) [member function]
    cls.add_method('SetRho', 
                   'void', 
                   [param('ns3::RandomVariable', 'rho')])
    ## position-allocator.h: void ns3::RandomDiscPositionAllocator::SetX(double x) [member function]
    cls.add_method('SetX', 
                   'void', 
                   [param('double', 'x')])
    ## position-allocator.h: void ns3::RandomDiscPositionAllocator::SetY(double y) [member function]
    cls.add_method('SetY', 
                   'void', 
                   [param('double', 'y')])
    ## position-allocator.h: ns3::Vector ns3::RandomDiscPositionAllocator::GetNext() const [member function]
    cls.add_method('GetNext', 
                   'ns3::Vector', 
                   [], 
                   is_const=True, is_virtual=True)
    return

def register_Ns3RandomRectanglePositionAllocator_methods(root_module, cls):
    ## position-allocator.h: ns3::RandomRectanglePositionAllocator::RandomRectanglePositionAllocator(ns3::RandomRectanglePositionAllocator const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::RandomRectanglePositionAllocator const &', 'arg0')])
    ## position-allocator.h: static ns3::TypeId ns3::RandomRectanglePositionAllocator::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## position-allocator.h: ns3::RandomRectanglePositionAllocator::RandomRectanglePositionAllocator() [constructor]
    cls.add_constructor([])
    ## position-allocator.h: void ns3::RandomRectanglePositionAllocator::SetX(ns3::RandomVariable x) [member function]
    cls.add_method('SetX', 
                   'void', 
                   [param('ns3::RandomVariable', 'x')])
    ## position-allocator.h: void ns3::RandomRectanglePositionAllocator::SetY(ns3::RandomVariable y) [member function]
    cls.add_method('SetY', 
                   'void', 
                   [param('ns3::RandomVariable', 'y')])
    ## position-allocator.h: ns3::Vector ns3::RandomRectanglePositionAllocator::GetNext() const [member function]
    cls.add_method('GetNext', 
                   'ns3::Vector', 
                   [], 
                   is_const=True, is_virtual=True)
    return

def register_Ns3RectangleChecker_methods(root_module, cls):
    ## rectangle.h: ns3::RectangleChecker::RectangleChecker(ns3::RectangleChecker const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::RectangleChecker const &', 'arg0')])
    ## rectangle.h: ns3::RectangleChecker::RectangleChecker() [constructor]
    cls.add_constructor([])
    return

def register_Ns3RectangleValue_methods(root_module, cls):
    ## rectangle.h: ns3::RectangleValue::RectangleValue(ns3::RectangleValue const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::RectangleValue const &', 'arg0')])
    ## rectangle.h: ns3::RectangleValue::RectangleValue() [constructor]
    cls.add_constructor([])
    ## rectangle.h: ns3::RectangleValue::RectangleValue(ns3::Rectangle const & value) [constructor]
    cls.add_constructor([param('ns3::Rectangle const &', 'value')])
    ## rectangle.h: void ns3::RectangleValue::Set(ns3::Rectangle const & value) [member function]
    cls.add_method('Set', 
                   'void', 
                   [param('ns3::Rectangle const &', 'value')])
    ## rectangle.h: ns3::Rectangle ns3::RectangleValue::Get() const [member function]
    cls.add_method('Get', 
                   'ns3::Rectangle', 
                   [], 
                   is_const=True)
    ## rectangle.h: ns3::Ptr<ns3::AttributeValue> ns3::RectangleValue::Copy() const [member function]
    cls.add_method('Copy', 
                   'ns3::Ptr< ns3::AttributeValue >', 
                   [], 
                   is_const=True, is_virtual=True)
    ## rectangle.h: std::string ns3::RectangleValue::SerializeToString(ns3::Ptr<ns3::AttributeChecker const> checker) const [member function]
    cls.add_method('SerializeToString', 
                   'std::string', 
                   [param('ns3::Ptr< ns3::AttributeChecker const >', 'checker')], 
                   is_const=True, is_virtual=True)
    ## rectangle.h: bool ns3::RectangleValue::DeserializeFromString(std::string value, ns3::Ptr<ns3::AttributeChecker const> checker) [member function]
    cls.add_method('DeserializeFromString', 
                   'bool', 
                   [param('std::string', 'value'), param('ns3::Ptr< ns3::AttributeChecker const >', 'checker')], 
                   is_virtual=True)
    return

def register_Ns3VectorChecker_methods(root_module, cls):
    ## vector.h: ns3::VectorChecker::VectorChecker(ns3::VectorChecker const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::VectorChecker const &', 'arg0')])
    ## vector.h: ns3::VectorChecker::VectorChecker() [constructor]
    cls.add_constructor([])
    return

def register_Ns3VectorValue_methods(root_module, cls):
    ## vector.h: ns3::VectorValue::VectorValue(ns3::VectorValue const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::VectorValue const &', 'arg0')])
    ## vector.h: ns3::VectorValue::VectorValue() [constructor]
    cls.add_constructor([])
    ## vector.h: ns3::VectorValue::VectorValue(ns3::Vector const & value) [constructor]
    cls.add_constructor([param('ns3::Vector const &', 'value')])
    ## vector.h: void ns3::VectorValue::Set(ns3::Vector const & value) [member function]
    cls.add_method('Set', 
                   'void', 
                   [param('ns3::Vector const &', 'value')])
    ## vector.h: ns3::Vector ns3::VectorValue::Get() const [member function]
    cls.add_method('Get', 
                   'ns3::Vector', 
                   [], 
                   is_const=True)
    ## vector.h: ns3::Ptr<ns3::AttributeValue> ns3::VectorValue::Copy() const [member function]
    cls.add_method('Copy', 
                   'ns3::Ptr< ns3::AttributeValue >', 
                   [], 
                   is_const=True, is_virtual=True)
    ## vector.h: std::string ns3::VectorValue::SerializeToString(ns3::Ptr<ns3::AttributeChecker const> checker) const [member function]
    cls.add_method('SerializeToString', 
                   'std::string', 
                   [param('ns3::Ptr< ns3::AttributeChecker const >', 'checker')], 
                   is_const=True, is_virtual=True)
    ## vector.h: bool ns3::VectorValue::DeserializeFromString(std::string value, ns3::Ptr<ns3::AttributeChecker const> checker) [member function]
    cls.add_method('DeserializeFromString', 
                   'bool', 
                   [param('std::string', 'value'), param('ns3::Ptr< ns3::AttributeChecker const >', 'checker')], 
                   is_virtual=True)
    return

def register_Ns3GridPositionAllocator_methods(root_module, cls):
    ## position-allocator.h: ns3::GridPositionAllocator::GridPositionAllocator(ns3::GridPositionAllocator const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::GridPositionAllocator const &', 'arg0')])
    ## position-allocator.h: static ns3::TypeId ns3::GridPositionAllocator::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## position-allocator.h: ns3::GridPositionAllocator::GridPositionAllocator() [constructor]
    cls.add_constructor([])
    ## position-allocator.h: void ns3::GridPositionAllocator::SetMinX(double xMin) [member function]
    cls.add_method('SetMinX', 
                   'void', 
                   [param('double', 'xMin')])
    ## position-allocator.h: void ns3::GridPositionAllocator::SetMinY(double yMin) [member function]
    cls.add_method('SetMinY', 
                   'void', 
                   [param('double', 'yMin')])
    ## position-allocator.h: void ns3::GridPositionAllocator::SetDeltaX(double deltaX) [member function]
    cls.add_method('SetDeltaX', 
                   'void', 
                   [param('double', 'deltaX')])
    ## position-allocator.h: void ns3::GridPositionAllocator::SetDeltaY(double deltaY) [member function]
    cls.add_method('SetDeltaY', 
                   'void', 
                   [param('double', 'deltaY')])
    ## position-allocator.h: void ns3::GridPositionAllocator::SetN(uint32_t n) [member function]
    cls.add_method('SetN', 
                   'void', 
                   [param('uint32_t', 'n')])
    ## position-allocator.h: void ns3::GridPositionAllocator::SetLayoutType(ns3::GridPositionAllocator::LayoutType layoutType) [member function]
    cls.add_method('SetLayoutType', 
                   'void', 
                   [param('ns3::GridPositionAllocator::LayoutType', 'layoutType')])
    ## position-allocator.h: double ns3::GridPositionAllocator::GetMinX() const [member function]
    cls.add_method('GetMinX', 
                   'double', 
                   [], 
                   is_const=True)
    ## position-allocator.h: double ns3::GridPositionAllocator::GetMinY() const [member function]
    cls.add_method('GetMinY', 
                   'double', 
                   [], 
                   is_const=True)
    ## position-allocator.h: double ns3::GridPositionAllocator::GetDeltaX() const [member function]
    cls.add_method('GetDeltaX', 
                   'double', 
                   [], 
                   is_const=True)
    ## position-allocator.h: double ns3::GridPositionAllocator::GetDeltaY() const [member function]
    cls.add_method('GetDeltaY', 
                   'double', 
                   [], 
                   is_const=True)
    ## position-allocator.h: uint32_t ns3::GridPositionAllocator::GetN() const [member function]
    cls.add_method('GetN', 
                   'uint32_t', 
                   [], 
                   is_const=True)
    ## position-allocator.h: ns3::GridPositionAllocator::LayoutType ns3::GridPositionAllocator::GetLayoutType() const [member function]
    cls.add_method('GetLayoutType', 
                   'ns3::GridPositionAllocator::LayoutType', 
                   [], 
                   is_const=True)
    ## position-allocator.h: ns3::Vector ns3::GridPositionAllocator::GetNext() const [member function]
    cls.add_method('GetNext', 
                   'ns3::Vector', 
                   [], 
                   is_const=True, is_virtual=True)
    return

def register_Ns3ListPositionAllocator_methods(root_module, cls):
    ## position-allocator.h: ns3::ListPositionAllocator::ListPositionAllocator(ns3::ListPositionAllocator const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::ListPositionAllocator const &', 'arg0')])
    ## position-allocator.h: static ns3::TypeId ns3::ListPositionAllocator::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## position-allocator.h: ns3::ListPositionAllocator::ListPositionAllocator() [constructor]
    cls.add_constructor([])
    ## position-allocator.h: void ns3::ListPositionAllocator::Add(ns3::Vector v) [member function]
    cls.add_method('Add', 
                   'void', 
                   [param('ns3::Vector', 'v')])
    ## position-allocator.h: ns3::Vector ns3::ListPositionAllocator::GetNext() const [member function]
    cls.add_method('GetNext', 
                   'ns3::Vector', 
                   [], 
                   is_const=True, is_virtual=True)
    return

def register_Ns3MobilityModel_methods(root_module, cls):
    ## mobility-model.h: ns3::MobilityModel::MobilityModel(ns3::MobilityModel const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::MobilityModel const &', 'arg0')])
    ## mobility-model.h: static ns3::TypeId ns3::MobilityModel::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## mobility-model.h: ns3::MobilityModel::MobilityModel() [constructor]
    cls.add_constructor([])
    ## mobility-model.h: ns3::Vector ns3::MobilityModel::GetPosition() const [member function]
    cls.add_method('GetPosition', 
                   'ns3::Vector', 
                   [], 
                   is_const=True)
    ## mobility-model.h: void ns3::MobilityModel::SetPosition(ns3::Vector const & position) [member function]
    cls.add_method('SetPosition', 
                   'void', 
                   [param('ns3::Vector const &', 'position')])
    ## mobility-model.h: ns3::Vector ns3::MobilityModel::GetVelocity() const [member function]
    cls.add_method('GetVelocity', 
                   'ns3::Vector', 
                   [], 
                   is_const=True)
    ## mobility-model.h: double ns3::MobilityModel::GetDistanceFrom(ns3::Ptr<const ns3::MobilityModel> position) const [member function]
    cls.add_method('GetDistanceFrom', 
                   'double', 
                   [param('ns3::Ptr< ns3::MobilityModel const >', 'position')], 
                   is_const=True)
    ## mobility-model.h: void ns3::MobilityModel::NotifyCourseChange() const [member function]
    cls.add_method('NotifyCourseChange', 
                   'void', 
                   [], 
                   is_const=True, visibility='protected')
    ## mobility-model.h: ns3::Vector ns3::MobilityModel::DoGetPosition() const [member function]
    cls.add_method('DoGetPosition', 
                   'ns3::Vector', 
                   [], 
                   is_pure_virtual=True, is_const=True, visibility='private', is_virtual=True)
    ## mobility-model.h: void ns3::MobilityModel::DoSetPosition(ns3::Vector const & position) [member function]
    cls.add_method('DoSetPosition', 
                   'void', 
                   [param('ns3::Vector const &', 'position')], 
                   is_pure_virtual=True, visibility='private', is_virtual=True)
    ## mobility-model.h: ns3::Vector ns3::MobilityModel::DoGetVelocity() const [member function]
    cls.add_method('DoGetVelocity', 
                   'ns3::Vector', 
                   [], 
                   is_pure_virtual=True, is_const=True, visibility='private', is_virtual=True)
    return

def register_Ns3RandomDirection2dMobilityModel_methods(root_module, cls):
    ## random-direction-2d-mobility-model.h: ns3::RandomDirection2dMobilityModel::RandomDirection2dMobilityModel(ns3::RandomDirection2dMobilityModel const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::RandomDirection2dMobilityModel const &', 'arg0')])
    ## random-direction-2d-mobility-model.h: static ns3::TypeId ns3::RandomDirection2dMobilityModel::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## random-direction-2d-mobility-model.h: ns3::RandomDirection2dMobilityModel::RandomDirection2dMobilityModel() [constructor]
    cls.add_constructor([])
    ## random-direction-2d-mobility-model.h: void ns3::RandomDirection2dMobilityModel::DoDispose() [member function]
    cls.add_method('DoDispose', 
                   'void', 
                   [], 
                   visibility='private', is_virtual=True)
    ## random-direction-2d-mobility-model.h: ns3::Vector ns3::RandomDirection2dMobilityModel::DoGetPosition() const [member function]
    cls.add_method('DoGetPosition', 
                   'ns3::Vector', 
                   [], 
                   is_const=True, visibility='private', is_virtual=True)
    ## random-direction-2d-mobility-model.h: void ns3::RandomDirection2dMobilityModel::DoSetPosition(ns3::Vector const & position) [member function]
    cls.add_method('DoSetPosition', 
                   'void', 
                   [param('ns3::Vector const &', 'position')], 
                   visibility='private', is_virtual=True)
    ## random-direction-2d-mobility-model.h: ns3::Vector ns3::RandomDirection2dMobilityModel::DoGetVelocity() const [member function]
    cls.add_method('DoGetVelocity', 
                   'ns3::Vector', 
                   [], 
                   is_const=True, visibility='private', is_virtual=True)
    return

def register_Ns3RandomWalk2dMobilityModel_methods(root_module, cls):
    ## random-walk-2d-mobility-model.h: ns3::RandomWalk2dMobilityModel::RandomWalk2dMobilityModel(ns3::RandomWalk2dMobilityModel const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::RandomWalk2dMobilityModel const &', 'arg0')])
    ## random-walk-2d-mobility-model.h: static ns3::TypeId ns3::RandomWalk2dMobilityModel::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## random-walk-2d-mobility-model.h: ns3::RandomWalk2dMobilityModel::RandomWalk2dMobilityModel() [constructor]
    cls.add_constructor([])
    ## random-walk-2d-mobility-model.h: void ns3::RandomWalk2dMobilityModel::DoDispose() [member function]
    cls.add_method('DoDispose', 
                   'void', 
                   [], 
                   visibility='private', is_virtual=True)
    ## random-walk-2d-mobility-model.h: ns3::Vector ns3::RandomWalk2dMobilityModel::DoGetPosition() const [member function]
    cls.add_method('DoGetPosition', 
                   'ns3::Vector', 
                   [], 
                   is_const=True, visibility='private', is_virtual=True)
    ## random-walk-2d-mobility-model.h: void ns3::RandomWalk2dMobilityModel::DoSetPosition(ns3::Vector const & position) [member function]
    cls.add_method('DoSetPosition', 
                   'void', 
                   [param('ns3::Vector const &', 'position')], 
                   visibility='private', is_virtual=True)
    ## random-walk-2d-mobility-model.h: ns3::Vector ns3::RandomWalk2dMobilityModel::DoGetVelocity() const [member function]
    cls.add_method('DoGetVelocity', 
                   'ns3::Vector', 
                   [], 
                   is_const=True, visibility='private', is_virtual=True)
    return

def register_Ns3RandomWaypointMobilityModel_methods(root_module, cls):
    ## random-waypoint-mobility-model.h: ns3::RandomWaypointMobilityModel::RandomWaypointMobilityModel(ns3::RandomWaypointMobilityModel const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::RandomWaypointMobilityModel const &', 'arg0')])
    ## random-waypoint-mobility-model.h: static ns3::TypeId ns3::RandomWaypointMobilityModel::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## random-waypoint-mobility-model.h: ns3::RandomWaypointMobilityModel::RandomWaypointMobilityModel() [constructor]
    cls.add_constructor([])
    ## random-waypoint-mobility-model.h: ns3::Vector ns3::RandomWaypointMobilityModel::DoGetPosition() const [member function]
    cls.add_method('DoGetPosition', 
                   'ns3::Vector', 
                   [], 
                   is_const=True, visibility='private', is_virtual=True)
    ## random-waypoint-mobility-model.h: void ns3::RandomWaypointMobilityModel::DoSetPosition(ns3::Vector const & position) [member function]
    cls.add_method('DoSetPosition', 
                   'void', 
                   [param('ns3::Vector const &', 'position')], 
                   visibility='private', is_virtual=True)
    ## random-waypoint-mobility-model.h: ns3::Vector ns3::RandomWaypointMobilityModel::DoGetVelocity() const [member function]
    cls.add_method('DoGetVelocity', 
                   'ns3::Vector', 
                   [], 
                   is_const=True, visibility='private', is_virtual=True)
    return

def register_Ns3ConstantAccelerationMobilityModel_methods(root_module, cls):
    ## constant-acceleration-mobility-model.h: ns3::ConstantAccelerationMobilityModel::ConstantAccelerationMobilityModel(ns3::ConstantAccelerationMobilityModel const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::ConstantAccelerationMobilityModel const &', 'arg0')])
    ## constant-acceleration-mobility-model.h: static ns3::TypeId ns3::ConstantAccelerationMobilityModel::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## constant-acceleration-mobility-model.h: ns3::ConstantAccelerationMobilityModel::ConstantAccelerationMobilityModel() [constructor]
    cls.add_constructor([])
    ## constant-acceleration-mobility-model.h: void ns3::ConstantAccelerationMobilityModel::SetVelocityAndAcceleration(ns3::Vector const & velocity, ns3::Vector const & acceleration) [member function]
    cls.add_method('SetVelocityAndAcceleration', 
                   'void', 
                   [param('ns3::Vector const &', 'velocity'), param('ns3::Vector const &', 'acceleration')])
    ## constant-acceleration-mobility-model.h: ns3::Vector ns3::ConstantAccelerationMobilityModel::DoGetPosition() const [member function]
    cls.add_method('DoGetPosition', 
                   'ns3::Vector', 
                   [], 
                   is_const=True, visibility='private', is_virtual=True)
    ## constant-acceleration-mobility-model.h: void ns3::ConstantAccelerationMobilityModel::DoSetPosition(ns3::Vector const & position) [member function]
    cls.add_method('DoSetPosition', 
                   'void', 
                   [param('ns3::Vector const &', 'position')], 
                   visibility='private', is_virtual=True)
    ## constant-acceleration-mobility-model.h: ns3::Vector ns3::ConstantAccelerationMobilityModel::DoGetVelocity() const [member function]
    cls.add_method('DoGetVelocity', 
                   'ns3::Vector', 
                   [], 
                   is_const=True, visibility='private', is_virtual=True)
    return

def register_Ns3ConstantPositionMobilityModel_methods(root_module, cls):
    ## constant-position-mobility-model.h: ns3::ConstantPositionMobilityModel::ConstantPositionMobilityModel(ns3::ConstantPositionMobilityModel const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::ConstantPositionMobilityModel const &', 'arg0')])
    ## constant-position-mobility-model.h: static ns3::TypeId ns3::ConstantPositionMobilityModel::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## constant-position-mobility-model.h: ns3::ConstantPositionMobilityModel::ConstantPositionMobilityModel() [constructor]
    cls.add_constructor([])
    ## constant-position-mobility-model.h: ns3::Vector ns3::ConstantPositionMobilityModel::DoGetPosition() const [member function]
    cls.add_method('DoGetPosition', 
                   'ns3::Vector', 
                   [], 
                   is_const=True, visibility='private', is_virtual=True)
    ## constant-position-mobility-model.h: void ns3::ConstantPositionMobilityModel::DoSetPosition(ns3::Vector const & position) [member function]
    cls.add_method('DoSetPosition', 
                   'void', 
                   [param('ns3::Vector const &', 'position')], 
                   visibility='private', is_virtual=True)
    ## constant-position-mobility-model.h: ns3::Vector ns3::ConstantPositionMobilityModel::DoGetVelocity() const [member function]
    cls.add_method('DoGetVelocity', 
                   'ns3::Vector', 
                   [], 
                   is_const=True, visibility='private', is_virtual=True)
    return

def register_Ns3ConstantVelocityMobilityModel_methods(root_module, cls):
    ## constant-velocity-mobility-model.h: ns3::ConstantVelocityMobilityModel::ConstantVelocityMobilityModel(ns3::ConstantVelocityMobilityModel const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::ConstantVelocityMobilityModel const &', 'arg0')])
    ## constant-velocity-mobility-model.h: static ns3::TypeId ns3::ConstantVelocityMobilityModel::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## constant-velocity-mobility-model.h: ns3::ConstantVelocityMobilityModel::ConstantVelocityMobilityModel() [constructor]
    cls.add_constructor([])
    ## constant-velocity-mobility-model.h: void ns3::ConstantVelocityMobilityModel::SetVelocity(ns3::Vector const & speed) [member function]
    cls.add_method('SetVelocity', 
                   'void', 
                   [param('ns3::Vector const &', 'speed')])
    ## constant-velocity-mobility-model.h: ns3::Vector ns3::ConstantVelocityMobilityModel::DoGetPosition() const [member function]
    cls.add_method('DoGetPosition', 
                   'ns3::Vector', 
                   [], 
                   is_const=True, visibility='private', is_virtual=True)
    ## constant-velocity-mobility-model.h: void ns3::ConstantVelocityMobilityModel::DoSetPosition(ns3::Vector const & position) [member function]
    cls.add_method('DoSetPosition', 
                   'void', 
                   [param('ns3::Vector const &', 'position')], 
                   visibility='private', is_virtual=True)
    ## constant-velocity-mobility-model.h: ns3::Vector ns3::ConstantVelocityMobilityModel::DoGetVelocity() const [member function]
    cls.add_method('DoGetVelocity', 
                   'ns3::Vector', 
                   [], 
                   is_const=True, visibility='private', is_virtual=True)
    return

def register_Ns3HierarchicalMobilityModel_methods(root_module, cls):
    ## hierarchical-mobility-model.h: ns3::HierarchicalMobilityModel::HierarchicalMobilityModel(ns3::HierarchicalMobilityModel const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::HierarchicalMobilityModel const &', 'arg0')])
    ## hierarchical-mobility-model.h: static ns3::TypeId ns3::HierarchicalMobilityModel::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## hierarchical-mobility-model.h: ns3::HierarchicalMobilityModel::HierarchicalMobilityModel() [constructor]
    cls.add_constructor([])
    ## hierarchical-mobility-model.h: ns3::Ptr<ns3::MobilityModel> ns3::HierarchicalMobilityModel::GetChild() const [member function]
    cls.add_method('GetChild', 
                   'ns3::Ptr< ns3::MobilityModel >', 
                   [], 
                   is_const=True)
    ## hierarchical-mobility-model.h: ns3::Ptr<ns3::MobilityModel> ns3::HierarchicalMobilityModel::GetParent() const [member function]
    cls.add_method('GetParent', 
                   'ns3::Ptr< ns3::MobilityModel >', 
                   [], 
                   is_const=True)
    ## hierarchical-mobility-model.h: void ns3::HierarchicalMobilityModel::SetChild(ns3::Ptr<ns3::MobilityModel> model) [member function]
    cls.add_method('SetChild', 
                   'void', 
                   [param('ns3::Ptr< ns3::MobilityModel >', 'model')])
    ## hierarchical-mobility-model.h: void ns3::HierarchicalMobilityModel::SetParent(ns3::Ptr<ns3::MobilityModel> model) [member function]
    cls.add_method('SetParent', 
                   'void', 
                   [param('ns3::Ptr< ns3::MobilityModel >', 'model')])
    ## hierarchical-mobility-model.h: ns3::Vector ns3::HierarchicalMobilityModel::DoGetPosition() const [member function]
    cls.add_method('DoGetPosition', 
                   'ns3::Vector', 
                   [], 
                   is_const=True, visibility='private', is_virtual=True)
    ## hierarchical-mobility-model.h: void ns3::HierarchicalMobilityModel::DoSetPosition(ns3::Vector const & position) [member function]
    cls.add_method('DoSetPosition', 
                   'void', 
                   [param('ns3::Vector const &', 'position')], 
                   visibility='private', is_virtual=True)
    ## hierarchical-mobility-model.h: ns3::Vector ns3::HierarchicalMobilityModel::DoGetVelocity() const [member function]
    cls.add_method('DoGetVelocity', 
                   'ns3::Vector', 
                   [], 
                   is_const=True, visibility='private', is_virtual=True)
    return

def register_functions(root_module):
    module = root_module
    ## vector.h: extern double ns3::CalculateDistance(ns3::Vector const & a, ns3::Vector const & b) [free function]
    module.add_function('CalculateDistance', 
                        'double', 
                        [param('ns3::Vector const &', 'a'), param('ns3::Vector const &', 'b')])
    ## rectangle.h: extern ns3::Ptr<ns3::AttributeChecker const> ns3::MakeRectangleChecker() [free function]
    module.add_function('MakeRectangleChecker', 
                        'ns3::Ptr< ns3::AttributeChecker const >', 
                        [])
    ## vector.h: extern ns3::Ptr<ns3::AttributeChecker const> ns3::MakeVectorChecker() [free function]
    module.add_function('MakeVectorChecker', 
                        'ns3::Ptr< ns3::AttributeChecker const >', 
                        [])
    register_functions_ns3_Config(module.get_submodule('Config'), root_module)
    register_functions_ns3_TimeStepPrecision(module.get_submodule('TimeStepPrecision'), root_module)
    register_functions_ns3_internal(module.get_submodule('internal'), root_module)
    register_functions_ns3_olsr(module.get_submodule('olsr'), root_module)
    return

def register_functions_ns3_Config(module, root_module):
    return

def register_functions_ns3_TimeStepPrecision(module, root_module):
    return

def register_functions_ns3_internal(module, root_module):
    return

def register_functions_ns3_olsr(module, root_module):
    return

