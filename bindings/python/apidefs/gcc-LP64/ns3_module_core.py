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
    ## traced-value.h: ns3::TracedValue<double> [class]
    module.add_class('TracedValue', template_parameters=['double'])
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
    ## random-variable.h: ns3::ZetaVariable [class]
    module.add_class('ZetaVariable', parent=root_module['ns3::RandomVariable'])
    ## random-variable.h: ns3::ZipfVariable [class]
    module.add_class('ZipfVariable', parent=root_module['ns3::RandomVariable'])
    ## empty.h: ns3::empty [class]
    module.add_class('empty')
    ## random-variable.h: ns3::ConstantVariable [class]
    module.add_class('ConstantVariable', parent=root_module['ns3::RandomVariable'])
    ## random-variable.h: ns3::DeterministicVariable [class]
    module.add_class('DeterministicVariable', parent=root_module['ns3::RandomVariable'])
    ## random-variable.h: ns3::EmpiricalVariable [class]
    module.add_class('EmpiricalVariable', parent=root_module['ns3::RandomVariable'])
    ## random-variable.h: ns3::ErlangVariable [class]
    module.add_class('ErlangVariable', parent=root_module['ns3::RandomVariable'])
    ## random-variable.h: ns3::ExponentialVariable [class]
    module.add_class('ExponentialVariable', parent=root_module['ns3::RandomVariable'])
    ## random-variable.h: ns3::GammaVariable [class]
    module.add_class('GammaVariable', parent=root_module['ns3::RandomVariable'])
    ## random-variable.h: ns3::IntEmpiricalVariable [class]
    module.add_class('IntEmpiricalVariable', parent=root_module['ns3::EmpiricalVariable'])
    ## random-variable.h: ns3::LogNormalVariable [class]
    module.add_class('LogNormalVariable', parent=root_module['ns3::RandomVariable'])
    ## random-variable.h: ns3::NormalVariable [class]
    module.add_class('NormalVariable', parent=root_module['ns3::RandomVariable'])
    ## object.h: ns3::Object [class]
    module.add_class('Object', parent=root_module['ns3::SimpleRefCount< ns3::Object, ns3::ObjectBase, ns3::ObjectDeleter >'])
    ## object.h: ns3::Object::AggregateIterator [class]
    module.add_class('AggregateIterator', outer_class=root_module['ns3::Object'])
    ## random-variable.h: ns3::ParetoVariable [class]
    module.add_class('ParetoVariable', parent=root_module['ns3::RandomVariable'])
    ## simple-ref-count.h: ns3::SimpleRefCount<ns3::AttributeAccessor, ns3::empty, ns3::DefaultDeleter<ns3::AttributeAccessor> > [class]
    module.add_class('SimpleRefCount', automatic_type_narrowing=True, template_parameters=['ns3::AttributeAccessor', 'ns3::empty', 'ns3::DefaultDeleter<ns3::AttributeAccessor>'], parent=root_module['ns3::empty'], memory_policy=cppclass.ReferenceCountingMethodsPolicy(incref_method='Ref', decref_method='Unref', peekref_method='GetReferenceCount'))
    ## simple-ref-count.h: ns3::SimpleRefCount<ns3::AttributeChecker, ns3::empty, ns3::DefaultDeleter<ns3::AttributeChecker> > [class]
    module.add_class('SimpleRefCount', automatic_type_narrowing=True, template_parameters=['ns3::AttributeChecker', 'ns3::empty', 'ns3::DefaultDeleter<ns3::AttributeChecker>'], parent=root_module['ns3::empty'], memory_policy=cppclass.ReferenceCountingMethodsPolicy(incref_method='Ref', decref_method='Unref', peekref_method='GetReferenceCount'))
    ## simple-ref-count.h: ns3::SimpleRefCount<ns3::AttributeValue, ns3::empty, ns3::DefaultDeleter<ns3::AttributeValue> > [class]
    module.add_class('SimpleRefCount', automatic_type_narrowing=True, template_parameters=['ns3::AttributeValue', 'ns3::empty', 'ns3::DefaultDeleter<ns3::AttributeValue>'], parent=root_module['ns3::empty'], memory_policy=cppclass.ReferenceCountingMethodsPolicy(incref_method='Ref', decref_method='Unref', peekref_method='GetReferenceCount'))
    ## simple-ref-count.h: ns3::SimpleRefCount<ns3::CallbackImplBase, ns3::empty, ns3::DefaultDeleter<ns3::CallbackImplBase> > [class]
    module.add_class('SimpleRefCount', automatic_type_narrowing=True, template_parameters=['ns3::CallbackImplBase', 'ns3::empty', 'ns3::DefaultDeleter<ns3::CallbackImplBase>'], parent=root_module['ns3::empty'], memory_policy=cppclass.ReferenceCountingMethodsPolicy(incref_method='Ref', decref_method='Unref', peekref_method='GetReferenceCount'))
    ## simple-ref-count.h: ns3::SimpleRefCount<ns3::RefCountBase, ns3::empty, ns3::DefaultDeleter<ns3::RefCountBase> > [class]
    module.add_class('SimpleRefCount', automatic_type_narrowing=True, template_parameters=['ns3::RefCountBase', 'ns3::empty', 'ns3::DefaultDeleter<ns3::RefCountBase>'], parent=root_module['ns3::empty'], memory_policy=cppclass.ReferenceCountingMethodsPolicy(incref_method='Ref', decref_method='Unref', peekref_method='GetReferenceCount'))
    ## simple-ref-count.h: ns3::SimpleRefCount<ns3::SystemThread, ns3::empty, ns3::DefaultDeleter<ns3::SystemThread> > [class]
    module.add_class('SimpleRefCount', automatic_type_narrowing=True, template_parameters=['ns3::SystemThread', 'ns3::empty', 'ns3::DefaultDeleter<ns3::SystemThread>'], parent=root_module['ns3::empty'], memory_policy=cppclass.ReferenceCountingMethodsPolicy(incref_method='Ref', decref_method='Unref', peekref_method='GetReferenceCount'))
    ## simple-ref-count.h: ns3::SimpleRefCount<ns3::TraceSourceAccessor, ns3::empty, ns3::DefaultDeleter<ns3::TraceSourceAccessor> > [class]
    module.add_class('SimpleRefCount', automatic_type_narrowing=True, template_parameters=['ns3::TraceSourceAccessor', 'ns3::empty', 'ns3::DefaultDeleter<ns3::TraceSourceAccessor>'], parent=root_module['ns3::empty'], memory_policy=cppclass.ReferenceCountingMethodsPolicy(incref_method='Ref', decref_method='Unref', peekref_method='GetReferenceCount'))
    ## system-thread.h: ns3::SystemThread [class]
    module.add_class('SystemThread', parent=root_module['ns3::SimpleRefCount< ns3::SystemThread, ns3::empty, ns3::DefaultDeleter<ns3::SystemThread> >'])
    ## trace-source-accessor.h: ns3::TraceSourceAccessor [class]
    module.add_class('TraceSourceAccessor', parent=root_module['ns3::SimpleRefCount< ns3::TraceSourceAccessor, ns3::empty, ns3::DefaultDeleter<ns3::TraceSourceAccessor> >'])
    ## attribute.h: ns3::AttributeAccessor [class]
    module.add_class('AttributeAccessor', parent=root_module['ns3::SimpleRefCount< ns3::AttributeAccessor, ns3::empty, ns3::DefaultDeleter<ns3::AttributeAccessor> >'])
    ## attribute.h: ns3::AttributeChecker [class]
    module.add_class('AttributeChecker', allow_subclassing=False, automatic_type_narrowing=True, parent=root_module['ns3::SimpleRefCount< ns3::AttributeChecker, ns3::empty, ns3::DefaultDeleter<ns3::AttributeChecker> >'])
    ## attribute.h: ns3::AttributeValue [class]
    module.add_class('AttributeValue', allow_subclassing=False, automatic_type_narrowing=True, parent=root_module['ns3::SimpleRefCount< ns3::AttributeValue, ns3::empty, ns3::DefaultDeleter<ns3::AttributeValue> >'])
    ## boolean.h: ns3::BooleanChecker [class]
    module.add_class('BooleanChecker', parent=root_module['ns3::AttributeChecker'])
    ## boolean.h: ns3::BooleanValue [class]
    module.add_class('BooleanValue', parent=root_module['ns3::AttributeValue'])
    ## callback.h: ns3::CallbackChecker [class]
    module.add_class('CallbackChecker', parent=root_module['ns3::AttributeChecker'])
    ## callback.h: ns3::CallbackImplBase [class]
    module.add_class('CallbackImplBase', parent=root_module['ns3::SimpleRefCount< ns3::CallbackImplBase, ns3::empty, ns3::DefaultDeleter<ns3::CallbackImplBase> >'])
    ## callback.h: ns3::CallbackValue [class]
    module.add_class('CallbackValue', parent=root_module['ns3::AttributeValue'])
    ## double.h: ns3::DoubleValue [class]
    module.add_class('DoubleValue', parent=root_module['ns3::AttributeValue'])
    ## attribute.h: ns3::EmptyAttributeValue [class]
    module.add_class('EmptyAttributeValue', parent=root_module['ns3::AttributeValue'])
    ## enum.h: ns3::EnumChecker [class]
    module.add_class('EnumChecker', parent=root_module['ns3::AttributeChecker'])
    ## enum.h: ns3::EnumValue [class]
    module.add_class('EnumValue', parent=root_module['ns3::AttributeValue'])
    ## integer.h: ns3::IntegerValue [class]
    module.add_class('IntegerValue', parent=root_module['ns3::AttributeValue'])
    ## object-factory.h: ns3::ObjectFactoryChecker [class]
    module.add_class('ObjectFactoryChecker', parent=root_module['ns3::AttributeChecker'])
    ## object-factory.h: ns3::ObjectFactoryValue [class]
    module.add_class('ObjectFactoryValue', parent=root_module['ns3::AttributeValue'])
    ## object-vector.h: ns3::ObjectVectorAccessor [class]
    module.add_class('ObjectVectorAccessor', parent=root_module['ns3::AttributeAccessor'])
    ## object-vector.h: ns3::ObjectVectorChecker [class]
    module.add_class('ObjectVectorChecker', parent=root_module['ns3::AttributeChecker'])
    ## object-vector.h: ns3::ObjectVectorValue [class]
    module.add_class('ObjectVectorValue', parent=root_module['ns3::AttributeValue'])
    ## pointer.h: ns3::PointerChecker [class]
    module.add_class('PointerChecker', parent=root_module['ns3::AttributeChecker'])
    ## pointer.h: ns3::PointerValue [class]
    module.add_class('PointerValue', parent=root_module['ns3::AttributeValue'])
    ## random-variable.h: ns3::RandomVariableChecker [class]
    module.add_class('RandomVariableChecker', parent=root_module['ns3::AttributeChecker'])
    ## random-variable.h: ns3::RandomVariableValue [class]
    module.add_class('RandomVariableValue', parent=root_module['ns3::AttributeValue'])
    ## ref-count-base.h: ns3::RefCountBase [class]
    module.add_class('RefCountBase', parent=root_module['ns3::SimpleRefCount< ns3::RefCountBase, ns3::empty, ns3::DefaultDeleter<ns3::RefCountBase> >'])
    ## string.h: ns3::StringChecker [class]
    module.add_class('StringChecker', parent=root_module['ns3::AttributeChecker'])
    ## string.h: ns3::StringValue [class]
    module.add_class('StringValue', parent=root_module['ns3::AttributeValue'])
    ## type-id.h: ns3::TypeIdChecker [class]
    module.add_class('TypeIdChecker', parent=root_module['ns3::AttributeChecker'])
    ## type-id.h: ns3::TypeIdValue [class]
    module.add_class('TypeIdValue', parent=root_module['ns3::AttributeValue'])
    ## uinteger.h: ns3::UintegerValue [class]
    module.add_class('UintegerValue', parent=root_module['ns3::AttributeValue'])
    ## vector.h: ns3::Vector2DChecker [class]
    module.add_class('Vector2DChecker', parent=root_module['ns3::AttributeChecker'])
    ## vector.h: ns3::Vector2DValue [class]
    module.add_class('Vector2DValue', parent=root_module['ns3::AttributeValue'])
    ## vector.h: ns3::Vector3DChecker [class]
    module.add_class('Vector3DChecker', parent=root_module['ns3::AttributeChecker'])
    ## vector.h: ns3::Vector3DValue [class]
    module.add_class('Vector3DValue', parent=root_module['ns3::AttributeValue'])
    module.add_container('std::list< ns3::Ptr< ns3::SpectrumPhy > >', 'ns3::Ptr< ns3::SpectrumPhy >', container_type='list')
    module.add_container('std::list< ns3::Ptr< ns3::Packet > >', 'ns3::Ptr< ns3::Packet >', container_type='list')
    module.add_container('std::list< ns3::Ptr< ns3::UanPhy > >', 'ns3::Ptr< ns3::UanPhy >', container_type='list')
    module.add_container('std::list< ns3::Ptr< ns3::UlJob > >', 'ns3::Ptr< ns3::UlJob >', container_type='list')
    module.add_container('std::list< ns3::Ptr< ns3::Packet const > >', 'ns3::Ptr< ns3::Packet const >', container_type='list')
    module.add_container('std::vector< ns3::Ptr< ns3::WimaxConnection > >', 'ns3::Ptr< ns3::WimaxConnection >', container_type='vector')
    module.add_container('std::vector< ns3::Ptr< ns3::FlowProbe > >', 'ns3::Ptr< ns3::FlowProbe >', container_type='vector')
    module.add_container('std::list< ns3::Ptr< ns3::RadvdPrefix > >', 'ns3::Ptr< ns3::RadvdPrefix >', container_type='list')
    module.add_container('std::list< ns3::Ptr< ns3::UanTransducer > >', 'ns3::Ptr< ns3::UanTransducer >', container_type='list')
    module.add_container('std::vector< ns3::Ptr< ns3::NetDevice > >', 'ns3::Ptr< ns3::NetDevice >', container_type='vector')
    module.add_container('std::vector< ns3::Ptr< ns3::SpectrumPhy > >', 'ns3::Ptr< ns3::SpectrumPhy >', container_type='vector')
    typehandlers.add_type_alias('ns3::Vector3DChecker', 'ns3::VectorChecker')
    typehandlers.add_type_alias('ns3::Vector3DChecker*', 'ns3::VectorChecker*')
    typehandlers.add_type_alias('ns3::Vector3DChecker&', 'ns3::VectorChecker&')
    module.add_typedef(root_module['ns3::Vector3DChecker'], 'VectorChecker')
    typehandlers.add_type_alias('ns3::Vector3D', 'ns3::Vector')
    typehandlers.add_type_alias('ns3::Vector3D*', 'ns3::Vector*')
    typehandlers.add_type_alias('ns3::Vector3D&', 'ns3::Vector&')
    module.add_typedef(root_module['ns3::Vector3D'], 'Vector')
    typehandlers.add_type_alias('ns3::Vector3DValue', 'ns3::VectorValue')
    typehandlers.add_type_alias('ns3::Vector3DValue*', 'ns3::VectorValue*')
    typehandlers.add_type_alias('ns3::Vector3DValue&', 'ns3::VectorValue&')
    module.add_typedef(root_module['ns3::Vector3DValue'], 'VectorValue')
    
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
    
    ## config.h: ns3::Config::MatchContainer [class]
    module.add_class('MatchContainer')
    module.add_container('std::vector< ns3::Ptr< ns3::Object > >', 'ns3::Ptr< ns3::Object >', container_type='vector')

def register_types_ns3_FatalImpl(module):
    root_module = module.get_root()
    

def register_types_ns3_addressUtils(module):
    root_module = module.get_root()
    

def register_types_ns3_aodv(module):
    root_module = module.get_root()
    

def register_types_ns3_dot11s(module):
    root_module = module.get_root()
    
    module.add_container('std::vector< ns3::Ptr< ns3::dot11s::IeBeaconTimingUnit > >', 'ns3::Ptr< ns3::dot11s::IeBeaconTimingUnit >', container_type='vector')
    module.add_container('std::vector< ns3::Ptr< ns3::dot11s::PeerLink > >', 'ns3::Ptr< ns3::dot11s::PeerLink >', container_type='vector')

def register_types_ns3_flame(module):
    root_module = module.get_root()
    

def register_types_ns3_internal(module):
    root_module = module.get_root()
    

def register_types_ns3_olsr(module):
    root_module = module.get_root()
    

def register_methods(root_module):
    register_Ns3AttributeList_methods(root_module, root_module['ns3::AttributeList'])
    register_Ns3CallbackBase_methods(root_module, root_module['ns3::CallbackBase'])
    register_Ns3CommandLine_methods(root_module, root_module['ns3::CommandLine'])
    register_Ns3CriticalSection_methods(root_module, root_module['ns3::CriticalSection'])
    register_Ns3GlobalValue_methods(root_module, root_module['ns3::GlobalValue'])
    register_Ns3IntToType__0_methods(root_module, root_module['ns3::IntToType< 0 >'])
    register_Ns3IntToType__1_methods(root_module, root_module['ns3::IntToType< 1 >'])
    register_Ns3IntToType__2_methods(root_module, root_module['ns3::IntToType< 2 >'])
    register_Ns3IntToType__3_methods(root_module, root_module['ns3::IntToType< 3 >'])
    register_Ns3IntToType__4_methods(root_module, root_module['ns3::IntToType< 4 >'])
    register_Ns3IntToType__5_methods(root_module, root_module['ns3::IntToType< 5 >'])
    register_Ns3IntToType__6_methods(root_module, root_module['ns3::IntToType< 6 >'])
    register_Ns3Names_methods(root_module, root_module['ns3::Names'])
    register_Ns3ObjectBase_methods(root_module, root_module['ns3::ObjectBase'])
    register_Ns3ObjectDeleter_methods(root_module, root_module['ns3::ObjectDeleter'])
    register_Ns3ObjectFactory_methods(root_module, root_module['ns3::ObjectFactory'])
    register_Ns3RandomVariable_methods(root_module, root_module['ns3::RandomVariable'])
    register_Ns3RngStream_methods(root_module, root_module['ns3::RngStream'])
    register_Ns3SeedManager_methods(root_module, root_module['ns3::SeedManager'])
    register_Ns3SequentialVariable_methods(root_module, root_module['ns3::SequentialVariable'])
    register_Ns3SimpleRefCount__Ns3Object_Ns3ObjectBase_Ns3ObjectDeleter_methods(root_module, root_module['ns3::SimpleRefCount< ns3::Object, ns3::ObjectBase, ns3::ObjectDeleter >'])
    register_Ns3SystemCondition_methods(root_module, root_module['ns3::SystemCondition'])
    register_Ns3SystemMutex_methods(root_module, root_module['ns3::SystemMutex'])
    register_Ns3SystemWallClockMs_methods(root_module, root_module['ns3::SystemWallClockMs'])
    register_Ns3TestCase_methods(root_module, root_module['ns3::TestCase'])
    register_Ns3TestRunner_methods(root_module, root_module['ns3::TestRunner'])
    register_Ns3TestSuite_methods(root_module, root_module['ns3::TestSuite'])
    register_Ns3TracedValue__Double_methods(root_module, root_module['ns3::TracedValue< double >'])
    register_Ns3TriangularVariable_methods(root_module, root_module['ns3::TriangularVariable'])
    register_Ns3TypeId_methods(root_module, root_module['ns3::TypeId'])
    register_Ns3TypeIdAttributeInfo_methods(root_module, root_module['ns3::TypeId::AttributeInfo'])
    register_Ns3UniformVariable_methods(root_module, root_module['ns3::UniformVariable'])
    register_Ns3UnsafeAttributeList_methods(root_module, root_module['ns3::UnsafeAttributeList'])
    register_Ns3Vector2D_methods(root_module, root_module['ns3::Vector2D'])
    register_Ns3Vector3D_methods(root_module, root_module['ns3::Vector3D'])
    register_Ns3WeibullVariable_methods(root_module, root_module['ns3::WeibullVariable'])
    register_Ns3ZetaVariable_methods(root_module, root_module['ns3::ZetaVariable'])
    register_Ns3ZipfVariable_methods(root_module, root_module['ns3::ZipfVariable'])
    register_Ns3Empty_methods(root_module, root_module['ns3::empty'])
    register_Ns3ConstantVariable_methods(root_module, root_module['ns3::ConstantVariable'])
    register_Ns3DeterministicVariable_methods(root_module, root_module['ns3::DeterministicVariable'])
    register_Ns3EmpiricalVariable_methods(root_module, root_module['ns3::EmpiricalVariable'])
    register_Ns3ErlangVariable_methods(root_module, root_module['ns3::ErlangVariable'])
    register_Ns3ExponentialVariable_methods(root_module, root_module['ns3::ExponentialVariable'])
    register_Ns3GammaVariable_methods(root_module, root_module['ns3::GammaVariable'])
    register_Ns3IntEmpiricalVariable_methods(root_module, root_module['ns3::IntEmpiricalVariable'])
    register_Ns3LogNormalVariable_methods(root_module, root_module['ns3::LogNormalVariable'])
    register_Ns3NormalVariable_methods(root_module, root_module['ns3::NormalVariable'])
    register_Ns3Object_methods(root_module, root_module['ns3::Object'])
    register_Ns3ObjectAggregateIterator_methods(root_module, root_module['ns3::Object::AggregateIterator'])
    register_Ns3ParetoVariable_methods(root_module, root_module['ns3::ParetoVariable'])
    register_Ns3SimpleRefCount__Ns3AttributeAccessor_Ns3Empty_Ns3DefaultDeleter__lt__ns3AttributeAccessor__gt___methods(root_module, root_module['ns3::SimpleRefCount< ns3::AttributeAccessor, ns3::empty, ns3::DefaultDeleter<ns3::AttributeAccessor> >'])
    register_Ns3SimpleRefCount__Ns3AttributeChecker_Ns3Empty_Ns3DefaultDeleter__lt__ns3AttributeChecker__gt___methods(root_module, root_module['ns3::SimpleRefCount< ns3::AttributeChecker, ns3::empty, ns3::DefaultDeleter<ns3::AttributeChecker> >'])
    register_Ns3SimpleRefCount__Ns3AttributeValue_Ns3Empty_Ns3DefaultDeleter__lt__ns3AttributeValue__gt___methods(root_module, root_module['ns3::SimpleRefCount< ns3::AttributeValue, ns3::empty, ns3::DefaultDeleter<ns3::AttributeValue> >'])
    register_Ns3SimpleRefCount__Ns3CallbackImplBase_Ns3Empty_Ns3DefaultDeleter__lt__ns3CallbackImplBase__gt___methods(root_module, root_module['ns3::SimpleRefCount< ns3::CallbackImplBase, ns3::empty, ns3::DefaultDeleter<ns3::CallbackImplBase> >'])
    register_Ns3SimpleRefCount__Ns3EventImpl_Ns3Empty_Ns3DefaultDeleter__lt__ns3EventImpl__gt___methods(root_module, root_module['ns3::SimpleRefCount< ns3::EventImpl, ns3::empty, ns3::DefaultDeleter<ns3::EventImpl> >'])
    register_Ns3SimpleRefCount__Ns3FlowClassifier_Ns3Empty_Ns3DefaultDeleter__lt__ns3FlowClassifier__gt___methods(root_module, root_module['ns3::SimpleRefCount< ns3::FlowClassifier, ns3::empty, ns3::DefaultDeleter<ns3::FlowClassifier> >'])
    register_Ns3SimpleRefCount__Ns3FlowProbe_Ns3Empty_Ns3DefaultDeleter__lt__ns3FlowProbe__gt___methods(root_module, root_module['ns3::SimpleRefCount< ns3::FlowProbe, ns3::empty, ns3::DefaultDeleter<ns3::FlowProbe> >'])
    register_Ns3SimpleRefCount__Ns3InterferenceHelperEvent_Ns3Empty_Ns3DefaultDeleter__lt__ns3InterferenceHelperEvent__gt___methods(root_module, root_module['ns3::SimpleRefCount< ns3::InterferenceHelper::Event, ns3::empty, ns3::DefaultDeleter<ns3::InterferenceHelper::Event> >'])
    register_Ns3SimpleRefCount__Ns3Ipv4MulticastRoute_Ns3Empty_Ns3DefaultDeleter__lt__ns3Ipv4MulticastRoute__gt___methods(root_module, root_module['ns3::SimpleRefCount< ns3::Ipv4MulticastRoute, ns3::empty, ns3::DefaultDeleter<ns3::Ipv4MulticastRoute> >'])
    register_Ns3SimpleRefCount__Ns3Ipv4Route_Ns3Empty_Ns3DefaultDeleter__lt__ns3Ipv4Route__gt___methods(root_module, root_module['ns3::SimpleRefCount< ns3::Ipv4Route, ns3::empty, ns3::DefaultDeleter<ns3::Ipv4Route> >'])
    register_Ns3SimpleRefCount__Ns3Ipv6MulticastRoute_Ns3Empty_Ns3DefaultDeleter__lt__ns3Ipv6MulticastRoute__gt___methods(root_module, root_module['ns3::SimpleRefCount< ns3::Ipv6MulticastRoute, ns3::empty, ns3::DefaultDeleter<ns3::Ipv6MulticastRoute> >'])
    register_Ns3SimpleRefCount__Ns3Ipv6Route_Ns3Empty_Ns3DefaultDeleter__lt__ns3Ipv6Route__gt___methods(root_module, root_module['ns3::SimpleRefCount< ns3::Ipv6Route, ns3::empty, ns3::DefaultDeleter<ns3::Ipv6Route> >'])
    register_Ns3SimpleRefCount__Ns3MeshWifiInterfaceMacPlugin_Ns3Empty_Ns3DefaultDeleter__lt__ns3MeshWifiInterfaceMacPlugin__gt___methods(root_module, root_module['ns3::SimpleRefCount< ns3::MeshWifiInterfaceMacPlugin, ns3::empty, ns3::DefaultDeleter<ns3::MeshWifiInterfaceMacPlugin> >'])
    register_Ns3SimpleRefCount__Ns3NixVector_Ns3Empty_Ns3DefaultDeleter__lt__ns3NixVector__gt___methods(root_module, root_module['ns3::SimpleRefCount< ns3::NixVector, ns3::empty, ns3::DefaultDeleter<ns3::NixVector> >'])
    register_Ns3SimpleRefCount__Ns3OutputStreamWrapper_Ns3Empty_Ns3DefaultDeleter__lt__ns3OutputStreamWrapper__gt___methods(root_module, root_module['ns3::SimpleRefCount< ns3::OutputStreamWrapper, ns3::empty, ns3::DefaultDeleter<ns3::OutputStreamWrapper> >'])
    register_Ns3SimpleRefCount__Ns3Packet_Ns3Empty_Ns3DefaultDeleter__lt__ns3Packet__gt___methods(root_module, root_module['ns3::SimpleRefCount< ns3::Packet, ns3::empty, ns3::DefaultDeleter<ns3::Packet> >'])
    register_Ns3SimpleRefCount__Ns3PbbAddressBlock_Ns3Empty_Ns3DefaultDeleter__lt__ns3PbbAddressBlock__gt___methods(root_module, root_module['ns3::SimpleRefCount< ns3::PbbAddressBlock, ns3::empty, ns3::DefaultDeleter<ns3::PbbAddressBlock> >'])
    register_Ns3SimpleRefCount__Ns3PbbMessage_Ns3Empty_Ns3DefaultDeleter__lt__ns3PbbMessage__gt___methods(root_module, root_module['ns3::SimpleRefCount< ns3::PbbMessage, ns3::empty, ns3::DefaultDeleter<ns3::PbbMessage> >'])
    register_Ns3SimpleRefCount__Ns3PbbPacket_Ns3Header_Ns3DefaultDeleter__lt__ns3PbbPacket__gt___methods(root_module, root_module['ns3::SimpleRefCount< ns3::PbbPacket, ns3::Header, ns3::DefaultDeleter<ns3::PbbPacket> >'])
    register_Ns3SimpleRefCount__Ns3PbbTlv_Ns3Empty_Ns3DefaultDeleter__lt__ns3PbbTlv__gt___methods(root_module, root_module['ns3::SimpleRefCount< ns3::PbbTlv, ns3::empty, ns3::DefaultDeleter<ns3::PbbTlv> >'])
    register_Ns3SimpleRefCount__Ns3RadvdInterface_Ns3Empty_Ns3DefaultDeleter__lt__ns3RadvdInterface__gt___methods(root_module, root_module['ns3::SimpleRefCount< ns3::RadvdInterface, ns3::empty, ns3::DefaultDeleter<ns3::RadvdInterface> >'])
    register_Ns3SimpleRefCount__Ns3RadvdPrefix_Ns3Empty_Ns3DefaultDeleter__lt__ns3RadvdPrefix__gt___methods(root_module, root_module['ns3::SimpleRefCount< ns3::RadvdPrefix, ns3::empty, ns3::DefaultDeleter<ns3::RadvdPrefix> >'])
    register_Ns3SimpleRefCount__Ns3RefCountBase_Ns3Empty_Ns3DefaultDeleter__lt__ns3RefCountBase__gt___methods(root_module, root_module['ns3::SimpleRefCount< ns3::RefCountBase, ns3::empty, ns3::DefaultDeleter<ns3::RefCountBase> >'])
    register_Ns3SimpleRefCount__Ns3SpectrumConverter_Ns3Empty_Ns3DefaultDeleter__lt__ns3SpectrumConverter__gt___methods(root_module, root_module['ns3::SimpleRefCount< ns3::SpectrumConverter, ns3::empty, ns3::DefaultDeleter<ns3::SpectrumConverter> >'])
    register_Ns3SimpleRefCount__Ns3SpectrumModel_Ns3Empty_Ns3DefaultDeleter__lt__ns3SpectrumModel__gt___methods(root_module, root_module['ns3::SimpleRefCount< ns3::SpectrumModel, ns3::empty, ns3::DefaultDeleter<ns3::SpectrumModel> >'])
    register_Ns3SimpleRefCount__Ns3SpectrumValue_Ns3Empty_Ns3DefaultDeleter__lt__ns3SpectrumValue__gt___methods(root_module, root_module['ns3::SimpleRefCount< ns3::SpectrumValue, ns3::empty, ns3::DefaultDeleter<ns3::SpectrumValue> >'])
    register_Ns3SimpleRefCount__Ns3SystemThread_Ns3Empty_Ns3DefaultDeleter__lt__ns3SystemThread__gt___methods(root_module, root_module['ns3::SimpleRefCount< ns3::SystemThread, ns3::empty, ns3::DefaultDeleter<ns3::SystemThread> >'])
    register_Ns3SimpleRefCount__Ns3TraceSourceAccessor_Ns3Empty_Ns3DefaultDeleter__lt__ns3TraceSourceAccessor__gt___methods(root_module, root_module['ns3::SimpleRefCount< ns3::TraceSourceAccessor, ns3::empty, ns3::DefaultDeleter<ns3::TraceSourceAccessor> >'])
    register_Ns3SimpleRefCount__Ns3WifiInformationElement_Ns3Empty_Ns3DefaultDeleter__lt__ns3WifiInformationElement__gt___methods(root_module, root_module['ns3::SimpleRefCount< ns3::WifiInformationElement, ns3::empty, ns3::DefaultDeleter<ns3::WifiInformationElement> >'])
    register_Ns3SimpleRefCount__Ns3Dot11sIeBeaconTimingUnit_Ns3Empty_Ns3DefaultDeleter__lt__ns3Dot11sIeBeaconTimingUnit__gt___methods(root_module, root_module['ns3::SimpleRefCount< ns3::dot11s::IeBeaconTimingUnit, ns3::empty, ns3::DefaultDeleter<ns3::dot11s::IeBeaconTimingUnit> >'])
    register_Ns3SystemThread_methods(root_module, root_module['ns3::SystemThread'])
    register_Ns3TraceSourceAccessor_methods(root_module, root_module['ns3::TraceSourceAccessor'])
    register_Ns3AttributeAccessor_methods(root_module, root_module['ns3::AttributeAccessor'])
    register_Ns3AttributeChecker_methods(root_module, root_module['ns3::AttributeChecker'])
    register_Ns3AttributeValue_methods(root_module, root_module['ns3::AttributeValue'])
    register_Ns3BooleanChecker_methods(root_module, root_module['ns3::BooleanChecker'])
    register_Ns3BooleanValue_methods(root_module, root_module['ns3::BooleanValue'])
    register_Ns3CallbackChecker_methods(root_module, root_module['ns3::CallbackChecker'])
    register_Ns3CallbackImplBase_methods(root_module, root_module['ns3::CallbackImplBase'])
    register_Ns3CallbackValue_methods(root_module, root_module['ns3::CallbackValue'])
    register_Ns3DoubleValue_methods(root_module, root_module['ns3::DoubleValue'])
    register_Ns3EmptyAttributeValue_methods(root_module, root_module['ns3::EmptyAttributeValue'])
    register_Ns3EnumChecker_methods(root_module, root_module['ns3::EnumChecker'])
    register_Ns3EnumValue_methods(root_module, root_module['ns3::EnumValue'])
    register_Ns3IntegerValue_methods(root_module, root_module['ns3::IntegerValue'])
    register_Ns3ObjectFactoryChecker_methods(root_module, root_module['ns3::ObjectFactoryChecker'])
    register_Ns3ObjectFactoryValue_methods(root_module, root_module['ns3::ObjectFactoryValue'])
    register_Ns3ObjectVectorAccessor_methods(root_module, root_module['ns3::ObjectVectorAccessor'])
    register_Ns3ObjectVectorChecker_methods(root_module, root_module['ns3::ObjectVectorChecker'])
    register_Ns3ObjectVectorValue_methods(root_module, root_module['ns3::ObjectVectorValue'])
    register_Ns3PointerChecker_methods(root_module, root_module['ns3::PointerChecker'])
    register_Ns3PointerValue_methods(root_module, root_module['ns3::PointerValue'])
    register_Ns3RandomVariableChecker_methods(root_module, root_module['ns3::RandomVariableChecker'])
    register_Ns3RandomVariableValue_methods(root_module, root_module['ns3::RandomVariableValue'])
    register_Ns3RefCountBase_methods(root_module, root_module['ns3::RefCountBase'])
    register_Ns3StringChecker_methods(root_module, root_module['ns3::StringChecker'])
    register_Ns3StringValue_methods(root_module, root_module['ns3::StringValue'])
    register_Ns3TypeIdChecker_methods(root_module, root_module['ns3::TypeIdChecker'])
    register_Ns3TypeIdValue_methods(root_module, root_module['ns3::TypeIdValue'])
    register_Ns3UintegerValue_methods(root_module, root_module['ns3::UintegerValue'])
    register_Ns3Vector2DChecker_methods(root_module, root_module['ns3::Vector2DChecker'])
    register_Ns3Vector2DValue_methods(root_module, root_module['ns3::Vector2DValue'])
    register_Ns3Vector3DChecker_methods(root_module, root_module['ns3::Vector3DChecker'])
    register_Ns3Vector3DValue_methods(root_module, root_module['ns3::Vector3DValue'])
    register_Ns3ConfigMatchContainer_methods(root_module, root_module['ns3::Config::MatchContainer'])
    return

def register_Ns3AttributeList_methods(root_module, cls):
    ## attribute-list.h: ns3::AttributeList::AttributeList() [constructor]
    cls.add_constructor([])
    ## attribute-list.h: ns3::AttributeList::AttributeList(ns3::AttributeList const & o) [copy constructor]
    cls.add_constructor([param('ns3::AttributeList const &', 'o')])
    ## attribute-list.h: bool ns3::AttributeList::DeserializeFromString(std::string value) [member function]
    cls.add_method('DeserializeFromString', 
                   'bool', 
                   [param('std::string', 'value')])
    ## attribute-list.h: static ns3::AttributeList * ns3::AttributeList::GetGlobal() [member function]
    cls.add_method('GetGlobal', 
                   'ns3::AttributeList *', 
                   [], 
                   is_static=True)
    ## attribute-list.h: void ns3::AttributeList::Reset() [member function]
    cls.add_method('Reset', 
                   'void', 
                   [])
    ## attribute-list.h: std::string ns3::AttributeList::SerializeToString() const [member function]
    cls.add_method('SerializeToString', 
                   'std::string', 
                   [], 
                   is_const=True)
    ## attribute-list.h: void ns3::AttributeList::Set(std::string name, ns3::AttributeValue const & value) [member function]
    cls.add_method('Set', 
                   'void', 
                   [param('std::string', 'name'), param('ns3::AttributeValue const &', 'value')])
    ## attribute-list.h: bool ns3::AttributeList::SetFailSafe(std::string name, ns3::AttributeValue const & value) [member function]
    cls.add_method('SetFailSafe', 
                   'bool', 
                   [param('std::string', 'name'), param('ns3::AttributeValue const &', 'value')])
    ## attribute-list.h: void ns3::AttributeList::SetWithTid(ns3::TypeId tid, std::string name, ns3::AttributeValue const & value) [member function]
    cls.add_method('SetWithTid', 
                   'void', 
                   [param('ns3::TypeId', 'tid'), param('std::string', 'name'), param('ns3::AttributeValue const &', 'value')])
    return

def register_Ns3CallbackBase_methods(root_module, cls):
    ## callback.h: ns3::CallbackBase::CallbackBase(ns3::CallbackBase const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::CallbackBase const &', 'arg0')])
    ## callback.h: ns3::CallbackBase::CallbackBase() [constructor]
    cls.add_constructor([])
    ## callback.h: ns3::Ptr<ns3::CallbackImplBase> ns3::CallbackBase::GetImpl() const [member function]
    cls.add_method('GetImpl', 
                   'ns3::Ptr< ns3::CallbackImplBase >', 
                   [], 
                   is_const=True)
    ## callback.h: ns3::CallbackBase::CallbackBase(ns3::Ptr<ns3::CallbackImplBase> impl) [constructor]
    cls.add_constructor([param('ns3::Ptr< ns3::CallbackImplBase >', 'impl')], 
                        visibility='protected')
    ## callback.h: static std::string ns3::CallbackBase::Demangle(std::string const & mangled) [member function]
    cls.add_method('Demangle', 
                   'std::string', 
                   [param('std::string const &', 'mangled')], 
                   is_static=True, visibility='protected')
    return

def register_Ns3CommandLine_methods(root_module, cls):
    ## command-line.h: ns3::CommandLine::CommandLine() [constructor]
    cls.add_constructor([])
    ## command-line.h: ns3::CommandLine::CommandLine(ns3::CommandLine const & cmd) [copy constructor]
    cls.add_constructor([param('ns3::CommandLine const &', 'cmd')])
    ## command-line.h: void ns3::CommandLine::AddValue(std::string const & name, std::string const & help, ns3::Callback<bool, std::string, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty> callback) [member function]
    cls.add_method('AddValue', 
                   'void', 
                   [param('std::string const &', 'name'), param('std::string const &', 'help'), param('ns3::Callback< bool, std::string, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty >', 'callback')])
    return

def register_Ns3CriticalSection_methods(root_module, cls):
    ## system-mutex.h: ns3::CriticalSection::CriticalSection(ns3::CriticalSection const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::CriticalSection const &', 'arg0')])
    ## system-mutex.h: ns3::CriticalSection::CriticalSection(ns3::SystemMutex & mutex) [constructor]
    cls.add_constructor([param('ns3::SystemMutex &', 'mutex')])
    return

def register_Ns3GlobalValue_methods(root_module, cls):
    ## global-value.h: ns3::GlobalValue::GlobalValue(ns3::GlobalValue const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::GlobalValue const &', 'arg0')])
    ## global-value.h: ns3::GlobalValue::GlobalValue(std::string name, std::string help, ns3::AttributeValue const & initialValue, ns3::Ptr<ns3::AttributeChecker const> checker) [constructor]
    cls.add_constructor([param('std::string', 'name'), param('std::string', 'help'), param('ns3::AttributeValue const &', 'initialValue'), param('ns3::Ptr< ns3::AttributeChecker const >', 'checker')])
    ## global-value.h: static __gnu_cxx::__normal_iterator<ns3::GlobalValue* const*,std::vector<ns3::GlobalValue*, std::allocator<ns3::GlobalValue*> > > ns3::GlobalValue::Begin() [member function]
    cls.add_method('Begin', 
                   '__gnu_cxx::__normal_iterator< ns3::GlobalValue * const *, std::vector< ns3::GlobalValue * > >', 
                   [], 
                   is_static=True)
    ## global-value.h: static void ns3::GlobalValue::Bind(std::string name, ns3::AttributeValue const & value) [member function]
    cls.add_method('Bind', 
                   'void', 
                   [param('std::string', 'name'), param('ns3::AttributeValue const &', 'value')], 
                   is_static=True)
    ## global-value.h: static bool ns3::GlobalValue::BindFailSafe(std::string name, ns3::AttributeValue const & value) [member function]
    cls.add_method('BindFailSafe', 
                   'bool', 
                   [param('std::string', 'name'), param('ns3::AttributeValue const &', 'value')], 
                   is_static=True)
    ## global-value.h: static __gnu_cxx::__normal_iterator<ns3::GlobalValue* const*,std::vector<ns3::GlobalValue*, std::allocator<ns3::GlobalValue*> > > ns3::GlobalValue::End() [member function]
    cls.add_method('End', 
                   '__gnu_cxx::__normal_iterator< ns3::GlobalValue * const *, std::vector< ns3::GlobalValue * > >', 
                   [], 
                   is_static=True)
    ## global-value.h: ns3::Ptr<ns3::AttributeChecker const> ns3::GlobalValue::GetChecker() const [member function]
    cls.add_method('GetChecker', 
                   'ns3::Ptr< ns3::AttributeChecker const >', 
                   [], 
                   is_const=True)
    ## global-value.h: std::string ns3::GlobalValue::GetHelp() const [member function]
    cls.add_method('GetHelp', 
                   'std::string', 
                   [], 
                   is_const=True)
    ## global-value.h: std::string ns3::GlobalValue::GetName() const [member function]
    cls.add_method('GetName', 
                   'std::string', 
                   [], 
                   is_const=True)
    ## global-value.h: void ns3::GlobalValue::GetValue(ns3::AttributeValue & value) const [member function]
    cls.add_method('GetValue', 
                   'void', 
                   [param('ns3::AttributeValue &', 'value')], 
                   is_const=True)
    ## global-value.h: static void ns3::GlobalValue::GetValueByName(std::string name, ns3::AttributeValue & value) [member function]
    cls.add_method('GetValueByName', 
                   'void', 
                   [param('std::string', 'name'), param('ns3::AttributeValue &', 'value')], 
                   is_static=True)
    ## global-value.h: static bool ns3::GlobalValue::GetValueByNameFailSafe(std::string name, ns3::AttributeValue & value) [member function]
    cls.add_method('GetValueByNameFailSafe', 
                   'bool', 
                   [param('std::string', 'name'), param('ns3::AttributeValue &', 'value')], 
                   is_static=True)
    ## global-value.h: bool ns3::GlobalValue::SetValue(ns3::AttributeValue const & value) [member function]
    cls.add_method('SetValue', 
                   'bool', 
                   [param('ns3::AttributeValue const &', 'value')])
    return

def register_Ns3IntToType__0_methods(root_module, cls):
    ## int-to-type.h: ns3::IntToType<0>::IntToType() [constructor]
    cls.add_constructor([])
    ## int-to-type.h: ns3::IntToType<0>::IntToType(ns3::IntToType<0> const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::IntToType< 0 > const &', 'arg0')])
    return

def register_Ns3IntToType__1_methods(root_module, cls):
    ## int-to-type.h: ns3::IntToType<1>::IntToType() [constructor]
    cls.add_constructor([])
    ## int-to-type.h: ns3::IntToType<1>::IntToType(ns3::IntToType<1> const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::IntToType< 1 > const &', 'arg0')])
    return

def register_Ns3IntToType__2_methods(root_module, cls):
    ## int-to-type.h: ns3::IntToType<2>::IntToType() [constructor]
    cls.add_constructor([])
    ## int-to-type.h: ns3::IntToType<2>::IntToType(ns3::IntToType<2> const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::IntToType< 2 > const &', 'arg0')])
    return

def register_Ns3IntToType__3_methods(root_module, cls):
    ## int-to-type.h: ns3::IntToType<3>::IntToType() [constructor]
    cls.add_constructor([])
    ## int-to-type.h: ns3::IntToType<3>::IntToType(ns3::IntToType<3> const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::IntToType< 3 > const &', 'arg0')])
    return

def register_Ns3IntToType__4_methods(root_module, cls):
    ## int-to-type.h: ns3::IntToType<4>::IntToType() [constructor]
    cls.add_constructor([])
    ## int-to-type.h: ns3::IntToType<4>::IntToType(ns3::IntToType<4> const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::IntToType< 4 > const &', 'arg0')])
    return

def register_Ns3IntToType__5_methods(root_module, cls):
    ## int-to-type.h: ns3::IntToType<5>::IntToType() [constructor]
    cls.add_constructor([])
    ## int-to-type.h: ns3::IntToType<5>::IntToType(ns3::IntToType<5> const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::IntToType< 5 > const &', 'arg0')])
    return

def register_Ns3IntToType__6_methods(root_module, cls):
    ## int-to-type.h: ns3::IntToType<6>::IntToType() [constructor]
    cls.add_constructor([])
    ## int-to-type.h: ns3::IntToType<6>::IntToType(ns3::IntToType<6> const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::IntToType< 6 > const &', 'arg0')])
    return

def register_Ns3Names_methods(root_module, cls):
    ## names.h: ns3::Names::Names() [constructor]
    cls.add_constructor([])
    ## names.h: ns3::Names::Names(ns3::Names const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::Names const &', 'arg0')])
    ## names.h: static void ns3::Names::Add(std::string name, ns3::Ptr<ns3::Object> object) [member function]
    cls.add_method('Add', 
                   'void', 
                   [param('std::string', 'name'), param('ns3::Ptr< ns3::Object >', 'object')], 
                   is_static=True)
    ## names.h: static void ns3::Names::Add(std::string path, std::string name, ns3::Ptr<ns3::Object> object) [member function]
    cls.add_method('Add', 
                   'void', 
                   [param('std::string', 'path'), param('std::string', 'name'), param('ns3::Ptr< ns3::Object >', 'object')], 
                   is_static=True)
    ## names.h: static void ns3::Names::Add(ns3::Ptr<ns3::Object> context, std::string name, ns3::Ptr<ns3::Object> object) [member function]
    cls.add_method('Add', 
                   'void', 
                   [param('ns3::Ptr< ns3::Object >', 'context'), param('std::string', 'name'), param('ns3::Ptr< ns3::Object >', 'object')], 
                   is_static=True)
    ## names.h: static void ns3::Names::Clear() [member function]
    cls.add_method('Clear', 
                   'void', 
                   [], 
                   is_static=True)
    ## names.h: static std::string ns3::Names::FindName(ns3::Ptr<ns3::Object> object) [member function]
    cls.add_method('FindName', 
                   'std::string', 
                   [param('ns3::Ptr< ns3::Object >', 'object')], 
                   is_static=True)
    ## names.h: static std::string ns3::Names::FindPath(ns3::Ptr<ns3::Object> object) [member function]
    cls.add_method('FindPath', 
                   'std::string', 
                   [param('ns3::Ptr< ns3::Object >', 'object')], 
                   is_static=True)
    ## names.h: static void ns3::Names::Rename(std::string oldpath, std::string newname) [member function]
    cls.add_method('Rename', 
                   'void', 
                   [param('std::string', 'oldpath'), param('std::string', 'newname')], 
                   is_static=True)
    ## names.h: static void ns3::Names::Rename(std::string path, std::string oldname, std::string newname) [member function]
    cls.add_method('Rename', 
                   'void', 
                   [param('std::string', 'path'), param('std::string', 'oldname'), param('std::string', 'newname')], 
                   is_static=True)
    ## names.h: static void ns3::Names::Rename(ns3::Ptr<ns3::Object> context, std::string oldname, std::string newname) [member function]
    cls.add_method('Rename', 
                   'void', 
                   [param('ns3::Ptr< ns3::Object >', 'context'), param('std::string', 'oldname'), param('std::string', 'newname')], 
                   is_static=True)
    return

def register_Ns3ObjectBase_methods(root_module, cls):
    ## object-base.h: ns3::ObjectBase::ObjectBase() [constructor]
    cls.add_constructor([])
    ## object-base.h: ns3::ObjectBase::ObjectBase(ns3::ObjectBase const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::ObjectBase const &', 'arg0')])
    ## object-base.h: void ns3::ObjectBase::GetAttribute(std::string name, ns3::AttributeValue & value) const [member function]
    cls.add_method('GetAttribute', 
                   'void', 
                   [param('std::string', 'name'), param('ns3::AttributeValue &', 'value')], 
                   is_const=True)
    ## object-base.h: bool ns3::ObjectBase::GetAttributeFailSafe(std::string name, ns3::AttributeValue & attribute) const [member function]
    cls.add_method('GetAttributeFailSafe', 
                   'bool', 
                   [param('std::string', 'name'), param('ns3::AttributeValue &', 'attribute')], 
                   is_const=True)
    ## object-base.h: ns3::TypeId ns3::ObjectBase::GetInstanceTypeId() const [member function]
    cls.add_method('GetInstanceTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_pure_virtual=True, is_const=True, is_virtual=True)
    ## object-base.h: static ns3::TypeId ns3::ObjectBase::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## object-base.h: void ns3::ObjectBase::SetAttribute(std::string name, ns3::AttributeValue const & value) [member function]
    cls.add_method('SetAttribute', 
                   'void', 
                   [param('std::string', 'name'), param('ns3::AttributeValue const &', 'value')])
    ## object-base.h: bool ns3::ObjectBase::SetAttributeFailSafe(std::string name, ns3::AttributeValue const & value) [member function]
    cls.add_method('SetAttributeFailSafe', 
                   'bool', 
                   [param('std::string', 'name'), param('ns3::AttributeValue const &', 'value')])
    ## object-base.h: bool ns3::ObjectBase::TraceConnect(std::string name, std::string context, ns3::CallbackBase const & cb) [member function]
    cls.add_method('TraceConnect', 
                   'bool', 
                   [param('std::string', 'name'), param('std::string', 'context'), param('ns3::CallbackBase const &', 'cb')])
    ## object-base.h: bool ns3::ObjectBase::TraceConnectWithoutContext(std::string name, ns3::CallbackBase const & cb) [member function]
    cls.add_method('TraceConnectWithoutContext', 
                   'bool', 
                   [param('std::string', 'name'), param('ns3::CallbackBase const &', 'cb')])
    ## object-base.h: bool ns3::ObjectBase::TraceDisconnect(std::string name, std::string context, ns3::CallbackBase const & cb) [member function]
    cls.add_method('TraceDisconnect', 
                   'bool', 
                   [param('std::string', 'name'), param('std::string', 'context'), param('ns3::CallbackBase const &', 'cb')])
    ## object-base.h: bool ns3::ObjectBase::TraceDisconnectWithoutContext(std::string name, ns3::CallbackBase const & cb) [member function]
    cls.add_method('TraceDisconnectWithoutContext', 
                   'bool', 
                   [param('std::string', 'name'), param('ns3::CallbackBase const &', 'cb')])
    ## object-base.h: void ns3::ObjectBase::ConstructSelf(ns3::AttributeList const & attributes) [member function]
    cls.add_method('ConstructSelf', 
                   'void', 
                   [param('ns3::AttributeList const &', 'attributes')], 
                   visibility='protected')
    ## object-base.h: void ns3::ObjectBase::NotifyConstructionCompleted() [member function]
    cls.add_method('NotifyConstructionCompleted', 
                   'void', 
                   [], 
                   visibility='protected', is_virtual=True)
    return

def register_Ns3ObjectDeleter_methods(root_module, cls):
    ## object.h: ns3::ObjectDeleter::ObjectDeleter() [constructor]
    cls.add_constructor([])
    ## object.h: ns3::ObjectDeleter::ObjectDeleter(ns3::ObjectDeleter const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::ObjectDeleter const &', 'arg0')])
    ## object.h: static void ns3::ObjectDeleter::Delete(ns3::Object * object) [member function]
    cls.add_method('Delete', 
                   'void', 
                   [param('ns3::Object *', 'object')], 
                   is_static=True)
    return

def register_Ns3ObjectFactory_methods(root_module, cls):
    cls.add_output_stream_operator()
    ## object-factory.h: ns3::ObjectFactory::ObjectFactory(ns3::ObjectFactory const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::ObjectFactory const &', 'arg0')])
    ## object-factory.h: ns3::ObjectFactory::ObjectFactory() [constructor]
    cls.add_constructor([])
    ## object-factory.h: ns3::Ptr<ns3::Object> ns3::ObjectFactory::Create() const [member function]
    cls.add_method('Create', 
                   'ns3::Ptr< ns3::Object >', 
                   [], 
                   is_const=True)
    ## object-factory.h: ns3::TypeId ns3::ObjectFactory::GetTypeId() const [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_const=True)
    ## object-factory.h: void ns3::ObjectFactory::Set(std::string name, ns3::AttributeValue const & value) [member function]
    cls.add_method('Set', 
                   'void', 
                   [param('std::string', 'name'), param('ns3::AttributeValue const &', 'value')])
    ## object-factory.h: void ns3::ObjectFactory::Set(ns3::AttributeList const & list) [member function]
    cls.add_method('Set', 
                   'void', 
                   [param('ns3::AttributeList const &', 'list')])
    ## object-factory.h: void ns3::ObjectFactory::SetTypeId(ns3::TypeId tid) [member function]
    cls.add_method('SetTypeId', 
                   'void', 
                   [param('ns3::TypeId', 'tid')])
    ## object-factory.h: void ns3::ObjectFactory::SetTypeId(char const * tid) [member function]
    cls.add_method('SetTypeId', 
                   'void', 
                   [param('char const *', 'tid')])
    ## object-factory.h: void ns3::ObjectFactory::SetTypeId(std::string tid) [member function]
    cls.add_method('SetTypeId', 
                   'void', 
                   [param('std::string', 'tid')])
    return

def register_Ns3RandomVariable_methods(root_module, cls):
    cls.add_output_stream_operator()
    ## random-variable.h: ns3::RandomVariable::RandomVariable() [constructor]
    cls.add_constructor([])
    ## random-variable.h: ns3::RandomVariable::RandomVariable(ns3::RandomVariable const & o) [copy constructor]
    cls.add_constructor([param('ns3::RandomVariable const &', 'o')])
    ## random-variable.h: uint32_t ns3::RandomVariable::GetInteger() const [member function]
    cls.add_method('GetInteger', 
                   'uint32_t', 
                   [], 
                   is_const=True)
    ## random-variable.h: double ns3::RandomVariable::GetValue() const [member function]
    cls.add_method('GetValue', 
                   'double', 
                   [], 
                   is_const=True)
    return

def register_Ns3RngStream_methods(root_module, cls):
    ## rng-stream.h: ns3::RngStream::RngStream() [constructor]
    cls.add_constructor([])
    ## rng-stream.h: ns3::RngStream::RngStream(ns3::RngStream const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::RngStream const &', 'arg0')])
    ## rng-stream.h: void ns3::RngStream::AdvanceState(int32_t e, int32_t c) [member function]
    cls.add_method('AdvanceState', 
                   'void', 
                   [param('int32_t', 'e'), param('int32_t', 'c')])
    ## rng-stream.h: static bool ns3::RngStream::CheckSeed(uint32_t const * seed) [member function]
    cls.add_method('CheckSeed', 
                   'bool', 
                   [param('uint32_t const *', 'seed')], 
                   is_static=True)
    ## rng-stream.h: static bool ns3::RngStream::CheckSeed(uint32_t seed) [member function]
    cls.add_method('CheckSeed', 
                   'bool', 
                   [param('uint32_t', 'seed')], 
                   is_static=True)
    ## rng-stream.h: static uint32_t ns3::RngStream::GetPackageRun() [member function]
    cls.add_method('GetPackageRun', 
                   'uint32_t', 
                   [], 
                   is_static=True)
    ## rng-stream.h: static void ns3::RngStream::GetPackageSeed(uint32_t * seed) [member function]
    cls.add_method('GetPackageSeed', 
                   'void', 
                   [param('uint32_t *', 'seed')], 
                   is_static=True)
    ## rng-stream.h: void ns3::RngStream::GetState(uint32_t * seed) const [member function]
    cls.add_method('GetState', 
                   'void', 
                   [param('uint32_t *', 'seed')], 
                   is_const=True)
    ## rng-stream.h: void ns3::RngStream::IncreasedPrecis(bool incp) [member function]
    cls.add_method('IncreasedPrecis', 
                   'void', 
                   [param('bool', 'incp')])
    ## rng-stream.h: void ns3::RngStream::InitializeStream() [member function]
    cls.add_method('InitializeStream', 
                   'void', 
                   [])
    ## rng-stream.h: int32_t ns3::RngStream::RandInt(int32_t i, int32_t j) [member function]
    cls.add_method('RandInt', 
                   'int32_t', 
                   [param('int32_t', 'i'), param('int32_t', 'j')])
    ## rng-stream.h: double ns3::RngStream::RandU01() [member function]
    cls.add_method('RandU01', 
                   'double', 
                   [])
    ## rng-stream.h: void ns3::RngStream::ResetNextSubstream() [member function]
    cls.add_method('ResetNextSubstream', 
                   'void', 
                   [])
    ## rng-stream.h: void ns3::RngStream::ResetNthSubstream(uint32_t N) [member function]
    cls.add_method('ResetNthSubstream', 
                   'void', 
                   [param('uint32_t', 'N')])
    ## rng-stream.h: void ns3::RngStream::ResetStartStream() [member function]
    cls.add_method('ResetStartStream', 
                   'void', 
                   [])
    ## rng-stream.h: void ns3::RngStream::ResetStartSubstream() [member function]
    cls.add_method('ResetStartSubstream', 
                   'void', 
                   [])
    ## rng-stream.h: void ns3::RngStream::SetAntithetic(bool a) [member function]
    cls.add_method('SetAntithetic', 
                   'void', 
                   [param('bool', 'a')])
    ## rng-stream.h: static void ns3::RngStream::SetPackageRun(uint32_t run) [member function]
    cls.add_method('SetPackageRun', 
                   'void', 
                   [param('uint32_t', 'run')], 
                   is_static=True)
    ## rng-stream.h: static bool ns3::RngStream::SetPackageSeed(uint32_t seed) [member function]
    cls.add_method('SetPackageSeed', 
                   'bool', 
                   [param('uint32_t', 'seed')], 
                   is_static=True)
    ## rng-stream.h: static bool ns3::RngStream::SetPackageSeed(uint32_t const * seed) [member function]
    cls.add_method('SetPackageSeed', 
                   'bool', 
                   [param('uint32_t const *', 'seed')], 
                   is_static=True)
    ## rng-stream.h: bool ns3::RngStream::SetSeeds(uint32_t const * seed) [member function]
    cls.add_method('SetSeeds', 
                   'bool', 
                   [param('uint32_t const *', 'seed')])
    return

def register_Ns3SeedManager_methods(root_module, cls):
    ## random-variable.h: ns3::SeedManager::SeedManager() [constructor]
    cls.add_constructor([])
    ## random-variable.h: ns3::SeedManager::SeedManager(ns3::SeedManager const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::SeedManager const &', 'arg0')])
    ## random-variable.h: static bool ns3::SeedManager::CheckSeed(uint32_t seed) [member function]
    cls.add_method('CheckSeed', 
                   'bool', 
                   [param('uint32_t', 'seed')], 
                   is_static=True)
    ## random-variable.h: static uint32_t ns3::SeedManager::GetRun() [member function]
    cls.add_method('GetRun', 
                   'uint32_t', 
                   [], 
                   is_static=True)
    ## random-variable.h: static uint32_t ns3::SeedManager::GetSeed() [member function]
    cls.add_method('GetSeed', 
                   'uint32_t', 
                   [], 
                   is_static=True)
    ## random-variable.h: static void ns3::SeedManager::SetRun(uint32_t run) [member function]
    cls.add_method('SetRun', 
                   'void', 
                   [param('uint32_t', 'run')], 
                   is_static=True)
    ## random-variable.h: static void ns3::SeedManager::SetSeed(uint32_t seed) [member function]
    cls.add_method('SetSeed', 
                   'void', 
                   [param('uint32_t', 'seed')], 
                   is_static=True)
    return

def register_Ns3SequentialVariable_methods(root_module, cls):
    ## random-variable.h: ns3::SequentialVariable::SequentialVariable(ns3::SequentialVariable const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::SequentialVariable const &', 'arg0')])
    ## random-variable.h: ns3::SequentialVariable::SequentialVariable(double f, double l, double i=1, uint32_t c=1) [constructor]
    cls.add_constructor([param('double', 'f'), param('double', 'l'), param('double', 'i', default_value='1'), param('uint32_t', 'c', default_value='1')])
    ## random-variable.h: ns3::SequentialVariable::SequentialVariable(double f, double l, ns3::RandomVariable const & i, uint32_t c=1) [constructor]
    cls.add_constructor([param('double', 'f'), param('double', 'l'), param('ns3::RandomVariable const &', 'i'), param('uint32_t', 'c', default_value='1')])
    return

def register_Ns3SimpleRefCount__Ns3Object_Ns3ObjectBase_Ns3ObjectDeleter_methods(root_module, cls):
    ## simple-ref-count.h: ns3::SimpleRefCount<ns3::Object, ns3::ObjectBase, ns3::ObjectDeleter>::SimpleRefCount() [constructor]
    cls.add_constructor([])
    ## simple-ref-count.h: ns3::SimpleRefCount<ns3::Object, ns3::ObjectBase, ns3::ObjectDeleter>::SimpleRefCount(ns3::SimpleRefCount<ns3::Object, ns3::ObjectBase, ns3::ObjectDeleter> const & o) [copy constructor]
    cls.add_constructor([param('ns3::SimpleRefCount< ns3::Object, ns3::ObjectBase, ns3::ObjectDeleter > const &', 'o')])
    ## simple-ref-count.h: static void ns3::SimpleRefCount<ns3::Object, ns3::ObjectBase, ns3::ObjectDeleter>::Cleanup() [member function]
    cls.add_method('Cleanup', 
                   'void', 
                   [], 
                   is_static=True)
    return

def register_Ns3SystemCondition_methods(root_module, cls):
    ## system-condition.h: ns3::SystemCondition::SystemCondition(ns3::SystemCondition const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::SystemCondition const &', 'arg0')])
    ## system-condition.h: ns3::SystemCondition::SystemCondition() [constructor]
    cls.add_constructor([])
    ## system-condition.h: void ns3::SystemCondition::Broadcast() [member function]
    cls.add_method('Broadcast', 
                   'void', 
                   [])
    ## system-condition.h: bool ns3::SystemCondition::GetCondition() [member function]
    cls.add_method('GetCondition', 
                   'bool', 
                   [])
    ## system-condition.h: void ns3::SystemCondition::SetCondition(bool condition) [member function]
    cls.add_method('SetCondition', 
                   'void', 
                   [param('bool', 'condition')])
    ## system-condition.h: void ns3::SystemCondition::Signal() [member function]
    cls.add_method('Signal', 
                   'void', 
                   [])
    ## system-condition.h: bool ns3::SystemCondition::TimedWait(uint64_t ns) [member function]
    cls.add_method('TimedWait', 
                   'bool', 
                   [param('uint64_t', 'ns')])
    ## system-condition.h: void ns3::SystemCondition::Wait() [member function]
    cls.add_method('Wait', 
                   'void', 
                   [])
    return

def register_Ns3SystemMutex_methods(root_module, cls):
    ## system-mutex.h: ns3::SystemMutex::SystemMutex(ns3::SystemMutex const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::SystemMutex const &', 'arg0')])
    ## system-mutex.h: ns3::SystemMutex::SystemMutex() [constructor]
    cls.add_constructor([])
    ## system-mutex.h: void ns3::SystemMutex::Lock() [member function]
    cls.add_method('Lock', 
                   'void', 
                   [])
    ## system-mutex.h: void ns3::SystemMutex::Unlock() [member function]
    cls.add_method('Unlock', 
                   'void', 
                   [])
    return

def register_Ns3SystemWallClockMs_methods(root_module, cls):
    ## system-wall-clock-ms.h: ns3::SystemWallClockMs::SystemWallClockMs(ns3::SystemWallClockMs const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::SystemWallClockMs const &', 'arg0')])
    ## system-wall-clock-ms.h: ns3::SystemWallClockMs::SystemWallClockMs() [constructor]
    cls.add_constructor([])
    ## system-wall-clock-ms.h: int64_t ns3::SystemWallClockMs::End() [member function]
    cls.add_method('End', 
                   'int64_t', 
                   [])
    ## system-wall-clock-ms.h: int64_t ns3::SystemWallClockMs::GetElapsedReal() const [member function]
    cls.add_method('GetElapsedReal', 
                   'int64_t', 
                   [], 
                   is_const=True)
    ## system-wall-clock-ms.h: int64_t ns3::SystemWallClockMs::GetElapsedSystem() const [member function]
    cls.add_method('GetElapsedSystem', 
                   'int64_t', 
                   [], 
                   is_const=True)
    ## system-wall-clock-ms.h: int64_t ns3::SystemWallClockMs::GetElapsedUser() const [member function]
    cls.add_method('GetElapsedUser', 
                   'int64_t', 
                   [], 
                   is_const=True)
    ## system-wall-clock-ms.h: void ns3::SystemWallClockMs::Start() [member function]
    cls.add_method('Start', 
                   'void', 
                   [])
    return

def register_Ns3TestCase_methods(root_module, cls):
    ## test.h: ns3::TestCase::TestCase(std::string name) [constructor]
    cls.add_constructor([param('std::string', 'name')])
    ## test.h: bool ns3::TestCase::Run() [member function]
    cls.add_method('Run', 
                   'bool', 
                   [])
    ## test.h: void ns3::TestCase::SetVerbose(bool verbose) [member function]
    cls.add_method('SetVerbose', 
                   'void', 
                   [param('bool', 'verbose')])
    ## test.h: void ns3::TestCase::SetContinueOnFailure(bool continueOnFailure) [member function]
    cls.add_method('SetContinueOnFailure', 
                   'void', 
                   [param('bool', 'continueOnFailure')])
    ## test.h: void ns3::TestCase::SetName(std::string name) [member function]
    cls.add_method('SetName', 
                   'void', 
                   [param('std::string', 'name')])
    ## test.h: std::string ns3::TestCase::GetName() [member function]
    cls.add_method('GetName', 
                   'std::string', 
                   [])
    ## test.h: void ns3::TestCase::SetBaseDir(std::string dir) [member function]
    cls.add_method('SetBaseDir', 
                   'void', 
                   [param('std::string', 'dir')])
    ## test.h: std::string ns3::TestCase::GetBaseDir() [member function]
    cls.add_method('GetBaseDir', 
                   'std::string', 
                   [])
    ## test.h: void ns3::TestCase::SetTempDir(std::string dir) [member function]
    cls.add_method('SetTempDir', 
                   'void', 
                   [param('std::string', 'dir')])
    ## test.h: std::string ns3::TestCase::GetTempDir() [member function]
    cls.add_method('GetTempDir', 
                   'std::string', 
                   [])
    ## test.h: std::string ns3::TestCase::GetSourceDir(std::string file) [member function]
    cls.add_method('GetSourceDir', 
                   'std::string', 
                   [param('std::string', 'file')])
    ## test.h: void ns3::TestCase::SetStream(std::ofstream * ofs) [member function]
    cls.add_method('SetStream', 
                   'void', 
                   [param('std::ofstream *', 'ofs')])
    ## test.h: std::ofstream * ns3::TestCase::GetStream() [member function]
    cls.add_method('GetStream', 
                   'std::ofstream *', 
                   [])
    ## test.h: void ns3::TestCase::UpdateErrorStatus(bool error) [member function]
    cls.add_method('UpdateErrorStatus', 
                   'void', 
                   [param('bool', 'error')])
    ## test.h: void ns3::TestCase::SetErrorStatus(bool error) [member function]
    cls.add_method('SetErrorStatus', 
                   'void', 
                   [param('bool', 'error')])
    ## test.h: bool ns3::TestCase::GetErrorStatus() [member function]
    cls.add_method('GetErrorStatus', 
                   'bool', 
                   [])
    ## test.h: bool ns3::TestCase::ContinueOnFailure() [member function]
    cls.add_method('ContinueOnFailure', 
                   'bool', 
                   [])
    ## test.h: void ns3::TestCase::ReportStart() [member function]
    cls.add_method('ReportStart', 
                   'void', 
                   [])
    ## test.h: void ns3::TestCase::ReportCaseSuccess() [member function]
    cls.add_method('ReportCaseSuccess', 
                   'void', 
                   [])
    ## test.h: void ns3::TestCase::ReportCaseFailure() [member function]
    cls.add_method('ReportCaseFailure', 
                   'void', 
                   [])
    ## test.h: void ns3::TestCase::ReportTestFailure(std::string cond, std::string actual, std::string limit, std::string message, std::string file, int32_t line) [member function]
    cls.add_method('ReportTestFailure', 
                   'void', 
                   [param('std::string', 'cond'), param('std::string', 'actual'), param('std::string', 'limit'), param('std::string', 'message'), param('std::string', 'file'), param('int32_t', 'line')])
    ## test.h: void ns3::TestCase::ReportEnd() [member function]
    cls.add_method('ReportEnd', 
                   'void', 
                   [])
    ## test.h: void ns3::TestCase::DoReportStart() [member function]
    cls.add_method('DoReportStart', 
                   'void', 
                   [], 
                   visibility='protected', is_virtual=True)
    ## test.h: void ns3::TestCase::DoReportCaseSuccess() [member function]
    cls.add_method('DoReportCaseSuccess', 
                   'void', 
                   [], 
                   visibility='protected', is_virtual=True)
    ## test.h: void ns3::TestCase::DoReportCaseFailure() [member function]
    cls.add_method('DoReportCaseFailure', 
                   'void', 
                   [], 
                   visibility='protected', is_virtual=True)
    ## test.h: void ns3::TestCase::DoReportTestFailure(std::string cond, std::string actual, std::string limit, std::string message, std::string file, int32_t line) [member function]
    cls.add_method('DoReportTestFailure', 
                   'void', 
                   [param('std::string', 'cond'), param('std::string', 'actual'), param('std::string', 'limit'), param('std::string', 'message'), param('std::string', 'file'), param('int32_t', 'line')], 
                   visibility='protected', is_virtual=True)
    ## test.h: void ns3::TestCase::DoReportEnd() [member function]
    cls.add_method('DoReportEnd', 
                   'void', 
                   [], 
                   visibility='protected', is_virtual=True)
    ## test.h: void ns3::TestCase::DoSetup() [member function]
    cls.add_method('DoSetup', 
                   'void', 
                   [], 
                   visibility='protected', is_virtual=True)
    ## test.h: bool ns3::TestCase::DoRun() [member function]
    cls.add_method('DoRun', 
                   'bool', 
                   [], 
                   is_pure_virtual=True, visibility='protected', is_virtual=True)
    ## test.h: void ns3::TestCase::DoTeardown() [member function]
    cls.add_method('DoTeardown', 
                   'void', 
                   [], 
                   visibility='protected', is_virtual=True)
    return

def register_Ns3TestRunner_methods(root_module, cls):
    ## test.h: ns3::TestRunner::TestRunner() [constructor]
    cls.add_constructor([])
    ## test.h: ns3::TestRunner::TestRunner(ns3::TestRunner const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::TestRunner const &', 'arg0')])
    ## test.h: static uint32_t ns3::TestRunner::AddTestSuite(ns3::TestSuite * testSuite) [member function]
    cls.add_method('AddTestSuite', 
                   'uint32_t', 
                   [param('ns3::TestSuite *', 'testSuite')], 
                   is_static=True)
    ## test.h: static uint32_t ns3::TestRunner::GetNTestSuites() [member function]
    cls.add_method('GetNTestSuites', 
                   'uint32_t', 
                   [], 
                   is_static=True)
    ## test.h: static ns3::TestSuite * ns3::TestRunner::GetTestSuite(uint32_t n) [member function]
    cls.add_method('GetTestSuite', 
                   'ns3::TestSuite *', 
                   [param('uint32_t', 'n')], 
                   is_static=True)
    return

def register_Ns3TestSuite_methods(root_module, cls):
    ## test.h: ns3::TestSuite::TestSuite(std::string name, ns3::TestSuite::TestType type=::ns3::TestSuite::UNIT) [constructor]
    cls.add_constructor([param('std::string', 'name'), param('ns3::TestSuite::TestType', 'type', default_value='::ns3::TestSuite::UNIT')])
    ## test.h: bool ns3::TestSuite::Run() [member function]
    cls.add_method('Run', 
                   'bool', 
                   [])
    ## test.h: uint32_t ns3::TestSuite::AddTestCase(ns3::TestCase * testCase) [member function]
    cls.add_method('AddTestCase', 
                   'uint32_t', 
                   [param('ns3::TestCase *', 'testCase')])
    ## test.h: uint32_t ns3::TestSuite::GetNTestCases() [member function]
    cls.add_method('GetNTestCases', 
                   'uint32_t', 
                   [])
    ## test.h: ns3::TestCase * ns3::TestSuite::GetTestCase(uint32_t i) [member function]
    cls.add_method('GetTestCase', 
                   'ns3::TestCase *', 
                   [param('uint32_t', 'i')])
    ## test.h: ns3::TestSuite::TestType ns3::TestSuite::GetTestType() [member function]
    cls.add_method('GetTestType', 
                   'ns3::TestSuite::TestType', 
                   [])
    ## test.h: void ns3::TestSuite::SetVerbose(bool verbose) [member function]
    cls.add_method('SetVerbose', 
                   'void', 
                   [param('bool', 'verbose')])
    ## test.h: void ns3::TestSuite::SetContinueOnFailure(bool continueOnFailure) [member function]
    cls.add_method('SetContinueOnFailure', 
                   'void', 
                   [param('bool', 'continueOnFailure')])
    ## test.h: void ns3::TestSuite::SetName(std::string name) [member function]
    cls.add_method('SetName', 
                   'void', 
                   [param('std::string', 'name')])
    ## test.h: std::string ns3::TestSuite::GetName() [member function]
    cls.add_method('GetName', 
                   'std::string', 
                   [])
    ## test.h: void ns3::TestSuite::SetBaseDir(std::string basedir) [member function]
    cls.add_method('SetBaseDir', 
                   'void', 
                   [param('std::string', 'basedir')])
    ## test.h: std::string ns3::TestSuite::GetBaseDir() [member function]
    cls.add_method('GetBaseDir', 
                   'std::string', 
                   [])
    ## test.h: void ns3::TestSuite::SetTempDir(std::string dir) [member function]
    cls.add_method('SetTempDir', 
                   'void', 
                   [param('std::string', 'dir')])
    ## test.h: std::string ns3::TestSuite::GetTempDir() [member function]
    cls.add_method('GetTempDir', 
                   'std::string', 
                   [])
    ## test.h: void ns3::TestSuite::SetStream(std::ofstream * ofs) [member function]
    cls.add_method('SetStream', 
                   'void', 
                   [param('std::ofstream *', 'ofs')])
    ## test.h: void ns3::TestSuite::UpdateErrorStatus(bool error) [member function]
    cls.add_method('UpdateErrorStatus', 
                   'void', 
                   [param('bool', 'error')])
    ## test.h: void ns3::TestSuite::SetErrorStatus(bool error) [member function]
    cls.add_method('SetErrorStatus', 
                   'void', 
                   [param('bool', 'error')])
    ## test.h: bool ns3::TestSuite::GetErrorStatus() [member function]
    cls.add_method('GetErrorStatus', 
                   'bool', 
                   [])
    ## test.h: bool ns3::TestSuite::ContinueOnFailure() [member function]
    cls.add_method('ContinueOnFailure', 
                   'bool', 
                   [])
    ## test.h: void ns3::TestSuite::ReportStart() [member function]
    cls.add_method('ReportStart', 
                   'void', 
                   [])
    ## test.h: void ns3::TestSuite::ReportSuccess() [member function]
    cls.add_method('ReportSuccess', 
                   'void', 
                   [])
    ## test.h: void ns3::TestSuite::ReportFailure() [member function]
    cls.add_method('ReportFailure', 
                   'void', 
                   [])
    ## test.h: void ns3::TestSuite::ReportEnd() [member function]
    cls.add_method('ReportEnd', 
                   'void', 
                   [])
    ## test.h: void ns3::TestSuite::DoReportStart() [member function]
    cls.add_method('DoReportStart', 
                   'void', 
                   [], 
                   visibility='protected', is_virtual=True)
    ## test.h: void ns3::TestSuite::DoReportSuccess() [member function]
    cls.add_method('DoReportSuccess', 
                   'void', 
                   [], 
                   visibility='protected', is_virtual=True)
    ## test.h: void ns3::TestSuite::DoReportFailure() [member function]
    cls.add_method('DoReportFailure', 
                   'void', 
                   [], 
                   visibility='protected', is_virtual=True)
    ## test.h: void ns3::TestSuite::DoReportEnd() [member function]
    cls.add_method('DoReportEnd', 
                   'void', 
                   [], 
                   visibility='protected', is_virtual=True)
    ## test.h: void ns3::TestSuite::DoSetup() [member function]
    cls.add_method('DoSetup', 
                   'void', 
                   [], 
                   visibility='protected', is_virtual=True)
    ## test.h: bool ns3::TestSuite::DoRun() [member function]
    cls.add_method('DoRun', 
                   'bool', 
                   [], 
                   visibility='protected', is_virtual=True)
    ## test.h: void ns3::TestSuite::DoTeardown() [member function]
    cls.add_method('DoTeardown', 
                   'void', 
                   [], 
                   visibility='protected', is_virtual=True)
    return

def register_Ns3TracedValue__Double_methods(root_module, cls):
    ## traced-value.h: ns3::TracedValue<double>::TracedValue() [constructor]
    cls.add_constructor([])
    ## traced-value.h: ns3::TracedValue<double>::TracedValue(ns3::TracedValue<double> const & o) [copy constructor]
    cls.add_constructor([param('ns3::TracedValue< double > const &', 'o')])
    ## traced-value.h: ns3::TracedValue<double>::TracedValue(double const & v) [constructor]
    cls.add_constructor([param('double const &', 'v')])
    ## traced-value.h: void ns3::TracedValue<double>::Connect(ns3::CallbackBase const & cb, std::basic_string<char,std::char_traits<char>,std::allocator<char> > path) [member function]
    cls.add_method('Connect', 
                   'void', 
                   [param('ns3::CallbackBase const &', 'cb'), param('std::string', 'path')])
    ## traced-value.h: void ns3::TracedValue<double>::ConnectWithoutContext(ns3::CallbackBase const & cb) [member function]
    cls.add_method('ConnectWithoutContext', 
                   'void', 
                   [param('ns3::CallbackBase const &', 'cb')])
    ## traced-value.h: void ns3::TracedValue<double>::Disconnect(ns3::CallbackBase const & cb, std::basic_string<char,std::char_traits<char>,std::allocator<char> > path) [member function]
    cls.add_method('Disconnect', 
                   'void', 
                   [param('ns3::CallbackBase const &', 'cb'), param('std::string', 'path')])
    ## traced-value.h: void ns3::TracedValue<double>::DisconnectWithoutContext(ns3::CallbackBase const & cb) [member function]
    cls.add_method('DisconnectWithoutContext', 
                   'void', 
                   [param('ns3::CallbackBase const &', 'cb')])
    ## traced-value.h: double ns3::TracedValue<double>::Get() const [member function]
    cls.add_method('Get', 
                   'double', 
                   [], 
                   is_const=True)
    ## traced-value.h: void ns3::TracedValue<double>::Set(double const & v) [member function]
    cls.add_method('Set', 
                   'void', 
                   [param('double const &', 'v')])
    return

def register_Ns3TriangularVariable_methods(root_module, cls):
    ## random-variable.h: ns3::TriangularVariable::TriangularVariable(ns3::TriangularVariable const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::TriangularVariable const &', 'arg0')])
    ## random-variable.h: ns3::TriangularVariable::TriangularVariable() [constructor]
    cls.add_constructor([])
    ## random-variable.h: ns3::TriangularVariable::TriangularVariable(double s, double l, double mean) [constructor]
    cls.add_constructor([param('double', 's'), param('double', 'l'), param('double', 'mean')])
    return

def register_Ns3TypeId_methods(root_module, cls):
    cls.add_binary_comparison_operator('!=')
    cls.add_output_stream_operator()
    cls.add_binary_comparison_operator('==')
    cls.add_binary_comparison_operator('<')
    ## type-id.h: ns3::TypeId::TypeId(char const * name) [constructor]
    cls.add_constructor([param('char const *', 'name')])
    ## type-id.h: ns3::TypeId::TypeId() [constructor]
    cls.add_constructor([])
    ## type-id.h: ns3::TypeId::TypeId(ns3::TypeId const & o) [copy constructor]
    cls.add_constructor([param('ns3::TypeId const &', 'o')])
    ## type-id.h: ns3::TypeId ns3::TypeId::AddAttribute(std::string name, std::string help, ns3::AttributeValue const & initialValue, ns3::Ptr<ns3::AttributeAccessor const> accessor, ns3::Ptr<ns3::AttributeChecker const> checker) [member function]
    cls.add_method('AddAttribute', 
                   'ns3::TypeId', 
                   [param('std::string', 'name'), param('std::string', 'help'), param('ns3::AttributeValue const &', 'initialValue'), param('ns3::Ptr< ns3::AttributeAccessor const >', 'accessor'), param('ns3::Ptr< ns3::AttributeChecker const >', 'checker')])
    ## type-id.h: ns3::TypeId ns3::TypeId::AddAttribute(std::string name, std::string help, uint32_t flags, ns3::AttributeValue const & initialValue, ns3::Ptr<ns3::AttributeAccessor const> accessor, ns3::Ptr<ns3::AttributeChecker const> checker) [member function]
    cls.add_method('AddAttribute', 
                   'ns3::TypeId', 
                   [param('std::string', 'name'), param('std::string', 'help'), param('uint32_t', 'flags'), param('ns3::AttributeValue const &', 'initialValue'), param('ns3::Ptr< ns3::AttributeAccessor const >', 'accessor'), param('ns3::Ptr< ns3::AttributeChecker const >', 'checker')])
    ## type-id.h: ns3::TypeId ns3::TypeId::AddTraceSource(std::string name, std::string help, ns3::Ptr<ns3::TraceSourceAccessor const> accessor) [member function]
    cls.add_method('AddTraceSource', 
                   'ns3::TypeId', 
                   [param('std::string', 'name'), param('std::string', 'help'), param('ns3::Ptr< ns3::TraceSourceAccessor const >', 'accessor')])
    ## type-id.h: ns3::Ptr<ns3::AttributeAccessor const> ns3::TypeId::GetAttributeAccessor(uint32_t i) const [member function]
    cls.add_method('GetAttributeAccessor', 
                   'ns3::Ptr< ns3::AttributeAccessor const >', 
                   [param('uint32_t', 'i')], 
                   is_const=True)
    ## type-id.h: ns3::Ptr<ns3::AttributeChecker const> ns3::TypeId::GetAttributeChecker(uint32_t i) const [member function]
    cls.add_method('GetAttributeChecker', 
                   'ns3::Ptr< ns3::AttributeChecker const >', 
                   [param('uint32_t', 'i')], 
                   is_const=True)
    ## type-id.h: uint32_t ns3::TypeId::GetAttributeFlags(uint32_t i) const [member function]
    cls.add_method('GetAttributeFlags', 
                   'uint32_t', 
                   [param('uint32_t', 'i')], 
                   is_const=True)
    ## type-id.h: std::string ns3::TypeId::GetAttributeFullName(uint32_t i) const [member function]
    cls.add_method('GetAttributeFullName', 
                   'std::string', 
                   [param('uint32_t', 'i')], 
                   is_const=True)
    ## type-id.h: std::string ns3::TypeId::GetAttributeHelp(uint32_t i) const [member function]
    cls.add_method('GetAttributeHelp', 
                   'std::string', 
                   [param('uint32_t', 'i')], 
                   is_const=True)
    ## type-id.h: ns3::Ptr<ns3::AttributeValue const> ns3::TypeId::GetAttributeInitialValue(uint32_t i) const [member function]
    cls.add_method('GetAttributeInitialValue', 
                   'ns3::Ptr< ns3::AttributeValue const >', 
                   [param('uint32_t', 'i')], 
                   is_const=True)
    ## type-id.h: uint32_t ns3::TypeId::GetAttributeN() const [member function]
    cls.add_method('GetAttributeN', 
                   'uint32_t', 
                   [], 
                   is_const=True)
    ## type-id.h: std::string ns3::TypeId::GetAttributeName(uint32_t i) const [member function]
    cls.add_method('GetAttributeName', 
                   'std::string', 
                   [param('uint32_t', 'i')], 
                   is_const=True)
    ## type-id.h: ns3::Callback<ns3::ObjectBase*,ns3::empty,ns3::empty,ns3::empty,ns3::empty,ns3::empty,ns3::empty,ns3::empty,ns3::empty,ns3::empty> ns3::TypeId::GetConstructor() const [member function]
    cls.add_method('GetConstructor', 
                   'ns3::Callback< ns3::ObjectBase *, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty >', 
                   [], 
                   is_const=True)
    ## type-id.h: std::string ns3::TypeId::GetGroupName() const [member function]
    cls.add_method('GetGroupName', 
                   'std::string', 
                   [], 
                   is_const=True)
    ## type-id.h: std::string ns3::TypeId::GetName() const [member function]
    cls.add_method('GetName', 
                   'std::string', 
                   [], 
                   is_const=True)
    ## type-id.h: ns3::TypeId ns3::TypeId::GetParent() const [member function]
    cls.add_method('GetParent', 
                   'ns3::TypeId', 
                   [], 
                   is_const=True)
    ## type-id.h: static ns3::TypeId ns3::TypeId::GetRegistered(uint32_t i) [member function]
    cls.add_method('GetRegistered', 
                   'ns3::TypeId', 
                   [param('uint32_t', 'i')], 
                   is_static=True)
    ## type-id.h: static uint32_t ns3::TypeId::GetRegisteredN() [member function]
    cls.add_method('GetRegisteredN', 
                   'uint32_t', 
                   [], 
                   is_static=True)
    ## type-id.h: ns3::Ptr<ns3::TraceSourceAccessor const> ns3::TypeId::GetTraceSourceAccessor(uint32_t i) const [member function]
    cls.add_method('GetTraceSourceAccessor', 
                   'ns3::Ptr< ns3::TraceSourceAccessor const >', 
                   [param('uint32_t', 'i')], 
                   is_const=True)
    ## type-id.h: std::string ns3::TypeId::GetTraceSourceHelp(uint32_t i) const [member function]
    cls.add_method('GetTraceSourceHelp', 
                   'std::string', 
                   [param('uint32_t', 'i')], 
                   is_const=True)
    ## type-id.h: uint32_t ns3::TypeId::GetTraceSourceN() const [member function]
    cls.add_method('GetTraceSourceN', 
                   'uint32_t', 
                   [], 
                   is_const=True)
    ## type-id.h: std::string ns3::TypeId::GetTraceSourceName(uint32_t i) const [member function]
    cls.add_method('GetTraceSourceName', 
                   'std::string', 
                   [param('uint32_t', 'i')], 
                   is_const=True)
    ## type-id.h: uint16_t ns3::TypeId::GetUid() const [member function]
    cls.add_method('GetUid', 
                   'uint16_t', 
                   [], 
                   is_const=True)
    ## type-id.h: bool ns3::TypeId::HasConstructor() const [member function]
    cls.add_method('HasConstructor', 
                   'bool', 
                   [], 
                   is_const=True)
    ## type-id.h: bool ns3::TypeId::HasParent() const [member function]
    cls.add_method('HasParent', 
                   'bool', 
                   [], 
                   is_const=True)
    ## type-id.h: ns3::TypeId ns3::TypeId::HideFromDocumentation() [member function]
    cls.add_method('HideFromDocumentation', 
                   'ns3::TypeId', 
                   [])
    ## type-id.h: bool ns3::TypeId::IsChildOf(ns3::TypeId other) const [member function]
    cls.add_method('IsChildOf', 
                   'bool', 
                   [param('ns3::TypeId', 'other')], 
                   is_const=True)
    ## type-id.h: static bool ns3::TypeId::LookupAttributeByFullName(std::string fullName, ns3::TypeId::AttributeInfo * info) [member function]
    cls.add_method('LookupAttributeByFullName', 
                   'bool', 
                   [param('std::string', 'fullName'), param('ns3::TypeId::AttributeInfo *', 'info')], 
                   is_static=True)
    ## type-id.h: bool ns3::TypeId::LookupAttributeByName(std::string name, ns3::TypeId::AttributeInfo * info) const [member function]
    cls.add_method('LookupAttributeByName', 
                   'bool', 
                   [param('std::string', 'name'), param('ns3::TypeId::AttributeInfo *', 'info', transfer_ownership=False)], 
                   is_const=True)
    ## type-id.h: static ns3::TypeId ns3::TypeId::LookupByName(std::string name) [member function]
    cls.add_method('LookupByName', 
                   'ns3::TypeId', 
                   [param('std::string', 'name')], 
                   is_static=True)
    ## type-id.h: ns3::Ptr<ns3::TraceSourceAccessor const> ns3::TypeId::LookupTraceSourceByName(std::string name) const [member function]
    cls.add_method('LookupTraceSourceByName', 
                   'ns3::Ptr< ns3::TraceSourceAccessor const >', 
                   [param('std::string', 'name')], 
                   is_const=True)
    ## type-id.h: bool ns3::TypeId::MustHideFromDocumentation() const [member function]
    cls.add_method('MustHideFromDocumentation', 
                   'bool', 
                   [], 
                   is_const=True)
    ## type-id.h: ns3::TypeId ns3::TypeId::SetGroupName(std::string groupName) [member function]
    cls.add_method('SetGroupName', 
                   'ns3::TypeId', 
                   [param('std::string', 'groupName')])
    ## type-id.h: ns3::TypeId ns3::TypeId::SetParent(ns3::TypeId tid) [member function]
    cls.add_method('SetParent', 
                   'ns3::TypeId', 
                   [param('ns3::TypeId', 'tid')])
    ## type-id.h: void ns3::TypeId::SetUid(uint16_t tid) [member function]
    cls.add_method('SetUid', 
                   'void', 
                   [param('uint16_t', 'tid')])
    return

def register_Ns3TypeIdAttributeInfo_methods(root_module, cls):
    ## type-id.h: ns3::TypeId::AttributeInfo::AttributeInfo() [constructor]
    cls.add_constructor([])
    ## type-id.h: ns3::TypeId::AttributeInfo::AttributeInfo(ns3::TypeId::AttributeInfo const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::TypeId::AttributeInfo const &', 'arg0')])
    ## type-id.h: ns3::TypeId::AttributeInfo::accessor [variable]
    cls.add_instance_attribute('accessor', 'ns3::Ptr< ns3::AttributeAccessor const >', is_const=False)
    ## type-id.h: ns3::TypeId::AttributeInfo::checker [variable]
    cls.add_instance_attribute('checker', 'ns3::Ptr< ns3::AttributeChecker const >', is_const=False)
    ## type-id.h: ns3::TypeId::AttributeInfo::flags [variable]
    cls.add_instance_attribute('flags', 'uint32_t', is_const=False)
    ## type-id.h: ns3::TypeId::AttributeInfo::initialValue [variable]
    cls.add_instance_attribute('initialValue', 'ns3::Ptr< ns3::AttributeValue const >', is_const=False)
    return

def register_Ns3UniformVariable_methods(root_module, cls):
    ## random-variable.h: ns3::UniformVariable::UniformVariable(ns3::UniformVariable const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::UniformVariable const &', 'arg0')])
    ## random-variable.h: ns3::UniformVariable::UniformVariable() [constructor]
    cls.add_constructor([])
    ## random-variable.h: ns3::UniformVariable::UniformVariable(double s, double l) [constructor]
    cls.add_constructor([param('double', 's'), param('double', 'l')])
    ## random-variable.h: uint32_t ns3::UniformVariable::GetInteger(uint32_t s, uint32_t l) [member function]
    cls.add_method('GetInteger', 
                   'uint32_t', 
                   [param('uint32_t', 's'), param('uint32_t', 'l')])
    ## random-variable.h: double ns3::UniformVariable::GetValue() const [member function]
    cls.add_method('GetValue', 
                   'double', 
                   [], 
                   is_const=True)
    ## random-variable.h: double ns3::UniformVariable::GetValue(double s, double l) [member function]
    cls.add_method('GetValue', 
                   'double', 
                   [param('double', 's'), param('double', 'l')])
    return

def register_Ns3UnsafeAttributeList_methods(root_module, cls):
    ## attribute-list.h: ns3::UnsafeAttributeList::UnsafeAttributeList() [constructor]
    cls.add_constructor([])
    ## attribute-list.h: ns3::UnsafeAttributeList::UnsafeAttributeList(ns3::UnsafeAttributeList const & o) [copy constructor]
    cls.add_constructor([param('ns3::UnsafeAttributeList const &', 'o')])
    ## attribute-list.h: ns3::AttributeList ns3::UnsafeAttributeList::GetSafe(std::string name) const [member function]
    cls.add_method('GetSafe', 
                   'ns3::AttributeList', 
                   [param('std::string', 'name')], 
                   is_const=True)
    ## attribute-list.h: void ns3::UnsafeAttributeList::Set(std::string name, ns3::AttributeValue const & param) [member function]
    cls.add_method('Set', 
                   'void', 
                   [param('std::string', 'name'), param('ns3::AttributeValue const &', 'param')])
    return

def register_Ns3Vector2D_methods(root_module, cls):
    cls.add_output_stream_operator()
    ## vector.h: ns3::Vector2D::Vector2D(ns3::Vector2D const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::Vector2D const &', 'arg0')])
    ## vector.h: ns3::Vector2D::Vector2D(double _x, double _y) [constructor]
    cls.add_constructor([param('double', '_x'), param('double', '_y')])
    ## vector.h: ns3::Vector2D::Vector2D() [constructor]
    cls.add_constructor([])
    ## vector.h: ns3::Vector2D::x [variable]
    cls.add_instance_attribute('x', 'double', is_const=False)
    ## vector.h: ns3::Vector2D::y [variable]
    cls.add_instance_attribute('y', 'double', is_const=False)
    return

def register_Ns3Vector3D_methods(root_module, cls):
    cls.add_output_stream_operator()
    ## vector.h: ns3::Vector3D::Vector3D(ns3::Vector3D const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::Vector3D const &', 'arg0')])
    ## vector.h: ns3::Vector3D::Vector3D(double _x, double _y, double _z) [constructor]
    cls.add_constructor([param('double', '_x'), param('double', '_y'), param('double', '_z')])
    ## vector.h: ns3::Vector3D::Vector3D() [constructor]
    cls.add_constructor([])
    ## vector.h: ns3::Vector3D::x [variable]
    cls.add_instance_attribute('x', 'double', is_const=False)
    ## vector.h: ns3::Vector3D::y [variable]
    cls.add_instance_attribute('y', 'double', is_const=False)
    ## vector.h: ns3::Vector3D::z [variable]
    cls.add_instance_attribute('z', 'double', is_const=False)
    return

def register_Ns3WeibullVariable_methods(root_module, cls):
    ## random-variable.h: ns3::WeibullVariable::WeibullVariable(ns3::WeibullVariable const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::WeibullVariable const &', 'arg0')])
    ## random-variable.h: ns3::WeibullVariable::WeibullVariable() [constructor]
    cls.add_constructor([])
    ## random-variable.h: ns3::WeibullVariable::WeibullVariable(double m) [constructor]
    cls.add_constructor([param('double', 'm')])
    ## random-variable.h: ns3::WeibullVariable::WeibullVariable(double m, double s) [constructor]
    cls.add_constructor([param('double', 'm'), param('double', 's')])
    ## random-variable.h: ns3::WeibullVariable::WeibullVariable(double m, double s, double b) [constructor]
    cls.add_constructor([param('double', 'm'), param('double', 's'), param('double', 'b')])
    return

def register_Ns3ZetaVariable_methods(root_module, cls):
    ## random-variable.h: ns3::ZetaVariable::ZetaVariable(ns3::ZetaVariable const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::ZetaVariable const &', 'arg0')])
    ## random-variable.h: ns3::ZetaVariable::ZetaVariable(double alpha) [constructor]
    cls.add_constructor([param('double', 'alpha')])
    ## random-variable.h: ns3::ZetaVariable::ZetaVariable() [constructor]
    cls.add_constructor([])
    return

def register_Ns3ZipfVariable_methods(root_module, cls):
    ## random-variable.h: ns3::ZipfVariable::ZipfVariable(ns3::ZipfVariable const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::ZipfVariable const &', 'arg0')])
    ## random-variable.h: ns3::ZipfVariable::ZipfVariable(long int N, double alpha) [constructor]
    cls.add_constructor([param('long int', 'N'), param('double', 'alpha')])
    ## random-variable.h: ns3::ZipfVariable::ZipfVariable() [constructor]
    cls.add_constructor([])
    return

def register_Ns3Empty_methods(root_module, cls):
    ## empty.h: ns3::empty::empty() [constructor]
    cls.add_constructor([])
    ## empty.h: ns3::empty::empty(ns3::empty const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::empty const &', 'arg0')])
    return

def register_Ns3ConstantVariable_methods(root_module, cls):
    ## random-variable.h: ns3::ConstantVariable::ConstantVariable(ns3::ConstantVariable const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::ConstantVariable const &', 'arg0')])
    ## random-variable.h: ns3::ConstantVariable::ConstantVariable() [constructor]
    cls.add_constructor([])
    ## random-variable.h: ns3::ConstantVariable::ConstantVariable(double c) [constructor]
    cls.add_constructor([param('double', 'c')])
    ## random-variable.h: void ns3::ConstantVariable::SetConstant(double c) [member function]
    cls.add_method('SetConstant', 
                   'void', 
                   [param('double', 'c')])
    return

def register_Ns3DeterministicVariable_methods(root_module, cls):
    ## random-variable.h: ns3::DeterministicVariable::DeterministicVariable(ns3::DeterministicVariable const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::DeterministicVariable const &', 'arg0')])
    ## random-variable.h: ns3::DeterministicVariable::DeterministicVariable(double * d, uint32_t c) [constructor]
    cls.add_constructor([param('double *', 'd'), param('uint32_t', 'c')])
    return

def register_Ns3EmpiricalVariable_methods(root_module, cls):
    ## random-variable.h: ns3::EmpiricalVariable::EmpiricalVariable(ns3::EmpiricalVariable const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::EmpiricalVariable const &', 'arg0')])
    ## random-variable.h: ns3::EmpiricalVariable::EmpiricalVariable() [constructor]
    cls.add_constructor([])
    ## random-variable.h: void ns3::EmpiricalVariable::CDF(double v, double c) [member function]
    cls.add_method('CDF', 
                   'void', 
                   [param('double', 'v'), param('double', 'c')])
    return

def register_Ns3ErlangVariable_methods(root_module, cls):
    ## random-variable.h: ns3::ErlangVariable::ErlangVariable(ns3::ErlangVariable const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::ErlangVariable const &', 'arg0')])
    ## random-variable.h: ns3::ErlangVariable::ErlangVariable() [constructor]
    cls.add_constructor([])
    ## random-variable.h: ns3::ErlangVariable::ErlangVariable(unsigned int k, double lambda) [constructor]
    cls.add_constructor([param('unsigned int', 'k'), param('double', 'lambda')])
    ## random-variable.h: double ns3::ErlangVariable::GetValue() const [member function]
    cls.add_method('GetValue', 
                   'double', 
                   [], 
                   is_const=True)
    ## random-variable.h: double ns3::ErlangVariable::GetValue(unsigned int k, double lambda) const [member function]
    cls.add_method('GetValue', 
                   'double', 
                   [param('unsigned int', 'k'), param('double', 'lambda')], 
                   is_const=True)
    return

def register_Ns3ExponentialVariable_methods(root_module, cls):
    ## random-variable.h: ns3::ExponentialVariable::ExponentialVariable(ns3::ExponentialVariable const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::ExponentialVariable const &', 'arg0')])
    ## random-variable.h: ns3::ExponentialVariable::ExponentialVariable() [constructor]
    cls.add_constructor([])
    ## random-variable.h: ns3::ExponentialVariable::ExponentialVariable(double m) [constructor]
    cls.add_constructor([param('double', 'm')])
    ## random-variable.h: ns3::ExponentialVariable::ExponentialVariable(double m, double b) [constructor]
    cls.add_constructor([param('double', 'm'), param('double', 'b')])
    return

def register_Ns3GammaVariable_methods(root_module, cls):
    ## random-variable.h: ns3::GammaVariable::GammaVariable(ns3::GammaVariable const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::GammaVariable const &', 'arg0')])
    ## random-variable.h: ns3::GammaVariable::GammaVariable() [constructor]
    cls.add_constructor([])
    ## random-variable.h: ns3::GammaVariable::GammaVariable(double alpha, double beta) [constructor]
    cls.add_constructor([param('double', 'alpha'), param('double', 'beta')])
    ## random-variable.h: double ns3::GammaVariable::GetValue() const [member function]
    cls.add_method('GetValue', 
                   'double', 
                   [], 
                   is_const=True)
    ## random-variable.h: double ns3::GammaVariable::GetValue(double alpha, double beta) const [member function]
    cls.add_method('GetValue', 
                   'double', 
                   [param('double', 'alpha'), param('double', 'beta')], 
                   is_const=True)
    return

def register_Ns3IntEmpiricalVariable_methods(root_module, cls):
    ## random-variable.h: ns3::IntEmpiricalVariable::IntEmpiricalVariable(ns3::IntEmpiricalVariable const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::IntEmpiricalVariable const &', 'arg0')])
    ## random-variable.h: ns3::IntEmpiricalVariable::IntEmpiricalVariable() [constructor]
    cls.add_constructor([])
    return

def register_Ns3LogNormalVariable_methods(root_module, cls):
    ## random-variable.h: ns3::LogNormalVariable::LogNormalVariable(ns3::LogNormalVariable const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::LogNormalVariable const &', 'arg0')])
    ## random-variable.h: ns3::LogNormalVariable::LogNormalVariable(double mu, double sigma) [constructor]
    cls.add_constructor([param('double', 'mu'), param('double', 'sigma')])
    return

def register_Ns3NormalVariable_methods(root_module, cls):
    ## random-variable.h: ns3::NormalVariable::NormalVariable(ns3::NormalVariable const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::NormalVariable const &', 'arg0')])
    ## random-variable.h: ns3::NormalVariable::NormalVariable() [constructor]
    cls.add_constructor([])
    ## random-variable.h: ns3::NormalVariable::NormalVariable(double m, double v) [constructor]
    cls.add_constructor([param('double', 'm'), param('double', 'v')])
    ## random-variable.h: ns3::NormalVariable::NormalVariable(double m, double v, double b) [constructor]
    cls.add_constructor([param('double', 'm'), param('double', 'v'), param('double', 'b')])
    return

def register_Ns3Object_methods(root_module, cls):
    ## object.h: ns3::Object::Object() [constructor]
    cls.add_constructor([])
    ## object.h: void ns3::Object::AggregateObject(ns3::Ptr<ns3::Object> other) [member function]
    cls.add_method('AggregateObject', 
                   'void', 
                   [param('ns3::Ptr< ns3::Object >', 'other')])
    ## object.h: void ns3::Object::Dispose() [member function]
    cls.add_method('Dispose', 
                   'void', 
                   [])
    ## object.h: ns3::Object::AggregateIterator ns3::Object::GetAggregateIterator() const [member function]
    cls.add_method('GetAggregateIterator', 
                   'ns3::Object::AggregateIterator', 
                   [], 
                   is_const=True)
    ## object.h: ns3::TypeId ns3::Object::GetInstanceTypeId() const [member function]
    cls.add_method('GetInstanceTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_const=True, is_virtual=True)
    ## object.h: ns3::Ptr<ns3::Object> ns3::Object::GetObject(ns3::TypeId tid) const [member function]
    cls.add_method('GetObject', 
                   'ns3::Ptr< ns3::Object >', 
                   [param('ns3::TypeId', 'tid')], 
                   is_const=True, template_parameters=['ns3::Object'], custom_template_method_name='GetObject')
    ## object.h: static ns3::TypeId ns3::Object::GetTypeId() [member function]
    cls.add_method('GetTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_static=True)
    ## object.h: void ns3::Object::Start() [member function]
    cls.add_method('Start', 
                   'void', 
                   [])
    ## object.h: ns3::Object::Object(ns3::Object const & o) [copy constructor]
    cls.add_constructor([param('ns3::Object const &', 'o')], 
                        visibility='protected')
    ## object.h: void ns3::Object::DoDispose() [member function]
    cls.add_method('DoDispose', 
                   'void', 
                   [], 
                   visibility='protected', is_virtual=True)
    ## object.h: void ns3::Object::DoStart() [member function]
    cls.add_method('DoStart', 
                   'void', 
                   [], 
                   visibility='protected', is_virtual=True)
    ## object.h: void ns3::Object::NotifyNewAggregate() [member function]
    cls.add_method('NotifyNewAggregate', 
                   'void', 
                   [], 
                   visibility='protected', is_virtual=True)
    return

def register_Ns3ObjectAggregateIterator_methods(root_module, cls):
    ## object.h: ns3::Object::AggregateIterator::AggregateIterator(ns3::Object::AggregateIterator const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::Object::AggregateIterator const &', 'arg0')])
    ## object.h: ns3::Object::AggregateIterator::AggregateIterator() [constructor]
    cls.add_constructor([])
    ## object.h: bool ns3::Object::AggregateIterator::HasNext() const [member function]
    cls.add_method('HasNext', 
                   'bool', 
                   [], 
                   is_const=True)
    ## object.h: ns3::Ptr<ns3::Object const> ns3::Object::AggregateIterator::Next() [member function]
    cls.add_method('Next', 
                   'ns3::Ptr< ns3::Object const >', 
                   [])
    return

def register_Ns3ParetoVariable_methods(root_module, cls):
    ## random-variable.h: ns3::ParetoVariable::ParetoVariable(ns3::ParetoVariable const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::ParetoVariable const &', 'arg0')])
    ## random-variable.h: ns3::ParetoVariable::ParetoVariable() [constructor]
    cls.add_constructor([])
    ## random-variable.h: ns3::ParetoVariable::ParetoVariable(double m) [constructor]
    cls.add_constructor([param('double', 'm')])
    ## random-variable.h: ns3::ParetoVariable::ParetoVariable(double m, double s) [constructor]
    cls.add_constructor([param('double', 'm'), param('double', 's')])
    ## random-variable.h: ns3::ParetoVariable::ParetoVariable(double m, double s, double b) [constructor]
    cls.add_constructor([param('double', 'm'), param('double', 's'), param('double', 'b')])
    ## random-variable.h: ns3::ParetoVariable::ParetoVariable(std::pair<double,double> params) [constructor]
    cls.add_constructor([param('std::pair< double, double >', 'params')])
    ## random-variable.h: ns3::ParetoVariable::ParetoVariable(std::pair<double,double> params, double b) [constructor]
    cls.add_constructor([param('std::pair< double, double >', 'params'), param('double', 'b')])
    return

def register_Ns3SimpleRefCount__Ns3AttributeAccessor_Ns3Empty_Ns3DefaultDeleter__lt__ns3AttributeAccessor__gt___methods(root_module, cls):
    ## simple-ref-count.h: ns3::SimpleRefCount<ns3::AttributeAccessor, ns3::empty, ns3::DefaultDeleter<ns3::AttributeAccessor> >::SimpleRefCount() [constructor]
    cls.add_constructor([])
    ## simple-ref-count.h: ns3::SimpleRefCount<ns3::AttributeAccessor, ns3::empty, ns3::DefaultDeleter<ns3::AttributeAccessor> >::SimpleRefCount(ns3::SimpleRefCount<ns3::AttributeAccessor, ns3::empty, ns3::DefaultDeleter<ns3::AttributeAccessor> > const & o) [copy constructor]
    cls.add_constructor([param('ns3::SimpleRefCount< ns3::AttributeAccessor, ns3::empty, ns3::DefaultDeleter< ns3::AttributeAccessor > > const &', 'o')])
    ## simple-ref-count.h: static void ns3::SimpleRefCount<ns3::AttributeAccessor, ns3::empty, ns3::DefaultDeleter<ns3::AttributeAccessor> >::Cleanup() [member function]
    cls.add_method('Cleanup', 
                   'void', 
                   [], 
                   is_static=True)
    return

def register_Ns3SimpleRefCount__Ns3AttributeChecker_Ns3Empty_Ns3DefaultDeleter__lt__ns3AttributeChecker__gt___methods(root_module, cls):
    ## simple-ref-count.h: ns3::SimpleRefCount<ns3::AttributeChecker, ns3::empty, ns3::DefaultDeleter<ns3::AttributeChecker> >::SimpleRefCount() [constructor]
    cls.add_constructor([])
    ## simple-ref-count.h: ns3::SimpleRefCount<ns3::AttributeChecker, ns3::empty, ns3::DefaultDeleter<ns3::AttributeChecker> >::SimpleRefCount(ns3::SimpleRefCount<ns3::AttributeChecker, ns3::empty, ns3::DefaultDeleter<ns3::AttributeChecker> > const & o) [copy constructor]
    cls.add_constructor([param('ns3::SimpleRefCount< ns3::AttributeChecker, ns3::empty, ns3::DefaultDeleter< ns3::AttributeChecker > > const &', 'o')])
    ## simple-ref-count.h: static void ns3::SimpleRefCount<ns3::AttributeChecker, ns3::empty, ns3::DefaultDeleter<ns3::AttributeChecker> >::Cleanup() [member function]
    cls.add_method('Cleanup', 
                   'void', 
                   [], 
                   is_static=True)
    return

def register_Ns3SimpleRefCount__Ns3AttributeValue_Ns3Empty_Ns3DefaultDeleter__lt__ns3AttributeValue__gt___methods(root_module, cls):
    ## simple-ref-count.h: ns3::SimpleRefCount<ns3::AttributeValue, ns3::empty, ns3::DefaultDeleter<ns3::AttributeValue> >::SimpleRefCount() [constructor]
    cls.add_constructor([])
    ## simple-ref-count.h: ns3::SimpleRefCount<ns3::AttributeValue, ns3::empty, ns3::DefaultDeleter<ns3::AttributeValue> >::SimpleRefCount(ns3::SimpleRefCount<ns3::AttributeValue, ns3::empty, ns3::DefaultDeleter<ns3::AttributeValue> > const & o) [copy constructor]
    cls.add_constructor([param('ns3::SimpleRefCount< ns3::AttributeValue, ns3::empty, ns3::DefaultDeleter< ns3::AttributeValue > > const &', 'o')])
    ## simple-ref-count.h: static void ns3::SimpleRefCount<ns3::AttributeValue, ns3::empty, ns3::DefaultDeleter<ns3::AttributeValue> >::Cleanup() [member function]
    cls.add_method('Cleanup', 
                   'void', 
                   [], 
                   is_static=True)
    return

def register_Ns3SimpleRefCount__Ns3CallbackImplBase_Ns3Empty_Ns3DefaultDeleter__lt__ns3CallbackImplBase__gt___methods(root_module, cls):
    ## simple-ref-count.h: ns3::SimpleRefCount<ns3::CallbackImplBase, ns3::empty, ns3::DefaultDeleter<ns3::CallbackImplBase> >::SimpleRefCount() [constructor]
    cls.add_constructor([])
    ## simple-ref-count.h: ns3::SimpleRefCount<ns3::CallbackImplBase, ns3::empty, ns3::DefaultDeleter<ns3::CallbackImplBase> >::SimpleRefCount(ns3::SimpleRefCount<ns3::CallbackImplBase, ns3::empty, ns3::DefaultDeleter<ns3::CallbackImplBase> > const & o) [copy constructor]
    cls.add_constructor([param('ns3::SimpleRefCount< ns3::CallbackImplBase, ns3::empty, ns3::DefaultDeleter< ns3::CallbackImplBase > > const &', 'o')])
    ## simple-ref-count.h: static void ns3::SimpleRefCount<ns3::CallbackImplBase, ns3::empty, ns3::DefaultDeleter<ns3::CallbackImplBase> >::Cleanup() [member function]
    cls.add_method('Cleanup', 
                   'void', 
                   [], 
                   is_static=True)
    return

def register_Ns3SimpleRefCount__Ns3EventImpl_Ns3Empty_Ns3DefaultDeleter__lt__ns3EventImpl__gt___methods(root_module, cls):
    ## simple-ref-count.h: ns3::SimpleRefCount<ns3::EventImpl, ns3::empty, ns3::DefaultDeleter<ns3::EventImpl> >::SimpleRefCount() [constructor]
    cls.add_constructor([])
    ## simple-ref-count.h: ns3::SimpleRefCount<ns3::EventImpl, ns3::empty, ns3::DefaultDeleter<ns3::EventImpl> >::SimpleRefCount(ns3::SimpleRefCount<ns3::EventImpl, ns3::empty, ns3::DefaultDeleter<ns3::EventImpl> > const & o) [copy constructor]
    cls.add_constructor([param('ns3::SimpleRefCount< ns3::EventImpl, ns3::empty, ns3::DefaultDeleter< ns3::EventImpl > > const &', 'o')])
    ## simple-ref-count.h: static void ns3::SimpleRefCount<ns3::EventImpl, ns3::empty, ns3::DefaultDeleter<ns3::EventImpl> >::Cleanup() [member function]
    cls.add_method('Cleanup', 
                   'void', 
                   [], 
                   is_static=True)
    return

def register_Ns3SimpleRefCount__Ns3FlowClassifier_Ns3Empty_Ns3DefaultDeleter__lt__ns3FlowClassifier__gt___methods(root_module, cls):
    ## simple-ref-count.h: ns3::SimpleRefCount<ns3::FlowClassifier, ns3::empty, ns3::DefaultDeleter<ns3::FlowClassifier> >::SimpleRefCount() [constructor]
    cls.add_constructor([])
    ## simple-ref-count.h: ns3::SimpleRefCount<ns3::FlowClassifier, ns3::empty, ns3::DefaultDeleter<ns3::FlowClassifier> >::SimpleRefCount(ns3::SimpleRefCount<ns3::FlowClassifier, ns3::empty, ns3::DefaultDeleter<ns3::FlowClassifier> > const & o) [copy constructor]
    cls.add_constructor([param('ns3::SimpleRefCount< ns3::FlowClassifier, ns3::empty, ns3::DefaultDeleter< ns3::FlowClassifier > > const &', 'o')])
    ## simple-ref-count.h: static void ns3::SimpleRefCount<ns3::FlowClassifier, ns3::empty, ns3::DefaultDeleter<ns3::FlowClassifier> >::Cleanup() [member function]
    cls.add_method('Cleanup', 
                   'void', 
                   [], 
                   is_static=True)
    return

def register_Ns3SimpleRefCount__Ns3FlowProbe_Ns3Empty_Ns3DefaultDeleter__lt__ns3FlowProbe__gt___methods(root_module, cls):
    ## simple-ref-count.h: ns3::SimpleRefCount<ns3::FlowProbe, ns3::empty, ns3::DefaultDeleter<ns3::FlowProbe> >::SimpleRefCount() [constructor]
    cls.add_constructor([])
    ## simple-ref-count.h: ns3::SimpleRefCount<ns3::FlowProbe, ns3::empty, ns3::DefaultDeleter<ns3::FlowProbe> >::SimpleRefCount(ns3::SimpleRefCount<ns3::FlowProbe, ns3::empty, ns3::DefaultDeleter<ns3::FlowProbe> > const & o) [copy constructor]
    cls.add_constructor([param('ns3::SimpleRefCount< ns3::FlowProbe, ns3::empty, ns3::DefaultDeleter< ns3::FlowProbe > > const &', 'o')])
    ## simple-ref-count.h: static void ns3::SimpleRefCount<ns3::FlowProbe, ns3::empty, ns3::DefaultDeleter<ns3::FlowProbe> >::Cleanup() [member function]
    cls.add_method('Cleanup', 
                   'void', 
                   [], 
                   is_static=True)
    return

def register_Ns3SimpleRefCount__Ns3InterferenceHelperEvent_Ns3Empty_Ns3DefaultDeleter__lt__ns3InterferenceHelperEvent__gt___methods(root_module, cls):
    ## simple-ref-count.h: ns3::SimpleRefCount<ns3::InterferenceHelper::Event, ns3::empty, ns3::DefaultDeleter<ns3::InterferenceHelper::Event> >::SimpleRefCount() [constructor]
    cls.add_constructor([])
    ## simple-ref-count.h: ns3::SimpleRefCount<ns3::InterferenceHelper::Event, ns3::empty, ns3::DefaultDeleter<ns3::InterferenceHelper::Event> >::SimpleRefCount(ns3::SimpleRefCount<ns3::InterferenceHelper::Event, ns3::empty, ns3::DefaultDeleter<ns3::InterferenceHelper::Event> > const & o) [copy constructor]
    cls.add_constructor([param('ns3::SimpleRefCount< ns3::InterferenceHelper::Event, ns3::empty, ns3::DefaultDeleter< ns3::InterferenceHelper::Event > > const &', 'o')])
    ## simple-ref-count.h: static void ns3::SimpleRefCount<ns3::InterferenceHelper::Event, ns3::empty, ns3::DefaultDeleter<ns3::InterferenceHelper::Event> >::Cleanup() [member function]
    cls.add_method('Cleanup', 
                   'void', 
                   [], 
                   is_static=True)
    return

def register_Ns3SimpleRefCount__Ns3Ipv4MulticastRoute_Ns3Empty_Ns3DefaultDeleter__lt__ns3Ipv4MulticastRoute__gt___methods(root_module, cls):
    ## simple-ref-count.h: ns3::SimpleRefCount<ns3::Ipv4MulticastRoute, ns3::empty, ns3::DefaultDeleter<ns3::Ipv4MulticastRoute> >::SimpleRefCount() [constructor]
    cls.add_constructor([])
    ## simple-ref-count.h: ns3::SimpleRefCount<ns3::Ipv4MulticastRoute, ns3::empty, ns3::DefaultDeleter<ns3::Ipv4MulticastRoute> >::SimpleRefCount(ns3::SimpleRefCount<ns3::Ipv4MulticastRoute, ns3::empty, ns3::DefaultDeleter<ns3::Ipv4MulticastRoute> > const & o) [copy constructor]
    cls.add_constructor([param('ns3::SimpleRefCount< ns3::Ipv4MulticastRoute, ns3::empty, ns3::DefaultDeleter< ns3::Ipv4MulticastRoute > > const &', 'o')])
    ## simple-ref-count.h: static void ns3::SimpleRefCount<ns3::Ipv4MulticastRoute, ns3::empty, ns3::DefaultDeleter<ns3::Ipv4MulticastRoute> >::Cleanup() [member function]
    cls.add_method('Cleanup', 
                   'void', 
                   [], 
                   is_static=True)
    return

def register_Ns3SimpleRefCount__Ns3Ipv4Route_Ns3Empty_Ns3DefaultDeleter__lt__ns3Ipv4Route__gt___methods(root_module, cls):
    ## simple-ref-count.h: ns3::SimpleRefCount<ns3::Ipv4Route, ns3::empty, ns3::DefaultDeleter<ns3::Ipv4Route> >::SimpleRefCount() [constructor]
    cls.add_constructor([])
    ## simple-ref-count.h: ns3::SimpleRefCount<ns3::Ipv4Route, ns3::empty, ns3::DefaultDeleter<ns3::Ipv4Route> >::SimpleRefCount(ns3::SimpleRefCount<ns3::Ipv4Route, ns3::empty, ns3::DefaultDeleter<ns3::Ipv4Route> > const & o) [copy constructor]
    cls.add_constructor([param('ns3::SimpleRefCount< ns3::Ipv4Route, ns3::empty, ns3::DefaultDeleter< ns3::Ipv4Route > > const &', 'o')])
    ## simple-ref-count.h: static void ns3::SimpleRefCount<ns3::Ipv4Route, ns3::empty, ns3::DefaultDeleter<ns3::Ipv4Route> >::Cleanup() [member function]
    cls.add_method('Cleanup', 
                   'void', 
                   [], 
                   is_static=True)
    return

def register_Ns3SimpleRefCount__Ns3Ipv6MulticastRoute_Ns3Empty_Ns3DefaultDeleter__lt__ns3Ipv6MulticastRoute__gt___methods(root_module, cls):
    ## simple-ref-count.h: ns3::SimpleRefCount<ns3::Ipv6MulticastRoute, ns3::empty, ns3::DefaultDeleter<ns3::Ipv6MulticastRoute> >::SimpleRefCount() [constructor]
    cls.add_constructor([])
    ## simple-ref-count.h: ns3::SimpleRefCount<ns3::Ipv6MulticastRoute, ns3::empty, ns3::DefaultDeleter<ns3::Ipv6MulticastRoute> >::SimpleRefCount(ns3::SimpleRefCount<ns3::Ipv6MulticastRoute, ns3::empty, ns3::DefaultDeleter<ns3::Ipv6MulticastRoute> > const & o) [copy constructor]
    cls.add_constructor([param('ns3::SimpleRefCount< ns3::Ipv6MulticastRoute, ns3::empty, ns3::DefaultDeleter< ns3::Ipv6MulticastRoute > > const &', 'o')])
    ## simple-ref-count.h: static void ns3::SimpleRefCount<ns3::Ipv6MulticastRoute, ns3::empty, ns3::DefaultDeleter<ns3::Ipv6MulticastRoute> >::Cleanup() [member function]
    cls.add_method('Cleanup', 
                   'void', 
                   [], 
                   is_static=True)
    return

def register_Ns3SimpleRefCount__Ns3Ipv6Route_Ns3Empty_Ns3DefaultDeleter__lt__ns3Ipv6Route__gt___methods(root_module, cls):
    ## simple-ref-count.h: ns3::SimpleRefCount<ns3::Ipv6Route, ns3::empty, ns3::DefaultDeleter<ns3::Ipv6Route> >::SimpleRefCount() [constructor]
    cls.add_constructor([])
    ## simple-ref-count.h: ns3::SimpleRefCount<ns3::Ipv6Route, ns3::empty, ns3::DefaultDeleter<ns3::Ipv6Route> >::SimpleRefCount(ns3::SimpleRefCount<ns3::Ipv6Route, ns3::empty, ns3::DefaultDeleter<ns3::Ipv6Route> > const & o) [copy constructor]
    cls.add_constructor([param('ns3::SimpleRefCount< ns3::Ipv6Route, ns3::empty, ns3::DefaultDeleter< ns3::Ipv6Route > > const &', 'o')])
    ## simple-ref-count.h: static void ns3::SimpleRefCount<ns3::Ipv6Route, ns3::empty, ns3::DefaultDeleter<ns3::Ipv6Route> >::Cleanup() [member function]
    cls.add_method('Cleanup', 
                   'void', 
                   [], 
                   is_static=True)
    return

def register_Ns3SimpleRefCount__Ns3MeshWifiInterfaceMacPlugin_Ns3Empty_Ns3DefaultDeleter__lt__ns3MeshWifiInterfaceMacPlugin__gt___methods(root_module, cls):
    ## simple-ref-count.h: ns3::SimpleRefCount<ns3::MeshWifiInterfaceMacPlugin, ns3::empty, ns3::DefaultDeleter<ns3::MeshWifiInterfaceMacPlugin> >::SimpleRefCount() [constructor]
    cls.add_constructor([])
    ## simple-ref-count.h: ns3::SimpleRefCount<ns3::MeshWifiInterfaceMacPlugin, ns3::empty, ns3::DefaultDeleter<ns3::MeshWifiInterfaceMacPlugin> >::SimpleRefCount(ns3::SimpleRefCount<ns3::MeshWifiInterfaceMacPlugin, ns3::empty, ns3::DefaultDeleter<ns3::MeshWifiInterfaceMacPlugin> > const & o) [copy constructor]
    cls.add_constructor([param('ns3::SimpleRefCount< ns3::MeshWifiInterfaceMacPlugin, ns3::empty, ns3::DefaultDeleter< ns3::MeshWifiInterfaceMacPlugin > > const &', 'o')])
    ## simple-ref-count.h: static void ns3::SimpleRefCount<ns3::MeshWifiInterfaceMacPlugin, ns3::empty, ns3::DefaultDeleter<ns3::MeshWifiInterfaceMacPlugin> >::Cleanup() [member function]
    cls.add_method('Cleanup', 
                   'void', 
                   [], 
                   is_static=True)
    return

def register_Ns3SimpleRefCount__Ns3NixVector_Ns3Empty_Ns3DefaultDeleter__lt__ns3NixVector__gt___methods(root_module, cls):
    ## simple-ref-count.h: ns3::SimpleRefCount<ns3::NixVector, ns3::empty, ns3::DefaultDeleter<ns3::NixVector> >::SimpleRefCount() [constructor]
    cls.add_constructor([])
    ## simple-ref-count.h: ns3::SimpleRefCount<ns3::NixVector, ns3::empty, ns3::DefaultDeleter<ns3::NixVector> >::SimpleRefCount(ns3::SimpleRefCount<ns3::NixVector, ns3::empty, ns3::DefaultDeleter<ns3::NixVector> > const & o) [copy constructor]
    cls.add_constructor([param('ns3::SimpleRefCount< ns3::NixVector, ns3::empty, ns3::DefaultDeleter< ns3::NixVector > > const &', 'o')])
    ## simple-ref-count.h: static void ns3::SimpleRefCount<ns3::NixVector, ns3::empty, ns3::DefaultDeleter<ns3::NixVector> >::Cleanup() [member function]
    cls.add_method('Cleanup', 
                   'void', 
                   [], 
                   is_static=True)
    return

def register_Ns3SimpleRefCount__Ns3OutputStreamWrapper_Ns3Empty_Ns3DefaultDeleter__lt__ns3OutputStreamWrapper__gt___methods(root_module, cls):
    ## simple-ref-count.h: ns3::SimpleRefCount<ns3::OutputStreamWrapper, ns3::empty, ns3::DefaultDeleter<ns3::OutputStreamWrapper> >::SimpleRefCount() [constructor]
    cls.add_constructor([])
    ## simple-ref-count.h: ns3::SimpleRefCount<ns3::OutputStreamWrapper, ns3::empty, ns3::DefaultDeleter<ns3::OutputStreamWrapper> >::SimpleRefCount(ns3::SimpleRefCount<ns3::OutputStreamWrapper, ns3::empty, ns3::DefaultDeleter<ns3::OutputStreamWrapper> > const & o) [copy constructor]
    cls.add_constructor([param('ns3::SimpleRefCount< ns3::OutputStreamWrapper, ns3::empty, ns3::DefaultDeleter< ns3::OutputStreamWrapper > > const &', 'o')])
    ## simple-ref-count.h: static void ns3::SimpleRefCount<ns3::OutputStreamWrapper, ns3::empty, ns3::DefaultDeleter<ns3::OutputStreamWrapper> >::Cleanup() [member function]
    cls.add_method('Cleanup', 
                   'void', 
                   [], 
                   is_static=True)
    return

def register_Ns3SimpleRefCount__Ns3Packet_Ns3Empty_Ns3DefaultDeleter__lt__ns3Packet__gt___methods(root_module, cls):
    ## simple-ref-count.h: ns3::SimpleRefCount<ns3::Packet, ns3::empty, ns3::DefaultDeleter<ns3::Packet> >::SimpleRefCount() [constructor]
    cls.add_constructor([])
    ## simple-ref-count.h: ns3::SimpleRefCount<ns3::Packet, ns3::empty, ns3::DefaultDeleter<ns3::Packet> >::SimpleRefCount(ns3::SimpleRefCount<ns3::Packet, ns3::empty, ns3::DefaultDeleter<ns3::Packet> > const & o) [copy constructor]
    cls.add_constructor([param('ns3::SimpleRefCount< ns3::Packet, ns3::empty, ns3::DefaultDeleter< ns3::Packet > > const &', 'o')])
    ## simple-ref-count.h: static void ns3::SimpleRefCount<ns3::Packet, ns3::empty, ns3::DefaultDeleter<ns3::Packet> >::Cleanup() [member function]
    cls.add_method('Cleanup', 
                   'void', 
                   [], 
                   is_static=True)
    return

def register_Ns3SimpleRefCount__Ns3PbbAddressBlock_Ns3Empty_Ns3DefaultDeleter__lt__ns3PbbAddressBlock__gt___methods(root_module, cls):
    ## simple-ref-count.h: ns3::SimpleRefCount<ns3::PbbAddressBlock, ns3::empty, ns3::DefaultDeleter<ns3::PbbAddressBlock> >::SimpleRefCount() [constructor]
    cls.add_constructor([])
    ## simple-ref-count.h: ns3::SimpleRefCount<ns3::PbbAddressBlock, ns3::empty, ns3::DefaultDeleter<ns3::PbbAddressBlock> >::SimpleRefCount(ns3::SimpleRefCount<ns3::PbbAddressBlock, ns3::empty, ns3::DefaultDeleter<ns3::PbbAddressBlock> > const & o) [copy constructor]
    cls.add_constructor([param('ns3::SimpleRefCount< ns3::PbbAddressBlock, ns3::empty, ns3::DefaultDeleter< ns3::PbbAddressBlock > > const &', 'o')])
    ## simple-ref-count.h: static void ns3::SimpleRefCount<ns3::PbbAddressBlock, ns3::empty, ns3::DefaultDeleter<ns3::PbbAddressBlock> >::Cleanup() [member function]
    cls.add_method('Cleanup', 
                   'void', 
                   [], 
                   is_static=True)
    return

def register_Ns3SimpleRefCount__Ns3PbbMessage_Ns3Empty_Ns3DefaultDeleter__lt__ns3PbbMessage__gt___methods(root_module, cls):
    ## simple-ref-count.h: ns3::SimpleRefCount<ns3::PbbMessage, ns3::empty, ns3::DefaultDeleter<ns3::PbbMessage> >::SimpleRefCount() [constructor]
    cls.add_constructor([])
    ## simple-ref-count.h: ns3::SimpleRefCount<ns3::PbbMessage, ns3::empty, ns3::DefaultDeleter<ns3::PbbMessage> >::SimpleRefCount(ns3::SimpleRefCount<ns3::PbbMessage, ns3::empty, ns3::DefaultDeleter<ns3::PbbMessage> > const & o) [copy constructor]
    cls.add_constructor([param('ns3::SimpleRefCount< ns3::PbbMessage, ns3::empty, ns3::DefaultDeleter< ns3::PbbMessage > > const &', 'o')])
    ## simple-ref-count.h: static void ns3::SimpleRefCount<ns3::PbbMessage, ns3::empty, ns3::DefaultDeleter<ns3::PbbMessage> >::Cleanup() [member function]
    cls.add_method('Cleanup', 
                   'void', 
                   [], 
                   is_static=True)
    return

def register_Ns3SimpleRefCount__Ns3PbbPacket_Ns3Header_Ns3DefaultDeleter__lt__ns3PbbPacket__gt___methods(root_module, cls):
    ## simple-ref-count.h: ns3::SimpleRefCount<ns3::PbbPacket, ns3::Header, ns3::DefaultDeleter<ns3::PbbPacket> >::SimpleRefCount() [constructor]
    cls.add_constructor([])
    ## simple-ref-count.h: ns3::SimpleRefCount<ns3::PbbPacket, ns3::Header, ns3::DefaultDeleter<ns3::PbbPacket> >::SimpleRefCount(ns3::SimpleRefCount<ns3::PbbPacket, ns3::Header, ns3::DefaultDeleter<ns3::PbbPacket> > const & o) [copy constructor]
    cls.add_constructor([param('ns3::SimpleRefCount< ns3::PbbPacket, ns3::Header, ns3::DefaultDeleter< ns3::PbbPacket > > const &', 'o')])
    ## simple-ref-count.h: static void ns3::SimpleRefCount<ns3::PbbPacket, ns3::Header, ns3::DefaultDeleter<ns3::PbbPacket> >::Cleanup() [member function]
    cls.add_method('Cleanup', 
                   'void', 
                   [], 
                   is_static=True)
    return

def register_Ns3SimpleRefCount__Ns3PbbTlv_Ns3Empty_Ns3DefaultDeleter__lt__ns3PbbTlv__gt___methods(root_module, cls):
    ## simple-ref-count.h: ns3::SimpleRefCount<ns3::PbbTlv, ns3::empty, ns3::DefaultDeleter<ns3::PbbTlv> >::SimpleRefCount() [constructor]
    cls.add_constructor([])
    ## simple-ref-count.h: ns3::SimpleRefCount<ns3::PbbTlv, ns3::empty, ns3::DefaultDeleter<ns3::PbbTlv> >::SimpleRefCount(ns3::SimpleRefCount<ns3::PbbTlv, ns3::empty, ns3::DefaultDeleter<ns3::PbbTlv> > const & o) [copy constructor]
    cls.add_constructor([param('ns3::SimpleRefCount< ns3::PbbTlv, ns3::empty, ns3::DefaultDeleter< ns3::PbbTlv > > const &', 'o')])
    ## simple-ref-count.h: static void ns3::SimpleRefCount<ns3::PbbTlv, ns3::empty, ns3::DefaultDeleter<ns3::PbbTlv> >::Cleanup() [member function]
    cls.add_method('Cleanup', 
                   'void', 
                   [], 
                   is_static=True)
    return

def register_Ns3SimpleRefCount__Ns3RadvdInterface_Ns3Empty_Ns3DefaultDeleter__lt__ns3RadvdInterface__gt___methods(root_module, cls):
    ## simple-ref-count.h: ns3::SimpleRefCount<ns3::RadvdInterface, ns3::empty, ns3::DefaultDeleter<ns3::RadvdInterface> >::SimpleRefCount() [constructor]
    cls.add_constructor([])
    ## simple-ref-count.h: ns3::SimpleRefCount<ns3::RadvdInterface, ns3::empty, ns3::DefaultDeleter<ns3::RadvdInterface> >::SimpleRefCount(ns3::SimpleRefCount<ns3::RadvdInterface, ns3::empty, ns3::DefaultDeleter<ns3::RadvdInterface> > const & o) [copy constructor]
    cls.add_constructor([param('ns3::SimpleRefCount< ns3::RadvdInterface, ns3::empty, ns3::DefaultDeleter< ns3::RadvdInterface > > const &', 'o')])
    ## simple-ref-count.h: static void ns3::SimpleRefCount<ns3::RadvdInterface, ns3::empty, ns3::DefaultDeleter<ns3::RadvdInterface> >::Cleanup() [member function]
    cls.add_method('Cleanup', 
                   'void', 
                   [], 
                   is_static=True)
    return

def register_Ns3SimpleRefCount__Ns3RadvdPrefix_Ns3Empty_Ns3DefaultDeleter__lt__ns3RadvdPrefix__gt___methods(root_module, cls):
    ## simple-ref-count.h: ns3::SimpleRefCount<ns3::RadvdPrefix, ns3::empty, ns3::DefaultDeleter<ns3::RadvdPrefix> >::SimpleRefCount() [constructor]
    cls.add_constructor([])
    ## simple-ref-count.h: ns3::SimpleRefCount<ns3::RadvdPrefix, ns3::empty, ns3::DefaultDeleter<ns3::RadvdPrefix> >::SimpleRefCount(ns3::SimpleRefCount<ns3::RadvdPrefix, ns3::empty, ns3::DefaultDeleter<ns3::RadvdPrefix> > const & o) [copy constructor]
    cls.add_constructor([param('ns3::SimpleRefCount< ns3::RadvdPrefix, ns3::empty, ns3::DefaultDeleter< ns3::RadvdPrefix > > const &', 'o')])
    ## simple-ref-count.h: static void ns3::SimpleRefCount<ns3::RadvdPrefix, ns3::empty, ns3::DefaultDeleter<ns3::RadvdPrefix> >::Cleanup() [member function]
    cls.add_method('Cleanup', 
                   'void', 
                   [], 
                   is_static=True)
    return

def register_Ns3SimpleRefCount__Ns3RefCountBase_Ns3Empty_Ns3DefaultDeleter__lt__ns3RefCountBase__gt___methods(root_module, cls):
    ## simple-ref-count.h: ns3::SimpleRefCount<ns3::RefCountBase, ns3::empty, ns3::DefaultDeleter<ns3::RefCountBase> >::SimpleRefCount() [constructor]
    cls.add_constructor([])
    ## simple-ref-count.h: ns3::SimpleRefCount<ns3::RefCountBase, ns3::empty, ns3::DefaultDeleter<ns3::RefCountBase> >::SimpleRefCount(ns3::SimpleRefCount<ns3::RefCountBase, ns3::empty, ns3::DefaultDeleter<ns3::RefCountBase> > const & o) [copy constructor]
    cls.add_constructor([param('ns3::SimpleRefCount< ns3::RefCountBase, ns3::empty, ns3::DefaultDeleter< ns3::RefCountBase > > const &', 'o')])
    ## simple-ref-count.h: static void ns3::SimpleRefCount<ns3::RefCountBase, ns3::empty, ns3::DefaultDeleter<ns3::RefCountBase> >::Cleanup() [member function]
    cls.add_method('Cleanup', 
                   'void', 
                   [], 
                   is_static=True)
    return

def register_Ns3SimpleRefCount__Ns3SpectrumConverter_Ns3Empty_Ns3DefaultDeleter__lt__ns3SpectrumConverter__gt___methods(root_module, cls):
    ## simple-ref-count.h: ns3::SimpleRefCount<ns3::SpectrumConverter, ns3::empty, ns3::DefaultDeleter<ns3::SpectrumConverter> >::SimpleRefCount() [constructor]
    cls.add_constructor([])
    ## simple-ref-count.h: ns3::SimpleRefCount<ns3::SpectrumConverter, ns3::empty, ns3::DefaultDeleter<ns3::SpectrumConverter> >::SimpleRefCount(ns3::SimpleRefCount<ns3::SpectrumConverter, ns3::empty, ns3::DefaultDeleter<ns3::SpectrumConverter> > const & o) [copy constructor]
    cls.add_constructor([param('ns3::SimpleRefCount< ns3::SpectrumConverter, ns3::empty, ns3::DefaultDeleter< ns3::SpectrumConverter > > const &', 'o')])
    ## simple-ref-count.h: static void ns3::SimpleRefCount<ns3::SpectrumConverter, ns3::empty, ns3::DefaultDeleter<ns3::SpectrumConverter> >::Cleanup() [member function]
    cls.add_method('Cleanup', 
                   'void', 
                   [], 
                   is_static=True)
    return

def register_Ns3SimpleRefCount__Ns3SpectrumModel_Ns3Empty_Ns3DefaultDeleter__lt__ns3SpectrumModel__gt___methods(root_module, cls):
    ## simple-ref-count.h: ns3::SimpleRefCount<ns3::SpectrumModel, ns3::empty, ns3::DefaultDeleter<ns3::SpectrumModel> >::SimpleRefCount() [constructor]
    cls.add_constructor([])
    ## simple-ref-count.h: ns3::SimpleRefCount<ns3::SpectrumModel, ns3::empty, ns3::DefaultDeleter<ns3::SpectrumModel> >::SimpleRefCount(ns3::SimpleRefCount<ns3::SpectrumModel, ns3::empty, ns3::DefaultDeleter<ns3::SpectrumModel> > const & o) [copy constructor]
    cls.add_constructor([param('ns3::SimpleRefCount< ns3::SpectrumModel, ns3::empty, ns3::DefaultDeleter< ns3::SpectrumModel > > const &', 'o')])
    ## simple-ref-count.h: static void ns3::SimpleRefCount<ns3::SpectrumModel, ns3::empty, ns3::DefaultDeleter<ns3::SpectrumModel> >::Cleanup() [member function]
    cls.add_method('Cleanup', 
                   'void', 
                   [], 
                   is_static=True)
    return

def register_Ns3SimpleRefCount__Ns3SpectrumValue_Ns3Empty_Ns3DefaultDeleter__lt__ns3SpectrumValue__gt___methods(root_module, cls):
    ## simple-ref-count.h: ns3::SimpleRefCount<ns3::SpectrumValue, ns3::empty, ns3::DefaultDeleter<ns3::SpectrumValue> >::SimpleRefCount() [constructor]
    cls.add_constructor([])
    ## simple-ref-count.h: ns3::SimpleRefCount<ns3::SpectrumValue, ns3::empty, ns3::DefaultDeleter<ns3::SpectrumValue> >::SimpleRefCount(ns3::SimpleRefCount<ns3::SpectrumValue, ns3::empty, ns3::DefaultDeleter<ns3::SpectrumValue> > const & o) [copy constructor]
    cls.add_constructor([param('ns3::SimpleRefCount< ns3::SpectrumValue, ns3::empty, ns3::DefaultDeleter< ns3::SpectrumValue > > const &', 'o')])
    ## simple-ref-count.h: static void ns3::SimpleRefCount<ns3::SpectrumValue, ns3::empty, ns3::DefaultDeleter<ns3::SpectrumValue> >::Cleanup() [member function]
    cls.add_method('Cleanup', 
                   'void', 
                   [], 
                   is_static=True)
    return

def register_Ns3SimpleRefCount__Ns3SystemThread_Ns3Empty_Ns3DefaultDeleter__lt__ns3SystemThread__gt___methods(root_module, cls):
    ## simple-ref-count.h: ns3::SimpleRefCount<ns3::SystemThread, ns3::empty, ns3::DefaultDeleter<ns3::SystemThread> >::SimpleRefCount() [constructor]
    cls.add_constructor([])
    ## simple-ref-count.h: ns3::SimpleRefCount<ns3::SystemThread, ns3::empty, ns3::DefaultDeleter<ns3::SystemThread> >::SimpleRefCount(ns3::SimpleRefCount<ns3::SystemThread, ns3::empty, ns3::DefaultDeleter<ns3::SystemThread> > const & o) [copy constructor]
    cls.add_constructor([param('ns3::SimpleRefCount< ns3::SystemThread, ns3::empty, ns3::DefaultDeleter< ns3::SystemThread > > const &', 'o')])
    ## simple-ref-count.h: static void ns3::SimpleRefCount<ns3::SystemThread, ns3::empty, ns3::DefaultDeleter<ns3::SystemThread> >::Cleanup() [member function]
    cls.add_method('Cleanup', 
                   'void', 
                   [], 
                   is_static=True)
    return

def register_Ns3SimpleRefCount__Ns3TraceSourceAccessor_Ns3Empty_Ns3DefaultDeleter__lt__ns3TraceSourceAccessor__gt___methods(root_module, cls):
    ## simple-ref-count.h: ns3::SimpleRefCount<ns3::TraceSourceAccessor, ns3::empty, ns3::DefaultDeleter<ns3::TraceSourceAccessor> >::SimpleRefCount() [constructor]
    cls.add_constructor([])
    ## simple-ref-count.h: ns3::SimpleRefCount<ns3::TraceSourceAccessor, ns3::empty, ns3::DefaultDeleter<ns3::TraceSourceAccessor> >::SimpleRefCount(ns3::SimpleRefCount<ns3::TraceSourceAccessor, ns3::empty, ns3::DefaultDeleter<ns3::TraceSourceAccessor> > const & o) [copy constructor]
    cls.add_constructor([param('ns3::SimpleRefCount< ns3::TraceSourceAccessor, ns3::empty, ns3::DefaultDeleter< ns3::TraceSourceAccessor > > const &', 'o')])
    ## simple-ref-count.h: static void ns3::SimpleRefCount<ns3::TraceSourceAccessor, ns3::empty, ns3::DefaultDeleter<ns3::TraceSourceAccessor> >::Cleanup() [member function]
    cls.add_method('Cleanup', 
                   'void', 
                   [], 
                   is_static=True)
    return

def register_Ns3SimpleRefCount__Ns3WifiInformationElement_Ns3Empty_Ns3DefaultDeleter__lt__ns3WifiInformationElement__gt___methods(root_module, cls):
    ## simple-ref-count.h: ns3::SimpleRefCount<ns3::WifiInformationElement, ns3::empty, ns3::DefaultDeleter<ns3::WifiInformationElement> >::SimpleRefCount() [constructor]
    cls.add_constructor([])
    ## simple-ref-count.h: ns3::SimpleRefCount<ns3::WifiInformationElement, ns3::empty, ns3::DefaultDeleter<ns3::WifiInformationElement> >::SimpleRefCount(ns3::SimpleRefCount<ns3::WifiInformationElement, ns3::empty, ns3::DefaultDeleter<ns3::WifiInformationElement> > const & o) [copy constructor]
    cls.add_constructor([param('ns3::SimpleRefCount< ns3::WifiInformationElement, ns3::empty, ns3::DefaultDeleter< ns3::WifiInformationElement > > const &', 'o')])
    ## simple-ref-count.h: static void ns3::SimpleRefCount<ns3::WifiInformationElement, ns3::empty, ns3::DefaultDeleter<ns3::WifiInformationElement> >::Cleanup() [member function]
    cls.add_method('Cleanup', 
                   'void', 
                   [], 
                   is_static=True)
    return

def register_Ns3SimpleRefCount__Ns3Dot11sIeBeaconTimingUnit_Ns3Empty_Ns3DefaultDeleter__lt__ns3Dot11sIeBeaconTimingUnit__gt___methods(root_module, cls):
    ## simple-ref-count.h: ns3::SimpleRefCount<ns3::dot11s::IeBeaconTimingUnit, ns3::empty, ns3::DefaultDeleter<ns3::dot11s::IeBeaconTimingUnit> >::SimpleRefCount() [constructor]
    cls.add_constructor([])
    ## simple-ref-count.h: ns3::SimpleRefCount<ns3::dot11s::IeBeaconTimingUnit, ns3::empty, ns3::DefaultDeleter<ns3::dot11s::IeBeaconTimingUnit> >::SimpleRefCount(ns3::SimpleRefCount<ns3::dot11s::IeBeaconTimingUnit, ns3::empty, ns3::DefaultDeleter<ns3::dot11s::IeBeaconTimingUnit> > const & o) [copy constructor]
    cls.add_constructor([param('ns3::SimpleRefCount< ns3::dot11s::IeBeaconTimingUnit, ns3::empty, ns3::DefaultDeleter< ns3::dot11s::IeBeaconTimingUnit > > const &', 'o')])
    ## simple-ref-count.h: static void ns3::SimpleRefCount<ns3::dot11s::IeBeaconTimingUnit, ns3::empty, ns3::DefaultDeleter<ns3::dot11s::IeBeaconTimingUnit> >::Cleanup() [member function]
    cls.add_method('Cleanup', 
                   'void', 
                   [], 
                   is_static=True)
    return

def register_Ns3SystemThread_methods(root_module, cls):
    ## system-thread.h: ns3::SystemThread::SystemThread(ns3::SystemThread const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::SystemThread const &', 'arg0')])
    ## system-thread.h: ns3::SystemThread::SystemThread(ns3::Callback<void, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty> callback) [constructor]
    cls.add_constructor([param('ns3::Callback< void, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty >', 'callback')])
    ## system-thread.h: bool ns3::SystemThread::Break() [member function]
    cls.add_method('Break', 
                   'bool', 
                   [])
    ## system-thread.h: void ns3::SystemThread::Join() [member function]
    cls.add_method('Join', 
                   'void', 
                   [])
    ## system-thread.h: void ns3::SystemThread::Shutdown() [member function]
    cls.add_method('Shutdown', 
                   'void', 
                   [])
    ## system-thread.h: void ns3::SystemThread::Start() [member function]
    cls.add_method('Start', 
                   'void', 
                   [])
    return

def register_Ns3TraceSourceAccessor_methods(root_module, cls):
    ## trace-source-accessor.h: ns3::TraceSourceAccessor::TraceSourceAccessor(ns3::TraceSourceAccessor const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::TraceSourceAccessor const &', 'arg0')])
    ## trace-source-accessor.h: ns3::TraceSourceAccessor::TraceSourceAccessor() [constructor]
    cls.add_constructor([])
    ## trace-source-accessor.h: bool ns3::TraceSourceAccessor::Connect(ns3::ObjectBase * obj, std::string context, ns3::CallbackBase const & cb) const [member function]
    cls.add_method('Connect', 
                   'bool', 
                   [param('ns3::ObjectBase *', 'obj', transfer_ownership=False), param('std::string', 'context'), param('ns3::CallbackBase const &', 'cb')], 
                   is_pure_virtual=True, is_const=True, is_virtual=True)
    ## trace-source-accessor.h: bool ns3::TraceSourceAccessor::ConnectWithoutContext(ns3::ObjectBase * obj, ns3::CallbackBase const & cb) const [member function]
    cls.add_method('ConnectWithoutContext', 
                   'bool', 
                   [param('ns3::ObjectBase *', 'obj', transfer_ownership=False), param('ns3::CallbackBase const &', 'cb')], 
                   is_pure_virtual=True, is_const=True, is_virtual=True)
    ## trace-source-accessor.h: bool ns3::TraceSourceAccessor::Disconnect(ns3::ObjectBase * obj, std::string context, ns3::CallbackBase const & cb) const [member function]
    cls.add_method('Disconnect', 
                   'bool', 
                   [param('ns3::ObjectBase *', 'obj', transfer_ownership=False), param('std::string', 'context'), param('ns3::CallbackBase const &', 'cb')], 
                   is_pure_virtual=True, is_const=True, is_virtual=True)
    ## trace-source-accessor.h: bool ns3::TraceSourceAccessor::DisconnectWithoutContext(ns3::ObjectBase * obj, ns3::CallbackBase const & cb) const [member function]
    cls.add_method('DisconnectWithoutContext', 
                   'bool', 
                   [param('ns3::ObjectBase *', 'obj', transfer_ownership=False), param('ns3::CallbackBase const &', 'cb')], 
                   is_pure_virtual=True, is_const=True, is_virtual=True)
    return

def register_Ns3AttributeAccessor_methods(root_module, cls):
    ## attribute.h: ns3::AttributeAccessor::AttributeAccessor(ns3::AttributeAccessor const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::AttributeAccessor const &', 'arg0')])
    ## attribute.h: ns3::AttributeAccessor::AttributeAccessor() [constructor]
    cls.add_constructor([])
    ## attribute.h: bool ns3::AttributeAccessor::Get(ns3::ObjectBase const * object, ns3::AttributeValue & attribute) const [member function]
    cls.add_method('Get', 
                   'bool', 
                   [param('ns3::ObjectBase const *', 'object'), param('ns3::AttributeValue &', 'attribute')], 
                   is_pure_virtual=True, is_const=True, is_virtual=True)
    ## attribute.h: bool ns3::AttributeAccessor::HasGetter() const [member function]
    cls.add_method('HasGetter', 
                   'bool', 
                   [], 
                   is_pure_virtual=True, is_const=True, is_virtual=True)
    ## attribute.h: bool ns3::AttributeAccessor::HasSetter() const [member function]
    cls.add_method('HasSetter', 
                   'bool', 
                   [], 
                   is_pure_virtual=True, is_const=True, is_virtual=True)
    ## attribute.h: bool ns3::AttributeAccessor::Set(ns3::ObjectBase * object, ns3::AttributeValue const & value) const [member function]
    cls.add_method('Set', 
                   'bool', 
                   [param('ns3::ObjectBase *', 'object', transfer_ownership=False), param('ns3::AttributeValue const &', 'value')], 
                   is_pure_virtual=True, is_const=True, is_virtual=True)
    return

def register_Ns3AttributeChecker_methods(root_module, cls):
    ## attribute.h: ns3::AttributeChecker::AttributeChecker(ns3::AttributeChecker const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::AttributeChecker const &', 'arg0')])
    ## attribute.h: ns3::AttributeChecker::AttributeChecker() [constructor]
    cls.add_constructor([])
    ## attribute.h: bool ns3::AttributeChecker::Check(ns3::AttributeValue const & value) const [member function]
    cls.add_method('Check', 
                   'bool', 
                   [param('ns3::AttributeValue const &', 'value')], 
                   is_pure_virtual=True, is_const=True, is_virtual=True)
    ## attribute.h: bool ns3::AttributeChecker::Copy(ns3::AttributeValue const & source, ns3::AttributeValue & destination) const [member function]
    cls.add_method('Copy', 
                   'bool', 
                   [param('ns3::AttributeValue const &', 'source'), param('ns3::AttributeValue &', 'destination')], 
                   is_pure_virtual=True, is_const=True, is_virtual=True)
    ## attribute.h: ns3::Ptr<ns3::AttributeValue> ns3::AttributeChecker::Create() const [member function]
    cls.add_method('Create', 
                   'ns3::Ptr< ns3::AttributeValue >', 
                   [], 
                   is_pure_virtual=True, is_const=True, is_virtual=True)
    ## attribute.h: std::string ns3::AttributeChecker::GetUnderlyingTypeInformation() const [member function]
    cls.add_method('GetUnderlyingTypeInformation', 
                   'std::string', 
                   [], 
                   is_pure_virtual=True, is_const=True, is_virtual=True)
    ## attribute.h: std::string ns3::AttributeChecker::GetValueTypeName() const [member function]
    cls.add_method('GetValueTypeName', 
                   'std::string', 
                   [], 
                   is_pure_virtual=True, is_const=True, is_virtual=True)
    ## attribute.h: bool ns3::AttributeChecker::HasUnderlyingTypeInformation() const [member function]
    cls.add_method('HasUnderlyingTypeInformation', 
                   'bool', 
                   [], 
                   is_pure_virtual=True, is_const=True, is_virtual=True)
    return

def register_Ns3AttributeValue_methods(root_module, cls):
    ## attribute.h: ns3::AttributeValue::AttributeValue(ns3::AttributeValue const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::AttributeValue const &', 'arg0')])
    ## attribute.h: ns3::AttributeValue::AttributeValue() [constructor]
    cls.add_constructor([])
    ## attribute.h: ns3::Ptr<ns3::AttributeValue> ns3::AttributeValue::Copy() const [member function]
    cls.add_method('Copy', 
                   'ns3::Ptr< ns3::AttributeValue >', 
                   [], 
                   is_pure_virtual=True, is_const=True, is_virtual=True)
    ## attribute.h: bool ns3::AttributeValue::DeserializeFromString(std::string value, ns3::Ptr<ns3::AttributeChecker const> checker) [member function]
    cls.add_method('DeserializeFromString', 
                   'bool', 
                   [param('std::string', 'value'), param('ns3::Ptr< ns3::AttributeChecker const >', 'checker')], 
                   is_pure_virtual=True, is_virtual=True)
    ## attribute.h: std::string ns3::AttributeValue::SerializeToString(ns3::Ptr<ns3::AttributeChecker const> checker) const [member function]
    cls.add_method('SerializeToString', 
                   'std::string', 
                   [param('ns3::Ptr< ns3::AttributeChecker const >', 'checker')], 
                   is_pure_virtual=True, is_const=True, is_virtual=True)
    return

def register_Ns3BooleanChecker_methods(root_module, cls):
    ## boolean.h: ns3::BooleanChecker::BooleanChecker() [constructor]
    cls.add_constructor([])
    ## boolean.h: ns3::BooleanChecker::BooleanChecker(ns3::BooleanChecker const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::BooleanChecker const &', 'arg0')])
    return

def register_Ns3BooleanValue_methods(root_module, cls):
    cls.add_output_stream_operator()
    ## boolean.h: ns3::BooleanValue::BooleanValue(ns3::BooleanValue const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::BooleanValue const &', 'arg0')])
    ## boolean.h: ns3::BooleanValue::BooleanValue() [constructor]
    cls.add_constructor([])
    ## boolean.h: ns3::BooleanValue::BooleanValue(bool value) [constructor]
    cls.add_constructor([param('bool', 'value')])
    ## boolean.h: ns3::Ptr<ns3::AttributeValue> ns3::BooleanValue::Copy() const [member function]
    cls.add_method('Copy', 
                   'ns3::Ptr< ns3::AttributeValue >', 
                   [], 
                   is_const=True, is_virtual=True)
    ## boolean.h: bool ns3::BooleanValue::DeserializeFromString(std::string value, ns3::Ptr<ns3::AttributeChecker const> checker) [member function]
    cls.add_method('DeserializeFromString', 
                   'bool', 
                   [param('std::string', 'value'), param('ns3::Ptr< ns3::AttributeChecker const >', 'checker')], 
                   is_virtual=True)
    ## boolean.h: bool ns3::BooleanValue::Get() const [member function]
    cls.add_method('Get', 
                   'bool', 
                   [], 
                   is_const=True)
    ## boolean.h: std::string ns3::BooleanValue::SerializeToString(ns3::Ptr<ns3::AttributeChecker const> checker) const [member function]
    cls.add_method('SerializeToString', 
                   'std::string', 
                   [param('ns3::Ptr< ns3::AttributeChecker const >', 'checker')], 
                   is_const=True, is_virtual=True)
    ## boolean.h: void ns3::BooleanValue::Set(bool value) [member function]
    cls.add_method('Set', 
                   'void', 
                   [param('bool', 'value')])
    return

def register_Ns3CallbackChecker_methods(root_module, cls):
    ## callback.h: ns3::CallbackChecker::CallbackChecker() [constructor]
    cls.add_constructor([])
    ## callback.h: ns3::CallbackChecker::CallbackChecker(ns3::CallbackChecker const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::CallbackChecker const &', 'arg0')])
    return

def register_Ns3CallbackImplBase_methods(root_module, cls):
    ## callback.h: ns3::CallbackImplBase::CallbackImplBase() [constructor]
    cls.add_constructor([])
    ## callback.h: ns3::CallbackImplBase::CallbackImplBase(ns3::CallbackImplBase const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::CallbackImplBase const &', 'arg0')])
    ## callback.h: bool ns3::CallbackImplBase::IsEqual(ns3::Ptr<ns3::CallbackImplBase const> other) const [member function]
    cls.add_method('IsEqual', 
                   'bool', 
                   [param('ns3::Ptr< ns3::CallbackImplBase const >', 'other')], 
                   is_pure_virtual=True, is_const=True, is_virtual=True)
    return

def register_Ns3CallbackValue_methods(root_module, cls):
    ## callback.h: ns3::CallbackValue::CallbackValue(ns3::CallbackValue const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::CallbackValue const &', 'arg0')])
    ## callback.h: ns3::CallbackValue::CallbackValue() [constructor]
    cls.add_constructor([])
    ## callback.h: ns3::CallbackValue::CallbackValue(ns3::CallbackBase const & base) [constructor]
    cls.add_constructor([param('ns3::CallbackBase const &', 'base')])
    ## callback.h: ns3::Ptr<ns3::AttributeValue> ns3::CallbackValue::Copy() const [member function]
    cls.add_method('Copy', 
                   'ns3::Ptr< ns3::AttributeValue >', 
                   [], 
                   is_const=True, is_virtual=True)
    ## callback.h: bool ns3::CallbackValue::DeserializeFromString(std::string value, ns3::Ptr<ns3::AttributeChecker const> checker) [member function]
    cls.add_method('DeserializeFromString', 
                   'bool', 
                   [param('std::string', 'value'), param('ns3::Ptr< ns3::AttributeChecker const >', 'checker')], 
                   is_virtual=True)
    ## callback.h: std::string ns3::CallbackValue::SerializeToString(ns3::Ptr<ns3::AttributeChecker const> checker) const [member function]
    cls.add_method('SerializeToString', 
                   'std::string', 
                   [param('ns3::Ptr< ns3::AttributeChecker const >', 'checker')], 
                   is_const=True, is_virtual=True)
    ## callback.h: void ns3::CallbackValue::Set(ns3::CallbackBase base) [member function]
    cls.add_method('Set', 
                   'void', 
                   [param('ns3::CallbackBase', 'base')])
    return

def register_Ns3DoubleValue_methods(root_module, cls):
    ## double.h: ns3::DoubleValue::DoubleValue() [constructor]
    cls.add_constructor([])
    ## double.h: ns3::DoubleValue::DoubleValue(ns3::DoubleValue const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::DoubleValue const &', 'arg0')])
    ## double.h: ns3::DoubleValue::DoubleValue(double const & value) [constructor]
    cls.add_constructor([param('double const &', 'value')])
    ## double.h: ns3::Ptr<ns3::AttributeValue> ns3::DoubleValue::Copy() const [member function]
    cls.add_method('Copy', 
                   'ns3::Ptr< ns3::AttributeValue >', 
                   [], 
                   is_const=True, is_virtual=True)
    ## double.h: bool ns3::DoubleValue::DeserializeFromString(std::string value, ns3::Ptr<ns3::AttributeChecker const> checker) [member function]
    cls.add_method('DeserializeFromString', 
                   'bool', 
                   [param('std::string', 'value'), param('ns3::Ptr< ns3::AttributeChecker const >', 'checker')], 
                   is_virtual=True)
    ## double.h: double ns3::DoubleValue::Get() const [member function]
    cls.add_method('Get', 
                   'double', 
                   [], 
                   is_const=True)
    ## double.h: std::string ns3::DoubleValue::SerializeToString(ns3::Ptr<ns3::AttributeChecker const> checker) const [member function]
    cls.add_method('SerializeToString', 
                   'std::string', 
                   [param('ns3::Ptr< ns3::AttributeChecker const >', 'checker')], 
                   is_const=True, is_virtual=True)
    ## double.h: void ns3::DoubleValue::Set(double const & value) [member function]
    cls.add_method('Set', 
                   'void', 
                   [param('double const &', 'value')])
    return

def register_Ns3EmptyAttributeValue_methods(root_module, cls):
    ## attribute.h: ns3::EmptyAttributeValue::EmptyAttributeValue(ns3::EmptyAttributeValue const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::EmptyAttributeValue const &', 'arg0')])
    ## attribute.h: ns3::EmptyAttributeValue::EmptyAttributeValue() [constructor]
    cls.add_constructor([])
    ## attribute.h: ns3::Ptr<ns3::AttributeValue> ns3::EmptyAttributeValue::Copy() const [member function]
    cls.add_method('Copy', 
                   'ns3::Ptr< ns3::AttributeValue >', 
                   [], 
                   is_const=True, visibility='private', is_virtual=True)
    ## attribute.h: bool ns3::EmptyAttributeValue::DeserializeFromString(std::string value, ns3::Ptr<ns3::AttributeChecker const> checker) [member function]
    cls.add_method('DeserializeFromString', 
                   'bool', 
                   [param('std::string', 'value'), param('ns3::Ptr< ns3::AttributeChecker const >', 'checker')], 
                   visibility='private', is_virtual=True)
    ## attribute.h: std::string ns3::EmptyAttributeValue::SerializeToString(ns3::Ptr<ns3::AttributeChecker const> checker) const [member function]
    cls.add_method('SerializeToString', 
                   'std::string', 
                   [param('ns3::Ptr< ns3::AttributeChecker const >', 'checker')], 
                   is_const=True, visibility='private', is_virtual=True)
    return

def register_Ns3EnumChecker_methods(root_module, cls):
    ## enum.h: ns3::EnumChecker::EnumChecker(ns3::EnumChecker const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::EnumChecker const &', 'arg0')])
    ## enum.h: ns3::EnumChecker::EnumChecker() [constructor]
    cls.add_constructor([])
    ## enum.h: void ns3::EnumChecker::Add(int v, std::string name) [member function]
    cls.add_method('Add', 
                   'void', 
                   [param('int', 'v'), param('std::string', 'name')])
    ## enum.h: void ns3::EnumChecker::AddDefault(int v, std::string name) [member function]
    cls.add_method('AddDefault', 
                   'void', 
                   [param('int', 'v'), param('std::string', 'name')])
    ## enum.h: bool ns3::EnumChecker::Check(ns3::AttributeValue const & value) const [member function]
    cls.add_method('Check', 
                   'bool', 
                   [param('ns3::AttributeValue const &', 'value')], 
                   is_const=True, is_virtual=True)
    ## enum.h: bool ns3::EnumChecker::Copy(ns3::AttributeValue const & src, ns3::AttributeValue & dst) const [member function]
    cls.add_method('Copy', 
                   'bool', 
                   [param('ns3::AttributeValue const &', 'src'), param('ns3::AttributeValue &', 'dst')], 
                   is_const=True, is_virtual=True)
    ## enum.h: ns3::Ptr<ns3::AttributeValue> ns3::EnumChecker::Create() const [member function]
    cls.add_method('Create', 
                   'ns3::Ptr< ns3::AttributeValue >', 
                   [], 
                   is_const=True, is_virtual=True)
    ## enum.h: std::string ns3::EnumChecker::GetUnderlyingTypeInformation() const [member function]
    cls.add_method('GetUnderlyingTypeInformation', 
                   'std::string', 
                   [], 
                   is_const=True, is_virtual=True)
    ## enum.h: std::string ns3::EnumChecker::GetValueTypeName() const [member function]
    cls.add_method('GetValueTypeName', 
                   'std::string', 
                   [], 
                   is_const=True, is_virtual=True)
    ## enum.h: bool ns3::EnumChecker::HasUnderlyingTypeInformation() const [member function]
    cls.add_method('HasUnderlyingTypeInformation', 
                   'bool', 
                   [], 
                   is_const=True, is_virtual=True)
    return

def register_Ns3EnumValue_methods(root_module, cls):
    ## enum.h: ns3::EnumValue::EnumValue(ns3::EnumValue const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::EnumValue const &', 'arg0')])
    ## enum.h: ns3::EnumValue::EnumValue() [constructor]
    cls.add_constructor([])
    ## enum.h: ns3::EnumValue::EnumValue(int v) [constructor]
    cls.add_constructor([param('int', 'v')])
    ## enum.h: ns3::Ptr<ns3::AttributeValue> ns3::EnumValue::Copy() const [member function]
    cls.add_method('Copy', 
                   'ns3::Ptr< ns3::AttributeValue >', 
                   [], 
                   is_const=True, is_virtual=True)
    ## enum.h: bool ns3::EnumValue::DeserializeFromString(std::string value, ns3::Ptr<ns3::AttributeChecker const> checker) [member function]
    cls.add_method('DeserializeFromString', 
                   'bool', 
                   [param('std::string', 'value'), param('ns3::Ptr< ns3::AttributeChecker const >', 'checker')], 
                   is_virtual=True)
    ## enum.h: int ns3::EnumValue::Get() const [member function]
    cls.add_method('Get', 
                   'int', 
                   [], 
                   is_const=True)
    ## enum.h: std::string ns3::EnumValue::SerializeToString(ns3::Ptr<ns3::AttributeChecker const> checker) const [member function]
    cls.add_method('SerializeToString', 
                   'std::string', 
                   [param('ns3::Ptr< ns3::AttributeChecker const >', 'checker')], 
                   is_const=True, is_virtual=True)
    ## enum.h: void ns3::EnumValue::Set(int v) [member function]
    cls.add_method('Set', 
                   'void', 
                   [param('int', 'v')])
    return

def register_Ns3IntegerValue_methods(root_module, cls):
    ## integer.h: ns3::IntegerValue::IntegerValue() [constructor]
    cls.add_constructor([])
    ## integer.h: ns3::IntegerValue::IntegerValue(ns3::IntegerValue const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::IntegerValue const &', 'arg0')])
    ## integer.h: ns3::IntegerValue::IntegerValue(int64_t const & value) [constructor]
    cls.add_constructor([param('int64_t const &', 'value')])
    ## integer.h: ns3::Ptr<ns3::AttributeValue> ns3::IntegerValue::Copy() const [member function]
    cls.add_method('Copy', 
                   'ns3::Ptr< ns3::AttributeValue >', 
                   [], 
                   is_const=True, is_virtual=True)
    ## integer.h: bool ns3::IntegerValue::DeserializeFromString(std::string value, ns3::Ptr<ns3::AttributeChecker const> checker) [member function]
    cls.add_method('DeserializeFromString', 
                   'bool', 
                   [param('std::string', 'value'), param('ns3::Ptr< ns3::AttributeChecker const >', 'checker')], 
                   is_virtual=True)
    ## integer.h: int64_t ns3::IntegerValue::Get() const [member function]
    cls.add_method('Get', 
                   'int64_t', 
                   [], 
                   is_const=True)
    ## integer.h: std::string ns3::IntegerValue::SerializeToString(ns3::Ptr<ns3::AttributeChecker const> checker) const [member function]
    cls.add_method('SerializeToString', 
                   'std::string', 
                   [param('ns3::Ptr< ns3::AttributeChecker const >', 'checker')], 
                   is_const=True, is_virtual=True)
    ## integer.h: void ns3::IntegerValue::Set(int64_t const & value) [member function]
    cls.add_method('Set', 
                   'void', 
                   [param('int64_t const &', 'value')])
    return

def register_Ns3ObjectFactoryChecker_methods(root_module, cls):
    ## object-factory.h: ns3::ObjectFactoryChecker::ObjectFactoryChecker() [constructor]
    cls.add_constructor([])
    ## object-factory.h: ns3::ObjectFactoryChecker::ObjectFactoryChecker(ns3::ObjectFactoryChecker const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::ObjectFactoryChecker const &', 'arg0')])
    return

def register_Ns3ObjectFactoryValue_methods(root_module, cls):
    ## object-factory.h: ns3::ObjectFactoryValue::ObjectFactoryValue() [constructor]
    cls.add_constructor([])
    ## object-factory.h: ns3::ObjectFactoryValue::ObjectFactoryValue(ns3::ObjectFactoryValue const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::ObjectFactoryValue const &', 'arg0')])
    ## object-factory.h: ns3::ObjectFactoryValue::ObjectFactoryValue(ns3::ObjectFactory const & value) [constructor]
    cls.add_constructor([param('ns3::ObjectFactory const &', 'value')])
    ## object-factory.h: ns3::Ptr<ns3::AttributeValue> ns3::ObjectFactoryValue::Copy() const [member function]
    cls.add_method('Copy', 
                   'ns3::Ptr< ns3::AttributeValue >', 
                   [], 
                   is_const=True, is_virtual=True)
    ## object-factory.h: bool ns3::ObjectFactoryValue::DeserializeFromString(std::string value, ns3::Ptr<ns3::AttributeChecker const> checker) [member function]
    cls.add_method('DeserializeFromString', 
                   'bool', 
                   [param('std::string', 'value'), param('ns3::Ptr< ns3::AttributeChecker const >', 'checker')], 
                   is_virtual=True)
    ## object-factory.h: ns3::ObjectFactory ns3::ObjectFactoryValue::Get() const [member function]
    cls.add_method('Get', 
                   'ns3::ObjectFactory', 
                   [], 
                   is_const=True)
    ## object-factory.h: std::string ns3::ObjectFactoryValue::SerializeToString(ns3::Ptr<ns3::AttributeChecker const> checker) const [member function]
    cls.add_method('SerializeToString', 
                   'std::string', 
                   [param('ns3::Ptr< ns3::AttributeChecker const >', 'checker')], 
                   is_const=True, is_virtual=True)
    ## object-factory.h: void ns3::ObjectFactoryValue::Set(ns3::ObjectFactory const & value) [member function]
    cls.add_method('Set', 
                   'void', 
                   [param('ns3::ObjectFactory const &', 'value')])
    return

def register_Ns3ObjectVectorAccessor_methods(root_module, cls):
    ## object-vector.h: ns3::ObjectVectorAccessor::ObjectVectorAccessor() [constructor]
    cls.add_constructor([])
    ## object-vector.h: ns3::ObjectVectorAccessor::ObjectVectorAccessor(ns3::ObjectVectorAccessor const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::ObjectVectorAccessor const &', 'arg0')])
    ## object-vector.h: bool ns3::ObjectVectorAccessor::Get(ns3::ObjectBase const * object, ns3::AttributeValue & value) const [member function]
    cls.add_method('Get', 
                   'bool', 
                   [param('ns3::ObjectBase const *', 'object'), param('ns3::AttributeValue &', 'value')], 
                   is_const=True, is_virtual=True)
    ## object-vector.h: bool ns3::ObjectVectorAccessor::HasGetter() const [member function]
    cls.add_method('HasGetter', 
                   'bool', 
                   [], 
                   is_const=True, is_virtual=True)
    ## object-vector.h: bool ns3::ObjectVectorAccessor::HasSetter() const [member function]
    cls.add_method('HasSetter', 
                   'bool', 
                   [], 
                   is_const=True, is_virtual=True)
    ## object-vector.h: bool ns3::ObjectVectorAccessor::Set(ns3::ObjectBase * object, ns3::AttributeValue const & value) const [member function]
    cls.add_method('Set', 
                   'bool', 
                   [param('ns3::ObjectBase *', 'object'), param('ns3::AttributeValue const &', 'value')], 
                   is_const=True, is_virtual=True)
    ## object-vector.h: ns3::Ptr<ns3::Object> ns3::ObjectVectorAccessor::DoGet(ns3::ObjectBase const * object, uint32_t i) const [member function]
    cls.add_method('DoGet', 
                   'ns3::Ptr< ns3::Object >', 
                   [param('ns3::ObjectBase const *', 'object'), param('uint32_t', 'i')], 
                   is_pure_virtual=True, is_const=True, visibility='private', is_virtual=True)
    ## object-vector.h: bool ns3::ObjectVectorAccessor::DoGetN(ns3::ObjectBase const * object, uint32_t * n) const [member function]
    cls.add_method('DoGetN', 
                   'bool', 
                   [param('ns3::ObjectBase const *', 'object'), param('uint32_t *', 'n')], 
                   is_pure_virtual=True, is_const=True, visibility='private', is_virtual=True)
    return

def register_Ns3ObjectVectorChecker_methods(root_module, cls):
    ## object-vector.h: ns3::ObjectVectorChecker::ObjectVectorChecker() [constructor]
    cls.add_constructor([])
    ## object-vector.h: ns3::ObjectVectorChecker::ObjectVectorChecker(ns3::ObjectVectorChecker const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::ObjectVectorChecker const &', 'arg0')])
    ## object-vector.h: ns3::TypeId ns3::ObjectVectorChecker::GetItemTypeId() const [member function]
    cls.add_method('GetItemTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_pure_virtual=True, is_const=True, is_virtual=True)
    return

def register_Ns3ObjectVectorValue_methods(root_module, cls):
    ## object-vector.h: ns3::ObjectVectorValue::ObjectVectorValue(ns3::ObjectVectorValue const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::ObjectVectorValue const &', 'arg0')])
    ## object-vector.h: ns3::ObjectVectorValue::ObjectVectorValue() [constructor]
    cls.add_constructor([])
    ## object-vector.h: __gnu_cxx::__normal_iterator<const ns3::Ptr<ns3::Object>*,std::vector<ns3::Ptr<ns3::Object>, std::allocator<ns3::Ptr<ns3::Object> > > > ns3::ObjectVectorValue::Begin() const [member function]
    cls.add_method('Begin', 
                   '__gnu_cxx::__normal_iterator< ns3::Ptr< ns3::Object > const, std::vector< ns3::Ptr< ns3::Object > > >', 
                   [], 
                   is_const=True)
    ## object-vector.h: ns3::Ptr<ns3::AttributeValue> ns3::ObjectVectorValue::Copy() const [member function]
    cls.add_method('Copy', 
                   'ns3::Ptr< ns3::AttributeValue >', 
                   [], 
                   is_const=True, is_virtual=True)
    ## object-vector.h: bool ns3::ObjectVectorValue::DeserializeFromString(std::string value, ns3::Ptr<ns3::AttributeChecker const> checker) [member function]
    cls.add_method('DeserializeFromString', 
                   'bool', 
                   [param('std::string', 'value'), param('ns3::Ptr< ns3::AttributeChecker const >', 'checker')], 
                   is_virtual=True)
    ## object-vector.h: __gnu_cxx::__normal_iterator<const ns3::Ptr<ns3::Object>*,std::vector<ns3::Ptr<ns3::Object>, std::allocator<ns3::Ptr<ns3::Object> > > > ns3::ObjectVectorValue::End() const [member function]
    cls.add_method('End', 
                   '__gnu_cxx::__normal_iterator< ns3::Ptr< ns3::Object > const, std::vector< ns3::Ptr< ns3::Object > > >', 
                   [], 
                   is_const=True)
    ## object-vector.h: ns3::Ptr<ns3::Object> ns3::ObjectVectorValue::Get(uint32_t i) const [member function]
    cls.add_method('Get', 
                   'ns3::Ptr< ns3::Object >', 
                   [param('uint32_t', 'i')], 
                   is_const=True)
    ## object-vector.h: uint32_t ns3::ObjectVectorValue::GetN() const [member function]
    cls.add_method('GetN', 
                   'uint32_t', 
                   [], 
                   is_const=True)
    ## object-vector.h: std::string ns3::ObjectVectorValue::SerializeToString(ns3::Ptr<ns3::AttributeChecker const> checker) const [member function]
    cls.add_method('SerializeToString', 
                   'std::string', 
                   [param('ns3::Ptr< ns3::AttributeChecker const >', 'checker')], 
                   is_const=True, is_virtual=True)
    return

def register_Ns3PointerChecker_methods(root_module, cls):
    ## pointer.h: ns3::PointerChecker::PointerChecker() [constructor]
    cls.add_constructor([])
    ## pointer.h: ns3::PointerChecker::PointerChecker(ns3::PointerChecker const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::PointerChecker const &', 'arg0')])
    ## pointer.h: ns3::TypeId ns3::PointerChecker::GetPointeeTypeId() const [member function]
    cls.add_method('GetPointeeTypeId', 
                   'ns3::TypeId', 
                   [], 
                   is_pure_virtual=True, is_const=True, is_virtual=True)
    return

def register_Ns3PointerValue_methods(root_module, cls):
    ## pointer.h: ns3::PointerValue::PointerValue(ns3::PointerValue const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::PointerValue const &', 'arg0')])
    ## pointer.h: ns3::PointerValue::PointerValue() [constructor]
    cls.add_constructor([])
    ## pointer.h: ns3::PointerValue::PointerValue(ns3::Ptr<ns3::Object> object) [constructor]
    cls.add_constructor([param('ns3::Ptr< ns3::Object >', 'object')])
    ## pointer.h: ns3::Ptr<ns3::AttributeValue> ns3::PointerValue::Copy() const [member function]
    cls.add_method('Copy', 
                   'ns3::Ptr< ns3::AttributeValue >', 
                   [], 
                   is_const=True, is_virtual=True)
    ## pointer.h: bool ns3::PointerValue::DeserializeFromString(std::string value, ns3::Ptr<ns3::AttributeChecker const> checker) [member function]
    cls.add_method('DeserializeFromString', 
                   'bool', 
                   [param('std::string', 'value'), param('ns3::Ptr< ns3::AttributeChecker const >', 'checker')], 
                   is_virtual=True)
    ## pointer.h: ns3::Ptr<ns3::Object> ns3::PointerValue::GetObject() const [member function]
    cls.add_method('GetObject', 
                   'ns3::Ptr< ns3::Object >', 
                   [], 
                   is_const=True)
    ## pointer.h: std::string ns3::PointerValue::SerializeToString(ns3::Ptr<ns3::AttributeChecker const> checker) const [member function]
    cls.add_method('SerializeToString', 
                   'std::string', 
                   [param('ns3::Ptr< ns3::AttributeChecker const >', 'checker')], 
                   is_const=True, is_virtual=True)
    ## pointer.h: void ns3::PointerValue::SetObject(ns3::Ptr<ns3::Object> object) [member function]
    cls.add_method('SetObject', 
                   'void', 
                   [param('ns3::Ptr< ns3::Object >', 'object')])
    return

def register_Ns3RandomVariableChecker_methods(root_module, cls):
    ## random-variable.h: ns3::RandomVariableChecker::RandomVariableChecker() [constructor]
    cls.add_constructor([])
    ## random-variable.h: ns3::RandomVariableChecker::RandomVariableChecker(ns3::RandomVariableChecker const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::RandomVariableChecker const &', 'arg0')])
    return

def register_Ns3RandomVariableValue_methods(root_module, cls):
    ## random-variable.h: ns3::RandomVariableValue::RandomVariableValue() [constructor]
    cls.add_constructor([])
    ## random-variable.h: ns3::RandomVariableValue::RandomVariableValue(ns3::RandomVariableValue const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::RandomVariableValue const &', 'arg0')])
    ## random-variable.h: ns3::RandomVariableValue::RandomVariableValue(ns3::RandomVariable const & value) [constructor]
    cls.add_constructor([param('ns3::RandomVariable const &', 'value')])
    ## random-variable.h: ns3::Ptr<ns3::AttributeValue> ns3::RandomVariableValue::Copy() const [member function]
    cls.add_method('Copy', 
                   'ns3::Ptr< ns3::AttributeValue >', 
                   [], 
                   is_const=True, is_virtual=True)
    ## random-variable.h: bool ns3::RandomVariableValue::DeserializeFromString(std::string value, ns3::Ptr<ns3::AttributeChecker const> checker) [member function]
    cls.add_method('DeserializeFromString', 
                   'bool', 
                   [param('std::string', 'value'), param('ns3::Ptr< ns3::AttributeChecker const >', 'checker')], 
                   is_virtual=True)
    ## random-variable.h: ns3::RandomVariable ns3::RandomVariableValue::Get() const [member function]
    cls.add_method('Get', 
                   'ns3::RandomVariable', 
                   [], 
                   is_const=True)
    ## random-variable.h: std::string ns3::RandomVariableValue::SerializeToString(ns3::Ptr<ns3::AttributeChecker const> checker) const [member function]
    cls.add_method('SerializeToString', 
                   'std::string', 
                   [param('ns3::Ptr< ns3::AttributeChecker const >', 'checker')], 
                   is_const=True, is_virtual=True)
    ## random-variable.h: void ns3::RandomVariableValue::Set(ns3::RandomVariable const & value) [member function]
    cls.add_method('Set', 
                   'void', 
                   [param('ns3::RandomVariable const &', 'value')])
    return

def register_Ns3RefCountBase_methods(root_module, cls):
    ## ref-count-base.h: ns3::RefCountBase::RefCountBase() [constructor]
    cls.add_constructor([])
    ## ref-count-base.h: ns3::RefCountBase::RefCountBase(ns3::RefCountBase const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::RefCountBase const &', 'arg0')])
    return

def register_Ns3StringChecker_methods(root_module, cls):
    ## string.h: ns3::StringChecker::StringChecker() [constructor]
    cls.add_constructor([])
    ## string.h: ns3::StringChecker::StringChecker(ns3::StringChecker const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::StringChecker const &', 'arg0')])
    return

def register_Ns3StringValue_methods(root_module, cls):
    ## string.h: ns3::StringValue::StringValue() [constructor]
    cls.add_constructor([])
    ## string.h: ns3::StringValue::StringValue(ns3::StringValue const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::StringValue const &', 'arg0')])
    ## string.h: ns3::StringValue::StringValue(std::string const & value) [constructor]
    cls.add_constructor([param('std::string const &', 'value')])
    ## string.h: ns3::Ptr<ns3::AttributeValue> ns3::StringValue::Copy() const [member function]
    cls.add_method('Copy', 
                   'ns3::Ptr< ns3::AttributeValue >', 
                   [], 
                   is_const=True, is_virtual=True)
    ## string.h: bool ns3::StringValue::DeserializeFromString(std::string value, ns3::Ptr<ns3::AttributeChecker const> checker) [member function]
    cls.add_method('DeserializeFromString', 
                   'bool', 
                   [param('std::string', 'value'), param('ns3::Ptr< ns3::AttributeChecker const >', 'checker')], 
                   is_virtual=True)
    ## string.h: std::string ns3::StringValue::Get() const [member function]
    cls.add_method('Get', 
                   'std::string', 
                   [], 
                   is_const=True)
    ## string.h: std::string ns3::StringValue::SerializeToString(ns3::Ptr<ns3::AttributeChecker const> checker) const [member function]
    cls.add_method('SerializeToString', 
                   'std::string', 
                   [param('ns3::Ptr< ns3::AttributeChecker const >', 'checker')], 
                   is_const=True, is_virtual=True)
    ## string.h: void ns3::StringValue::Set(std::string const & value) [member function]
    cls.add_method('Set', 
                   'void', 
                   [param('std::string const &', 'value')])
    return

def register_Ns3TypeIdChecker_methods(root_module, cls):
    ## type-id.h: ns3::TypeIdChecker::TypeIdChecker() [constructor]
    cls.add_constructor([])
    ## type-id.h: ns3::TypeIdChecker::TypeIdChecker(ns3::TypeIdChecker const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::TypeIdChecker const &', 'arg0')])
    return

def register_Ns3TypeIdValue_methods(root_module, cls):
    ## type-id.h: ns3::TypeIdValue::TypeIdValue() [constructor]
    cls.add_constructor([])
    ## type-id.h: ns3::TypeIdValue::TypeIdValue(ns3::TypeIdValue const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::TypeIdValue const &', 'arg0')])
    ## type-id.h: ns3::TypeIdValue::TypeIdValue(ns3::TypeId const & value) [constructor]
    cls.add_constructor([param('ns3::TypeId const &', 'value')])
    ## type-id.h: ns3::Ptr<ns3::AttributeValue> ns3::TypeIdValue::Copy() const [member function]
    cls.add_method('Copy', 
                   'ns3::Ptr< ns3::AttributeValue >', 
                   [], 
                   is_const=True, is_virtual=True)
    ## type-id.h: bool ns3::TypeIdValue::DeserializeFromString(std::string value, ns3::Ptr<ns3::AttributeChecker const> checker) [member function]
    cls.add_method('DeserializeFromString', 
                   'bool', 
                   [param('std::string', 'value'), param('ns3::Ptr< ns3::AttributeChecker const >', 'checker')], 
                   is_virtual=True)
    ## type-id.h: ns3::TypeId ns3::TypeIdValue::Get() const [member function]
    cls.add_method('Get', 
                   'ns3::TypeId', 
                   [], 
                   is_const=True)
    ## type-id.h: std::string ns3::TypeIdValue::SerializeToString(ns3::Ptr<ns3::AttributeChecker const> checker) const [member function]
    cls.add_method('SerializeToString', 
                   'std::string', 
                   [param('ns3::Ptr< ns3::AttributeChecker const >', 'checker')], 
                   is_const=True, is_virtual=True)
    ## type-id.h: void ns3::TypeIdValue::Set(ns3::TypeId const & value) [member function]
    cls.add_method('Set', 
                   'void', 
                   [param('ns3::TypeId const &', 'value')])
    return

def register_Ns3UintegerValue_methods(root_module, cls):
    ## uinteger.h: ns3::UintegerValue::UintegerValue() [constructor]
    cls.add_constructor([])
    ## uinteger.h: ns3::UintegerValue::UintegerValue(ns3::UintegerValue const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::UintegerValue const &', 'arg0')])
    ## uinteger.h: ns3::UintegerValue::UintegerValue(uint64_t const & value) [constructor]
    cls.add_constructor([param('uint64_t const &', 'value')])
    ## uinteger.h: ns3::Ptr<ns3::AttributeValue> ns3::UintegerValue::Copy() const [member function]
    cls.add_method('Copy', 
                   'ns3::Ptr< ns3::AttributeValue >', 
                   [], 
                   is_const=True, is_virtual=True)
    ## uinteger.h: bool ns3::UintegerValue::DeserializeFromString(std::string value, ns3::Ptr<ns3::AttributeChecker const> checker) [member function]
    cls.add_method('DeserializeFromString', 
                   'bool', 
                   [param('std::string', 'value'), param('ns3::Ptr< ns3::AttributeChecker const >', 'checker')], 
                   is_virtual=True)
    ## uinteger.h: uint64_t ns3::UintegerValue::Get() const [member function]
    cls.add_method('Get', 
                   'uint64_t', 
                   [], 
                   is_const=True)
    ## uinteger.h: std::string ns3::UintegerValue::SerializeToString(ns3::Ptr<ns3::AttributeChecker const> checker) const [member function]
    cls.add_method('SerializeToString', 
                   'std::string', 
                   [param('ns3::Ptr< ns3::AttributeChecker const >', 'checker')], 
                   is_const=True, is_virtual=True)
    ## uinteger.h: void ns3::UintegerValue::Set(uint64_t const & value) [member function]
    cls.add_method('Set', 
                   'void', 
                   [param('uint64_t const &', 'value')])
    return

def register_Ns3Vector2DChecker_methods(root_module, cls):
    ## vector.h: ns3::Vector2DChecker::Vector2DChecker() [constructor]
    cls.add_constructor([])
    ## vector.h: ns3::Vector2DChecker::Vector2DChecker(ns3::Vector2DChecker const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::Vector2DChecker const &', 'arg0')])
    return

def register_Ns3Vector2DValue_methods(root_module, cls):
    ## vector.h: ns3::Vector2DValue::Vector2DValue() [constructor]
    cls.add_constructor([])
    ## vector.h: ns3::Vector2DValue::Vector2DValue(ns3::Vector2DValue const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::Vector2DValue const &', 'arg0')])
    ## vector.h: ns3::Vector2DValue::Vector2DValue(ns3::Vector2D const & value) [constructor]
    cls.add_constructor([param('ns3::Vector2D const &', 'value')])
    ## vector.h: ns3::Ptr<ns3::AttributeValue> ns3::Vector2DValue::Copy() const [member function]
    cls.add_method('Copy', 
                   'ns3::Ptr< ns3::AttributeValue >', 
                   [], 
                   is_const=True, is_virtual=True)
    ## vector.h: bool ns3::Vector2DValue::DeserializeFromString(std::string value, ns3::Ptr<ns3::AttributeChecker const> checker) [member function]
    cls.add_method('DeserializeFromString', 
                   'bool', 
                   [param('std::string', 'value'), param('ns3::Ptr< ns3::AttributeChecker const >', 'checker')], 
                   is_virtual=True)
    ## vector.h: ns3::Vector2D ns3::Vector2DValue::Get() const [member function]
    cls.add_method('Get', 
                   'ns3::Vector2D', 
                   [], 
                   is_const=True)
    ## vector.h: std::string ns3::Vector2DValue::SerializeToString(ns3::Ptr<ns3::AttributeChecker const> checker) const [member function]
    cls.add_method('SerializeToString', 
                   'std::string', 
                   [param('ns3::Ptr< ns3::AttributeChecker const >', 'checker')], 
                   is_const=True, is_virtual=True)
    ## vector.h: void ns3::Vector2DValue::Set(ns3::Vector2D const & value) [member function]
    cls.add_method('Set', 
                   'void', 
                   [param('ns3::Vector2D const &', 'value')])
    return

def register_Ns3Vector3DChecker_methods(root_module, cls):
    ## vector.h: ns3::Vector3DChecker::Vector3DChecker() [constructor]
    cls.add_constructor([])
    ## vector.h: ns3::Vector3DChecker::Vector3DChecker(ns3::Vector3DChecker const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::Vector3DChecker const &', 'arg0')])
    return

def register_Ns3Vector3DValue_methods(root_module, cls):
    ## vector.h: ns3::Vector3DValue::Vector3DValue() [constructor]
    cls.add_constructor([])
    ## vector.h: ns3::Vector3DValue::Vector3DValue(ns3::Vector3DValue const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::Vector3DValue const &', 'arg0')])
    ## vector.h: ns3::Vector3DValue::Vector3DValue(ns3::Vector3D const & value) [constructor]
    cls.add_constructor([param('ns3::Vector3D const &', 'value')])
    ## vector.h: ns3::Ptr<ns3::AttributeValue> ns3::Vector3DValue::Copy() const [member function]
    cls.add_method('Copy', 
                   'ns3::Ptr< ns3::AttributeValue >', 
                   [], 
                   is_const=True, is_virtual=True)
    ## vector.h: bool ns3::Vector3DValue::DeserializeFromString(std::string value, ns3::Ptr<ns3::AttributeChecker const> checker) [member function]
    cls.add_method('DeserializeFromString', 
                   'bool', 
                   [param('std::string', 'value'), param('ns3::Ptr< ns3::AttributeChecker const >', 'checker')], 
                   is_virtual=True)
    ## vector.h: ns3::Vector3D ns3::Vector3DValue::Get() const [member function]
    cls.add_method('Get', 
                   'ns3::Vector3D', 
                   [], 
                   is_const=True)
    ## vector.h: std::string ns3::Vector3DValue::SerializeToString(ns3::Ptr<ns3::AttributeChecker const> checker) const [member function]
    cls.add_method('SerializeToString', 
                   'std::string', 
                   [param('ns3::Ptr< ns3::AttributeChecker const >', 'checker')], 
                   is_const=True, is_virtual=True)
    ## vector.h: void ns3::Vector3DValue::Set(ns3::Vector3D const & value) [member function]
    cls.add_method('Set', 
                   'void', 
                   [param('ns3::Vector3D const &', 'value')])
    return

def register_Ns3ConfigMatchContainer_methods(root_module, cls):
    ## config.h: ns3::Config::MatchContainer::MatchContainer(ns3::Config::MatchContainer const & arg0) [copy constructor]
    cls.add_constructor([param('ns3::Config::MatchContainer const &', 'arg0')])
    ## config.h: ns3::Config::MatchContainer::MatchContainer() [constructor]
    cls.add_constructor([])
    ## config.h: ns3::Config::MatchContainer::MatchContainer(std::vector<ns3::Ptr<ns3::Object>, std::allocator<ns3::Ptr<ns3::Object> > > const & objects, std::vector<std::string, std::allocator<std::string> > const & contexts, std::string path) [constructor]
    cls.add_constructor([param('std::vector< ns3::Ptr< ns3::Object > > const &', 'objects'), param('std::vector< std::string > const &', 'contexts'), param('std::string', 'path')])
    ## config.h: __gnu_cxx::__normal_iterator<const ns3::Ptr<ns3::Object>*,std::vector<ns3::Ptr<ns3::Object>, std::allocator<ns3::Ptr<ns3::Object> > > > ns3::Config::MatchContainer::Begin() const [member function]
    cls.add_method('Begin', 
                   '__gnu_cxx::__normal_iterator< ns3::Ptr< ns3::Object > const, std::vector< ns3::Ptr< ns3::Object > > >', 
                   [], 
                   is_const=True)
    ## config.h: void ns3::Config::MatchContainer::Connect(std::string name, ns3::CallbackBase const & cb) [member function]
    cls.add_method('Connect', 
                   'void', 
                   [param('std::string', 'name'), param('ns3::CallbackBase const &', 'cb')])
    ## config.h: void ns3::Config::MatchContainer::ConnectWithoutContext(std::string name, ns3::CallbackBase const & cb) [member function]
    cls.add_method('ConnectWithoutContext', 
                   'void', 
                   [param('std::string', 'name'), param('ns3::CallbackBase const &', 'cb')])
    ## config.h: void ns3::Config::MatchContainer::Disconnect(std::string name, ns3::CallbackBase const & cb) [member function]
    cls.add_method('Disconnect', 
                   'void', 
                   [param('std::string', 'name'), param('ns3::CallbackBase const &', 'cb')])
    ## config.h: void ns3::Config::MatchContainer::DisconnectWithoutContext(std::string name, ns3::CallbackBase const & cb) [member function]
    cls.add_method('DisconnectWithoutContext', 
                   'void', 
                   [param('std::string', 'name'), param('ns3::CallbackBase const &', 'cb')])
    ## config.h: __gnu_cxx::__normal_iterator<const ns3::Ptr<ns3::Object>*,std::vector<ns3::Ptr<ns3::Object>, std::allocator<ns3::Ptr<ns3::Object> > > > ns3::Config::MatchContainer::End() const [member function]
    cls.add_method('End', 
                   '__gnu_cxx::__normal_iterator< ns3::Ptr< ns3::Object > const, std::vector< ns3::Ptr< ns3::Object > > >', 
                   [], 
                   is_const=True)
    ## config.h: ns3::Ptr<ns3::Object> ns3::Config::MatchContainer::Get(uint32_t i) const [member function]
    cls.add_method('Get', 
                   'ns3::Ptr< ns3::Object >', 
                   [param('uint32_t', 'i')], 
                   is_const=True)
    ## config.h: std::string ns3::Config::MatchContainer::GetMatchedPath(uint32_t i) const [member function]
    cls.add_method('GetMatchedPath', 
                   'std::string', 
                   [param('uint32_t', 'i')], 
                   is_const=True)
    ## config.h: uint32_t ns3::Config::MatchContainer::GetN() const [member function]
    cls.add_method('GetN', 
                   'uint32_t', 
                   [], 
                   is_const=True)
    ## config.h: std::string ns3::Config::MatchContainer::GetPath() const [member function]
    cls.add_method('GetPath', 
                   'std::string', 
                   [], 
                   is_const=True)
    ## config.h: void ns3::Config::MatchContainer::Set(std::string name, ns3::AttributeValue const & value) [member function]
    cls.add_method('Set', 
                   'void', 
                   [param('std::string', 'name'), param('ns3::AttributeValue const &', 'value')])
    return

def register_functions(root_module):
    module = root_module
    ## breakpoint.h: extern void ns3::BreakpointFallback() [free function]
    module.add_function('BreakpointFallback', 
                        'void', 
                        [])
    ## vector.h: extern double ns3::CalculateDistance(ns3::Vector2D const & a, ns3::Vector2D const & b) [free function]
    module.add_function('CalculateDistance', 
                        'double', 
                        [param('ns3::Vector2D const &', 'a'), param('ns3::Vector2D const &', 'b')])
    ## vector.h: extern double ns3::CalculateDistance(ns3::Vector3D const & a, ns3::Vector3D const & b) [free function]
    module.add_function('CalculateDistance', 
                        'double', 
                        [param('ns3::Vector3D const &', 'a'), param('ns3::Vector3D const &', 'b')])
    ## ptr.h: extern ns3::Ptr<ns3::ObjectVectorValue> ns3::Create() [free function]
    module.add_function('Create', 
                        'ns3::Ptr< ns3::ObjectVectorValue >', 
                        [], 
                        template_parameters=['ns3::ObjectVectorValue'])
    ## ptr.h: extern ns3::Ptr<ns3::PointerValue> ns3::Create() [free function]
    module.add_function('Create', 
                        'ns3::Ptr< ns3::PointerValue >', 
                        [], 
                        template_parameters=['ns3::PointerValue'])
    ## log.h: extern void ns3::LogComponentDisable(char const * name, ns3::LogLevel level) [free function]
    module.add_function('LogComponentDisable', 
                        'void', 
                        [param('char const *', 'name'), param('ns3::LogLevel', 'level')])
    ## log.h: extern void ns3::LogComponentDisableAll(ns3::LogLevel level) [free function]
    module.add_function('LogComponentDisableAll', 
                        'void', 
                        [param('ns3::LogLevel', 'level')])
    ## log.h: extern void ns3::LogComponentEnable(char const * name, ns3::LogLevel level) [free function]
    module.add_function('LogComponentEnable', 
                        'void', 
                        [param('char const *', 'name'), param('ns3::LogLevel', 'level')])
    ## log.h: extern void ns3::LogComponentEnableAll(ns3::LogLevel level) [free function]
    module.add_function('LogComponentEnableAll', 
                        'void', 
                        [param('ns3::LogLevel', 'level')])
    ## boolean.h: extern ns3::Ptr<ns3::AttributeChecker const> ns3::MakeBooleanChecker() [free function]
    module.add_function('MakeBooleanChecker', 
                        'ns3::Ptr< ns3::AttributeChecker const >', 
                        [])
    ## callback.h: extern ns3::Callback<void, ns3::Ptr<ns3::Packet const>, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty> ns3::MakeBoundCallback(void (*)( ::ns3::Ptr<ns3::OutputStreamWrapper>,::ns3::Ptr<ns3::Packet const> ) * fnPtr, ns3::Ptr<ns3::OutputStreamWrapper> a) [free function]
    module.add_function('MakeBoundCallback', 
                        'ns3::Callback< void, ns3::Ptr< ns3::Packet const >, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty >', 
                        [param('void ( * ) ( ns3::Ptr< ns3::OutputStreamWrapper >, ns3::Ptr< ns3::Packet const > ) *', 'fnPtr'), param('ns3::Ptr< ns3::OutputStreamWrapper >', 'a')], 
                        template_parameters=['void', 'ns3::Ptr<ns3::OutputStreamWrapper>', 'ns3::Ptr<ns3::OutputStreamWrapper>', 'ns3::Ptr<ns3::Packet const>'])
    ## callback.h: extern ns3::Callback<void, ns3::Ptr<ns3::Packet const>, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty> ns3::MakeBoundCallback(void (*)( ::ns3::Ptr<ns3::PcapFileWrapper>,::ns3::Ptr<ns3::Packet const> ) * fnPtr, ns3::Ptr<ns3::PcapFileWrapper> a) [free function]
    module.add_function('MakeBoundCallback', 
                        'ns3::Callback< void, ns3::Ptr< ns3::Packet const >, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty >', 
                        [param('void ( * ) ( ns3::Ptr< ns3::PcapFileWrapper >, ns3::Ptr< ns3::Packet const > ) *', 'fnPtr'), param('ns3::Ptr< ns3::PcapFileWrapper >', 'a')], 
                        template_parameters=['void', 'ns3::Ptr<ns3::PcapFileWrapper>', 'ns3::Ptr<ns3::PcapFileWrapper>', 'ns3::Ptr<ns3::Packet const>'])
    ## callback.h: extern ns3::Callback<void,std::basic_string<char, std::char_traits<char>, std::allocator<char> >,ns3::Ptr<const ns3::Packet>,ns3::empty,ns3::empty,ns3::empty,ns3::empty,ns3::empty,ns3::empty,ns3::empty> ns3::MakeBoundCallback(void (*)( ::ns3::Ptr<ns3::OutputStreamWrapper>,::std::basic_string<char,std::char_traits<char>,std::allocator<char> >,::ns3::Ptr<ns3::Packet const> ) * fnPtr, ns3::Ptr<ns3::OutputStreamWrapper> a) [free function]
    module.add_function('MakeBoundCallback', 
                        'ns3::Callback< void, std::basic_string< char, std::char_traits< char >, std::allocator< char > >, ns3::Ptr< ns3::Packet const >, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty, ns3::empty >', 
                        [param('void ( * ) ( ns3::Ptr< ns3::OutputStreamWrapper >, std::string, ns3::Ptr< ns3::Packet const > ) *', 'fnPtr'), param('ns3::Ptr< ns3::OutputStreamWrapper >', 'a')], 
                        template_parameters=['void', 'ns3::Ptr<ns3::OutputStreamWrapper>', 'ns3::Ptr<ns3::OutputStreamWrapper>', 'std::string', 'ns3::Ptr<ns3::Packet const>'])
    ## callback.h: extern ns3::Ptr<ns3::AttributeChecker const> ns3::MakeCallbackChecker() [free function]
    module.add_function('MakeCallbackChecker', 
                        'ns3::Ptr< ns3::AttributeChecker const >', 
                        [])
    ## enum.h: extern ns3::Ptr<ns3::AttributeChecker const> ns3::MakeEnumChecker(int v1, std::string n1, int v2=0, std::string n2="", int v3=0, std::string n3="", int v4=0, std::string n4="", int v5=0, std::string n5="", int v6=0, std::string n6="", int v7=0, std::string n7="", int v8=0, std::string n8="", int v9=0, std::string n9="", int v10=0, std::string n10="", int v11=0, std::string n11="", int v12=0, std::string n12="") [free function]
    module.add_function('MakeEnumChecker', 
                        'ns3::Ptr< ns3::AttributeChecker const >', 
                        [param('int', 'v1'), param('std::string', 'n1'), param('int', 'v2', default_value='0'), param('std::string', 'n2', default_value='""'), param('int', 'v3', default_value='0'), param('std::string', 'n3', default_value='""'), param('int', 'v4', default_value='0'), param('std::string', 'n4', default_value='""'), param('int', 'v5', default_value='0'), param('std::string', 'n5', default_value='""'), param('int', 'v6', default_value='0'), param('std::string', 'n6', default_value='""'), param('int', 'v7', default_value='0'), param('std::string', 'n7', default_value='""'), param('int', 'v8', default_value='0'), param('std::string', 'n8', default_value='""'), param('int', 'v9', default_value='0'), param('std::string', 'n9', default_value='""'), param('int', 'v10', default_value='0'), param('std::string', 'n10', default_value='""'), param('int', 'v11', default_value='0'), param('std::string', 'n11', default_value='""'), param('int', 'v12', default_value='0'), param('std::string', 'n12', default_value='""')])
    ## object-factory.h: extern ns3::Ptr<ns3::AttributeChecker const> ns3::MakeObjectFactoryChecker() [free function]
    module.add_function('MakeObjectFactoryChecker', 
                        'ns3::Ptr< ns3::AttributeChecker const >', 
                        [])
    ## random-variable.h: extern ns3::Ptr<ns3::AttributeChecker const> ns3::MakeRandomVariableChecker() [free function]
    module.add_function('MakeRandomVariableChecker', 
                        'ns3::Ptr< ns3::AttributeChecker const >', 
                        [])
    ## string.h: extern ns3::Ptr<ns3::AttributeChecker const> ns3::MakeStringChecker() [free function]
    module.add_function('MakeStringChecker', 
                        'ns3::Ptr< ns3::AttributeChecker const >', 
                        [])
    ## type-id.h: extern ns3::Ptr<ns3::AttributeChecker const> ns3::MakeTypeIdChecker() [free function]
    module.add_function('MakeTypeIdChecker', 
                        'ns3::Ptr< ns3::AttributeChecker const >', 
                        [])
    ## vector.h: extern ns3::Ptr<ns3::AttributeChecker const> ns3::MakeVector2DChecker() [free function]
    module.add_function('MakeVector2DChecker', 
                        'ns3::Ptr< ns3::AttributeChecker const >', 
                        [])
    ## vector.h: extern ns3::Ptr<ns3::AttributeChecker const> ns3::MakeVector3DChecker() [free function]
    module.add_function('MakeVector3DChecker', 
                        'ns3::Ptr< ns3::AttributeChecker const >', 
                        [])
    ## vector.h: extern ns3::Ptr<ns3::AttributeChecker const> ns3::MakeVectorChecker() [free function]
    module.add_function('MakeVectorChecker', 
                        'ns3::Ptr< ns3::AttributeChecker const >', 
                        [])
    ## test.h: extern bool ns3::TestDoubleIsEqual(double const a, double const b, double const epsilon=std::numeric_limits<double>::epsilon()) [free function]
    module.add_function('TestDoubleIsEqual', 
                        'bool', 
                        [param('double const', 'a'), param('double const', 'b'), param('double const', 'epsilon', default_value='std::numeric_limits<double>::epsilon()')])
    ## type-name.h: extern std::string ns3::TypeNameGet() [free function]
    module.add_function('TypeNameGet', 
                        'std::string', 
                        [], 
                        template_parameters=['double'])
    ## type-name.h: extern std::string ns3::TypeNameGet() [free function]
    module.add_function('TypeNameGet', 
                        'std::string', 
                        [], 
                        template_parameters=['float'])
    ## type-name.h: extern std::string ns3::TypeNameGet() [free function]
    module.add_function('TypeNameGet', 
                        'std::string', 
                        [], 
                        template_parameters=['long'])
    ## type-name.h: extern std::string ns3::TypeNameGet() [free function]
    module.add_function('TypeNameGet', 
                        'std::string', 
                        [], 
                        template_parameters=['int'])
    ## type-name.h: extern std::string ns3::TypeNameGet() [free function]
    module.add_function('TypeNameGet', 
                        'std::string', 
                        [], 
                        template_parameters=['short'])
    ## type-name.h: extern std::string ns3::TypeNameGet() [free function]
    module.add_function('TypeNameGet', 
                        'std::string', 
                        [], 
                        template_parameters=['signed char'])
    ## type-name.h: extern std::string ns3::TypeNameGet() [free function]
    module.add_function('TypeNameGet', 
                        'std::string', 
                        [], 
                        template_parameters=['unsigned long'])
    ## type-name.h: extern std::string ns3::TypeNameGet() [free function]
    module.add_function('TypeNameGet', 
                        'std::string', 
                        [], 
                        template_parameters=['unsigned int'])
    ## type-name.h: extern std::string ns3::TypeNameGet() [free function]
    module.add_function('TypeNameGet', 
                        'std::string', 
                        [], 
                        template_parameters=['unsigned short'])
    ## type-name.h: extern std::string ns3::TypeNameGet() [free function]
    module.add_function('TypeNameGet', 
                        'std::string', 
                        [], 
                        template_parameters=['unsigned char'])
    register_functions_ns3_Config(module.get_submodule('Config'), root_module)
    register_functions_ns3_FatalImpl(module.get_submodule('FatalImpl'), root_module)
    register_functions_ns3_addressUtils(module.get_submodule('addressUtils'), root_module)
    register_functions_ns3_aodv(module.get_submodule('aodv'), root_module)
    register_functions_ns3_dot11s(module.get_submodule('dot11s'), root_module)
    register_functions_ns3_flame(module.get_submodule('flame'), root_module)
    register_functions_ns3_internal(module.get_submodule('internal'), root_module)
    register_functions_ns3_olsr(module.get_submodule('olsr'), root_module)
    return

def register_functions_ns3_Config(module, root_module):
    ## config.h: extern void ns3::Config::Connect(std::string path, ns3::CallbackBase const & cb) [free function]
    module.add_function('Connect', 
                        'void', 
                        [param('std::string', 'path'), param('ns3::CallbackBase const &', 'cb')])
    ## config.h: extern void ns3::Config::ConnectWithoutContext(std::string path, ns3::CallbackBase const & cb) [free function]
    module.add_function('ConnectWithoutContext', 
                        'void', 
                        [param('std::string', 'path'), param('ns3::CallbackBase const &', 'cb')])
    ## config.h: extern void ns3::Config::Disconnect(std::string path, ns3::CallbackBase const & cb) [free function]
    module.add_function('Disconnect', 
                        'void', 
                        [param('std::string', 'path'), param('ns3::CallbackBase const &', 'cb')])
    ## config.h: extern void ns3::Config::DisconnectWithoutContext(std::string path, ns3::CallbackBase const & cb) [free function]
    module.add_function('DisconnectWithoutContext', 
                        'void', 
                        [param('std::string', 'path'), param('ns3::CallbackBase const &', 'cb')])
    ## config.h: extern ns3::Ptr<ns3::Object> ns3::Config::GetRootNamespaceObject(uint32_t i) [free function]
    module.add_function('GetRootNamespaceObject', 
                        'ns3::Ptr< ns3::Object >', 
                        [param('uint32_t', 'i')])
    ## config.h: extern uint32_t ns3::Config::GetRootNamespaceObjectN() [free function]
    module.add_function('GetRootNamespaceObjectN', 
                        'uint32_t', 
                        [])
    ## config.h: extern ns3::Config::MatchContainer ns3::Config::LookupMatches(std::string path) [free function]
    module.add_function('LookupMatches', 
                        'ns3::Config::MatchContainer', 
                        [param('std::string', 'path')])
    ## config.h: extern void ns3::Config::RegisterRootNamespaceObject(ns3::Ptr<ns3::Object> obj) [free function]
    module.add_function('RegisterRootNamespaceObject', 
                        'void', 
                        [param('ns3::Ptr< ns3::Object >', 'obj')])
    ## config.h: extern void ns3::Config::Set(std::string path, ns3::AttributeValue const & value) [free function]
    module.add_function('Set', 
                        'void', 
                        [param('std::string', 'path'), param('ns3::AttributeValue const &', 'value')])
    ## config.h: extern void ns3::Config::SetDefault(std::string name, ns3::AttributeValue const & value) [free function]
    module.add_function('SetDefault', 
                        'void', 
                        [param('std::string', 'name'), param('ns3::AttributeValue const &', 'value')])
    ## config.h: extern bool ns3::Config::SetDefaultFailSafe(std::string name, ns3::AttributeValue const & value) [free function]
    module.add_function('SetDefaultFailSafe', 
                        'bool', 
                        [param('std::string', 'name'), param('ns3::AttributeValue const &', 'value')])
    ## config.h: extern void ns3::Config::SetGlobal(std::string name, ns3::AttributeValue const & value) [free function]
    module.add_function('SetGlobal', 
                        'void', 
                        [param('std::string', 'name'), param('ns3::AttributeValue const &', 'value')])
    ## config.h: extern bool ns3::Config::SetGlobalFailSafe(std::string name, ns3::AttributeValue const & value) [free function]
    module.add_function('SetGlobalFailSafe', 
                        'bool', 
                        [param('std::string', 'name'), param('ns3::AttributeValue const &', 'value')])
    ## config.h: extern void ns3::Config::UnregisterRootNamespaceObject(ns3::Ptr<ns3::Object> obj) [free function]
    module.add_function('UnregisterRootNamespaceObject', 
                        'void', 
                        [param('ns3::Ptr< ns3::Object >', 'obj')])
    return

def register_functions_ns3_FatalImpl(module, root_module):
    ## fatal-impl.h: extern void ns3::FatalImpl::FlushStreams() [free function]
    module.add_function('FlushStreams', 
                        'void', 
                        [])
    ## fatal-impl.h: extern void ns3::FatalImpl::RegisterStream(std::ostream * stream) [free function]
    module.add_function('RegisterStream', 
                        'void', 
                        [param('std::ostream *', 'stream')])
    ## fatal-impl.h: extern void ns3::FatalImpl::UnregisterStream(std::ostream * stream) [free function]
    module.add_function('UnregisterStream', 
                        'void', 
                        [param('std::ostream *', 'stream')])
    return

def register_functions_ns3_addressUtils(module, root_module):
    return

def register_functions_ns3_aodv(module, root_module):
    return

def register_functions_ns3_dot11s(module, root_module):
    return

def register_functions_ns3_flame(module, root_module):
    return

def register_functions_ns3_internal(module, root_module):
    ## double.h: extern ns3::Ptr<ns3::AttributeChecker const> ns3::internal::MakeDoubleChecker(double min, double max, std::string name) [free function]
    module.add_function('MakeDoubleChecker', 
                        'ns3::Ptr< ns3::AttributeChecker const >', 
                        [param('double', 'min'), param('double', 'max'), param('std::string', 'name')])
    ## integer.h: extern ns3::Ptr<ns3::AttributeChecker const> ns3::internal::MakeIntegerChecker(int64_t min, int64_t max, std::string name) [free function]
    module.add_function('MakeIntegerChecker', 
                        'ns3::Ptr< ns3::AttributeChecker const >', 
                        [param('int64_t', 'min'), param('int64_t', 'max'), param('std::string', 'name')])
    ## uinteger.h: extern ns3::Ptr<ns3::AttributeChecker const> ns3::internal::MakeUintegerChecker(uint64_t min, uint64_t max, std::string name) [free function]
    module.add_function('MakeUintegerChecker', 
                        'ns3::Ptr< ns3::AttributeChecker const >', 
                        [param('uint64_t', 'min'), param('uint64_t', 'max'), param('std::string', 'name')])
    return

def register_functions_ns3_olsr(module, root_module):
    return

