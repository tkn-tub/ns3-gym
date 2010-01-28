from pybindgen import Module, FileCodeSink, param, retval, cppclass, typehandlers

def register_types(module):
    root_module = module.get_root()
    
    ## log.h: ns3::LogLevel [enumeration]
    module.add_enum('LogLevel', ['LOG_NONE', 'LOG_ERROR', 'LOG_LEVEL_ERROR', 'LOG_WARN', 'LOG_LEVEL_WARN', 'LOG_DEBUG', 'LOG_LEVEL_DEBUG', 'LOG_INFO', 'LOG_LEVEL_INFO', 'LOG_FUNCTION', 'LOG_LEVEL_FUNCTION', 'LOG_LOGIC', 'LOG_LEVEL_LOGIC', 'LOG_ALL', 'LOG_LEVEL_ALL', 'LOG_PREFIX_FUNC', 'LOG_PREFIX_TIME', 'LOG_PREFIX_NODE'])
    ## attribute-list.h: ns3::AttributeList [class]
    module.add_class('AttributeList')
    ## callback.h: ns3::CallbackBase [class]
    module.add_class('CallbackBase')
    ## command-line.h: ns3::CommandLine [class]
    module.add_class('CommandLine', allow_subclassing=True)
    ## system-mutex.h: ns3::CriticalSection [class]
    module.add_class('CriticalSection')
    ## global-value.h: ns3::GlobalValue [class]
    module.add_class('GlobalValue')
    ## int-to-type.h: ns3::IntToType<0> [struct]
    module.add_class('IntToType', template_parameters=['0'])
    ## int-to-type.h: ns3::IntToType<0>::v_e [enumeration]
    module.add_enum('v_e', ['value'], outer_class=root_module['ns3::IntToType< 0 >'])
    ## int-to-type.h: ns3::IntToType<1> [struct]
    module.add_class('IntToType', template_parameters=['1'])
    ## int-to-type.h: ns3::IntToType<1>::v_e [enumeration]
    module.add_enum('v_e', ['value'], outer_class=root_module['ns3::IntToType< 1 >'])
    ## int-to-type.h: ns3::IntToType<2> [struct]
    module.add_class('IntToType', template_parameters=['2'])
    ## int-to-type.h: ns3::IntToType<2>::v_e [enumeration]
    module.add_enum('v_e', ['value'], outer_class=root_module['ns3::IntToType< 2 >'])
    ## int-to-type.h: ns3::IntToType<3> [struct]
    module.add_class('IntToType', template_parameters=['3'])
    ## int-to-type.h: ns3::IntToType<3>::v_e [enumeration]
    module.add_enum('v_e', ['value'], outer_class=root_module['ns3::IntToType< 3 >'])
    ## int-to-type.h: ns3::IntToType<4> [struct]
    module.add_class('IntToType', template_parameters=['4'])
    ## int-to-type.h: ns3::IntToType<4>::v_e [enumeration]
    module.add_enum('v_e', ['value'], outer_class=root_module['ns3::IntToType< 4 >'])
    ## int-to-type.h: ns3::IntToType<5> [struct]
    module.add_class('IntToType', template_parameters=['5'])
    ## int-to-type.h: ns3::IntToType<5>::v_e [enumeration]
    module.add_enum('v_e', ['value'], outer_class=root_module['ns3::IntToType< 5 >'])
    ## int-to-type.h: ns3::IntToType<6> [struct]
    module.add_class('IntToType', template_parameters=['6'])
    ## int-to-type.h: ns3::IntToType<6>::v_e [enumeration]
    module.add_enum('v_e', ['value'], outer_class=root_module['ns3::IntToType< 6 >'])
    ## names.h: ns3::Names [class]
    module.add_class('Names')
    ## object-base.h: ns3::ObjectBase [class]
    module.add_class('ObjectBase', allow_subclassing=True)
    ## object.h: ns3::ObjectDeleter [struct]
    module.add_class('ObjectDeleter')
    ## object-factory.h: ns3::ObjectFactory [class]
    module.add_class('ObjectFactory')
    ## random-variable.h: ns3::RandomVariable [class]
    module.add_class('RandomVariable')
    ## rng-stream.h: ns3::RngStream [class]
    module.add_class('RngStream')
    ## random-variable.h: ns3::SeedManager [class]
    module.add_class('SeedManager')
    ## random-variable.h: ns3::SequentialVariable [class]
    module.add_class('SequentialVariable', parent=root_module['ns3::RandomVariable'])
    ## simple-ref-count.h: ns3::SimpleRefCount<ns3::Object, ns3::ObjectBase, ns3::ObjectDeleter> [class]
    module.add_class('SimpleRefCount', automatic_type_narrowing=True, template_parameters=['ns3::Object', 'ns3::ObjectBase', 'ns3::ObjectDeleter'], parent=root_module['ns3::ObjectBase'], memory_policy=cppclass.ReferenceCountingMethodsPolicy(incref_method='Ref', decref_method='Unref', peekref_method='GetReferenceCount'))
    ## system-condition.h: ns3::SystemCondition [class]
    module.add_class('SystemCondition')
    ## system-mutex.h: ns3::SystemMutex [class]
    module.add_class('SystemMutex')
    ## system-wall-clock-ms.h: ns3::SystemWallClockMs [class]
    module.add_class('SystemWallClockMs')
    ## test.h: ns3::TestCase [class]
    module.add_class('TestCase', allow_subclassing=True)
    ## test.h: ns3::TestRunner [class]
    module.add_class('TestRunner')
    ## test.h: ns3::TestSuite [class]
    module.add_class('TestSuite', allow_subclassing=True)
    ## test.h: ns3::TestSuite::TestType [enumeration]
    module.add_enum('TestType', ['BVT', 'UNIT', 'SYSTEM', 'EXAMPLE', 'PERFORMANCE'], outer_class=root_module['ns3::TestSuite'])
    ## random-variable.h: ns3::TriangularVariable [class]
    module.add_class('TriangularVariable', parent=root_module['ns3::RandomVariable'])
    ## type-id.h: ns3::TypeId [class]
    module.add_class('TypeId')
    ## type-id.h: ns3::TypeId::AttributeFlag [enumeration]
    module.add_enum('AttributeFlag', ['ATTR_GET', 'ATTR_SET', 'ATTR_CONSTRUCT', 'ATTR_SGC'], outer_class=root_module['ns3::TypeId'])
    ## type-id.h: ns3::TypeId::AttributeInfo [struct]
    module.add_class('AttributeInfo', outer_class=root_module['ns3::TypeId'])
    ## random-variable.h: ns3::UniformVariable [class]
    module.add_class('UniformVariable', parent=root_module['ns3::RandomVariable'])
    ## attribute-list.h: ns3::UnsafeAttributeList [class]
    module.add_class('UnsafeAttributeList')
    ## vector.h: ns3::Vector2D [class]
    module.add_class('Vector2D')
    ## vector.h: ns3::Vector3D [class]
    module.add_class('Vector3D')
    ## random-variable.h: ns3::WeibullVariable [class]
    module.add_class('WeibullVariable', parent=root_module['ns3::RandomVariable'])
