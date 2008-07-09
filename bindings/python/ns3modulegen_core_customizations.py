import re

from pybindgen.typehandlers import base as typehandlers
from pybindgen import (ReturnValue, Parameter)
from pybindgen.cppmethod import CustomCppMethodWrapper, CustomCppConstructorWrapper
from pybindgen.typehandlers.codesink import MemoryCodeSink
from pybindgen.typehandlers import ctypeparser
import warnings

from pybindgen.typehandlers.base import CodeGenerationError

import sys

class SmartPointerTransformation(typehandlers.TypeTransformation):
    """
    This class provides a "type transformation" that tends to support
    NS-3 smart pointers.  Parameters such as "Ptr<Foo> foo" are
    transformed into something like Parameter.new("Foo*", "foo",
    transfer_ownership=False).  Return values such as Ptr<Foo> are
    transformed into ReturnValue.new("Foo*",
    caller_owns_return=False).  Since the underlying objects have
    reference counting, PyBindGen does the right thing.
    """
    def __init__(self):
        super(SmartPointerTransformation, self).__init__()
        self.rx = re.compile(r'(ns3::|::ns3::|)Ptr<([^>]+)>')

    def _get_untransformed_type_traits(self, name):
        m = self.rx.match(name)
        is_const = False
        if m is None:
            return None, False
        else:
            name1 = m.group(2).strip()
            if name1.startswith('const '):
                name1 = name1[len('const '):]
                is_const = True
            if name1.endswith(' const'):
                name1 = name1[:-len(' const')]
                is_const = True
            new_name = name1+' *'

            if new_name.startswith('::'):
                new_name = new_name[2:]
            return new_name, is_const

    def get_untransformed_name(self, name):
        new_name, dummy_is_const = self._get_untransformed_type_traits(name)
        return new_name

    def create_type_handler(self, type_handler, *args, **kwargs):
        if issubclass(type_handler, Parameter):
            kwargs['transfer_ownership'] = False
        elif issubclass(type_handler, ReturnValue):
            kwargs['caller_owns_return'] = False
        else:
            raise AssertionError

        ## fix the ctype, add ns3:: namespace
        orig_ctype, is_const = self._get_untransformed_type_traits(args[0])
        if is_const:
            kwargs['is_const'] = True
            correct_ctype = 'ns3::Ptr< %s const >' % orig_ctype[:-2]
        else:
            correct_ctype = 'ns3::Ptr< %s >' % orig_ctype[:-2]
        args = tuple([correct_ctype] + list(args[1:]))

        handler = type_handler(*args, **kwargs)
        handler.set_tranformation(self, orig_ctype)
        return handler

    def untransform(self, type_handler, declarations, code_block, expression):
        return 'ns3::PeekPointer (%s)' % (expression,)

    def transform(self, type_handler, declarations, code_block, expression):
        assert type_handler.untransformed_ctype[-1] == '*'
        return 'ns3::Ptr< %s > (%s)' % (type_handler.untransformed_ctype[:-1], expression)

## register the type transformation
transf = SmartPointerTransformation()
typehandlers.return_type_matcher.register_transformation(transf)
typehandlers.param_type_matcher.register_transformation(transf)
del transf


class ArgvParam(Parameter):
    """
    Converts a python list-of-strings argument to a pair of 'int argc,
    char *argv[]' arguments to pass into C.

    One Python argument becomes two C function arguments -> it's a miracle!

    Note: this parameter type handler is not registered by any name;
    must be used explicitly.
    """

    DIRECTIONS = [Parameter.DIRECTION_IN]
    CTYPES = []
    
    def convert_c_to_python(self, wrapper):
        raise NotImplementedError

    def convert_python_to_c(self, wrapper):
        py_name = wrapper.declarations.declare_variable('PyObject*', 'py_' + self.name)
        argc_var = wrapper.declarations.declare_variable('int', 'argc')
        name = wrapper.declarations.declare_variable('char**', self.name)
        idx = wrapper.declarations.declare_variable('Py_ssize_t', 'idx')
        wrapper.parse_params.add_parameter('O!', ['&PyList_Type', '&'+py_name], self.name)

        #wrapper.before_call.write_error_check('!PyList_Check(%s)' % py_name) # XXX

        wrapper.before_call.write_code("%s = (char **) malloc(sizeof(char*)*PyList_Size(%s));"
                                       % (name, py_name))
        wrapper.before_call.add_cleanup_code('free(%s);' % name)
        wrapper.before_call.write_code('''
for (%(idx)s = 0; %(idx)s < PyList_Size(%(py_name)s); %(idx)s++)
{
''' % vars())
        wrapper.before_call.sink.indent()
        wrapper.before_call.write_code('''
PyObject *item = PyList_GET_ITEM(%(py_name)s, %(idx)s);
''' % vars())
        #wrapper.before_call.write_error_check('item == NULL')
        wrapper.before_call.write_error_check(
            '!PyString_Check(item)',
            failure_cleanup=('PyErr_SetString(PyExc_TypeError, '
                             '"argument %s must be a list of strings");') % self.name)
        wrapper.before_call.write_code(
            '%s[%s] = PyString_AsString(item);' % (name, idx))
        wrapper.before_call.sink.unindent()
        wrapper.before_call.write_code('}')
        wrapper.before_call.write_code('%s = PyList_Size(%s);' % (argc_var, py_name))
        
        wrapper.call_params.append(argc_var)
        wrapper.call_params.append(name)


class CallbackImplProxyMethod(typehandlers.ReverseWrapperBase):
    """
    Class that generates a proxy virtual method that calls a similarly named python method.
    """

    def __init__(self, return_value, parameters):
        super(CallbackImplProxyMethod, self).__init__(return_value, parameters)

    def generate_python_call(self):
        """code to call the python method"""
        build_params = self.build_params.get_parameters(force_tuple_creation=True)
        if build_params[0][0] == '"':
            build_params[0] = '(char *) ' + build_params[0]
        args = self.before_call.declare_variable('PyObject*', 'args')
        self.before_call.write_code('%s = Py_BuildValue(%s);'
                                    % (args, ', '.join(build_params)))
        self.before_call.add_cleanup_code('Py_DECREF(%s);' % args)
        self.before_call.write_code('py_retval = PyObject_CallObject(m_callback, %s);' % args)
        self.before_call.write_error_check('py_retval == NULL')
        self.before_call.add_cleanup_code('Py_DECREF(py_retval);')




def generate_callback_classes(out, callbacks):
    for callback_impl_num, template_parameters in enumerate(callbacks):
        sink = MemoryCodeSink()
        cls_name = "ns3::Callback< %s >" % ', '.join(template_parameters)
        #print >> sys.stderr, "***** trying to register callback: %r" % cls_name
        class_name = "PythonCallbackImpl%i" % callback_impl_num
        sink.writeln('''
class %s : public ns3::CallbackImpl<%s>
{
public:
    PyObject *m_callback;
    %s(PyObject *callback)
    {
        Py_INCREF(callback);
        m_callback = callback;
    }
    virtual ~%s()
    {
        Py_DECREF(m_callback);
        m_callback = NULL;
    }

    virtual bool IsEqual(ns3::Ptr<const ns3::CallbackImplBase> other_base) const
    {
        const %s *other = dynamic_cast<const %s*> (ns3::PeekPointer (other_base));
        if (other != NULL)
            return (other->m_callback == m_callback);
        else
            return false;
    }

''' % (class_name, ', '.join(template_parameters), class_name, class_name, class_name, class_name))
        sink.indent()
        callback_return = template_parameters[0]
        return_ctype = ctypeparser.parse_type(callback_return)
        if ('const' in return_ctype.remove_modifiers()):
            kwargs = {'is_const': True}
        else:
            kwargs = {}
        try:
            return_type = ReturnValue.new(str(return_ctype), **kwargs)
        except (typehandlers.TypeLookupError, typehandlers.TypeConfigurationError), ex:
            warnings.warn("***** Unable to register callback; Return value '%s' error (used in %s): %r"
                          % (callback_return, cls_name, ex),
                          Warning)
            continue

        arguments = []
        ok = True
        callback_parameters = [arg for arg in template_parameters[1:] if arg != 'ns3::empty']
        for arg_num, arg_type in enumerate(callback_parameters):
            arg_name = 'arg%i' % (arg_num+1)

            param_ctype = ctypeparser.parse_type(arg_type)
            if ('const' in param_ctype.remove_modifiers()):
                kwargs = {'is_const': True}
            else:
                kwargs = {}
            try:
                arguments.append(Parameter.new(str(param_ctype), arg_name, **kwargs))
            except (typehandlers.TypeLookupError, typehandlers.TypeConfigurationError), ex:
                warnings.warn("***** Unable to register callback; parameter '%s %s' error (used in %s): %r"
                              % (arg_type, arg_name, cls_name, ex),
                              Warning)
                ok = False
        if not ok:
            continue

        wrapper = CallbackImplProxyMethod(return_type, arguments)
        wrapper.generate(sink, 'operator()', decl_modifiers=[])
            
        sink.unindent()
        sink.writeln('};\n')
        sink.flush_to(out)
        
        class PythonCallbackParameter(Parameter):
            "Class handlers"
            CTYPES = [cls_name]
            #print >> sys.stderr, "***** registering callback handler: %r" % ctypeparser.normalize_type_string(cls_name)
            DIRECTIONS = [Parameter.DIRECTION_IN]
            PYTHON_CALLBACK_IMPL_NAME = class_name
            TEMPLATE_ARGS = template_parameters

            def convert_python_to_c(self, wrapper):
                "parses python args to get C++ value"
                assert isinstance(wrapper, typehandlers.ForwardWrapperBase)

                py_callback = wrapper.declarations.declare_variable('PyObject*', self.name)
                wrapper.parse_params.add_parameter('O', ['&'+py_callback], self.name)
                wrapper.before_call.write_error_check(
                    '!PyCallable_Check(%s)' % py_callback,
                    'PyErr_SetString(PyExc_TypeError, "parameter \'%s\' must be callbale");' % self.name)
                callback_impl = wrapper.declarations.declare_variable(
                    'ns3::Ptr<%s>' % self.PYTHON_CALLBACK_IMPL_NAME,
                    '%s_cb_impl' % self.name)
                wrapper.before_call.write_code("%s = ns3::Create<%s> (%s);"
                                               % (callback_impl, self.PYTHON_CALLBACK_IMPL_NAME, py_callback))
                wrapper.call_params.append(
                    'ns3::Callback<%s> (%s)' % (', '.join(self.TEMPLATE_ARGS), callback_impl))

            def convert_c_to_python(self, wrapper):
                raise typehandlers.NotSupportedError("Reverse wrappers for ns3::Callback<...> types "
                                                     "(python using callbacks defined in C++) not implemented.")


# def write_preamble(out):
#     pybindgen.write_preamble(out)
#     out.writeln("#include \"ns3/everything.h\"")



def Simulator_customizations(module):
    Simulator = module['ns3::Simulator']

    ## Simulator::Schedule(delay, callback, ...user..args...)
    Simulator.add_custom_method_wrapper("Schedule", "_wrap_Simulator_Schedule",
                                        flags=["METH_VARARGS", "METH_KEYWORDS", "METH_STATIC"])


    ## Simulator::ScheduleNow(callback, ...user..args...)
    Simulator.add_custom_method_wrapper("ScheduleNow", "_wrap_Simulator_ScheduleNow",
                                        flags=["METH_VARARGS", "METH_KEYWORDS", "METH_STATIC"])


    ## Simulator::ScheduleDestroy(callback, ...user..args...)
    Simulator.add_custom_method_wrapper("ScheduleDestroy", "_wrap_Simulator_ScheduleDestroy",
                                        flags=["METH_VARARGS", "METH_KEYWORDS", "METH_STATIC"])


def CommandLine_customizations(module):
    CommandLine = module['ns3::CommandLine']
    CommandLine.add_method('Parse', None, [ArgvParam(None, 'argv')],
                           is_static=False)


def Object_customizations(module):
    ## ---------------------------------------------------------------------
    ## Here we generate custom constructor code for all classes that
    ## derive from ns3::Object.  The custom constructors are needed in
    ## order to support kwargs only and to translate kwargs into ns3
    ## attributes, etc.
    ## ---------------------------------------------------------------------
    Object = module['ns3::Object']


    ## add a GetTypeId method to all generatd helper classes
    def helper_class_hook(helper_class):
        decl = """
static ns3::TypeId GetTypeId (void)
{
  static ns3::TypeId tid = ns3::TypeId ("%s")
    .SetParent< %s > ()
    ;
  return tid;
}"""  % (helper_class.name, helper_class.class_.full_name)

        helper_class.add_custom_method(decl)
        helper_class.add_post_generation_code(
            "NS_OBJECT_ENSURE_REGISTERED (%s);" % helper_class.name)
    Object.add_helper_class_hook(helper_class_hook)

    ## Replace all class constructors with a generic constructor based on CreateObject<T> (AttributeList)
    module.header.writeln('''
namespace ns3 {

void PythonCompleteConstruct (Ptr<Object> object, TypeId typeId, const AttributeList &attributes);

template <typename T>
Ptr<T> CreateObjectPython (PyObject *pyobj, const AttributeList &attributes)
{
  Ptr<T> p = Ptr<T> (new T (), false);
  p->set_pyobj (pyobj);
  PythonCompleteConstruct (p, T::GetTypeId (), attributes);
  return p;  
}

} // namespace ns3

''')
    
    for cls in module.classes:
        if not cls.is_subclass(Object):
            continue
        cls.constructors = [] # clear the list of constructors

        ## add our own custom constructor, if possible
        try:
            construct_name = cls.get_construct_name()
        except CodeGenerationError:
            construct_name = None

        if construct_name and not cls.helper_class:
            construct_code = '''
    ns3::Ptr< %(CONSTRUCT_NAME)s > obj = ns3::CreateObject< %(CONSTRUCT_NAME)s > (attrList);
    obj->Ref ();
    self->obj = ns3::PeekPointer (obj);
''' % dict (CONSTRUCT_NAME=construct_name)

        elif not construct_name and not cls.helper_class:
            continue

        elif not construct_name and cls.helper_class:
            construct_code = '''
    if (self->ob_type != &%(PYTYPESTRUCT)s)
    {
        ns3::Ptr< %(HELPER_CLASS_NAME)s > obj = ns3::CreateObjectPython< %(HELPER_CLASS_NAME)s > ((PyObject *)self, attrList);
        obj->Ref ();
        self->obj = ns3::PeekPointer (obj);
    } else {
        PyErr_SetString(PyExc_TypeError, "Class cannot be constructed (unless subclassed)");
        {
            PyObject *exc_type, *traceback;
            PyErr_Fetch(&exc_type, return_exception, &traceback);
            Py_XDECREF(exc_type);
            Py_XDECREF(traceback);
        }
        return -1;
    }
''' % dict (CONSTRUCT_NAME=construct_name, HELPER_CLASS_NAME=cls.helper_class.name,
            PYTYPESTRUCT=cls.pytypestruct)

        elif construct_name and cls.helper_class:
            construct_code = '''
    if (self->ob_type != &%(PYTYPESTRUCT)s)
    {
        ns3::Ptr< %(HELPER_CLASS_NAME)s > obj = ns3::CreateObjectPython< %(HELPER_CLASS_NAME)s > ((PyObject *)self, attrList);
        obj->Ref ();
        self->obj = ns3::PeekPointer (obj);
    } else {
        ns3::Ptr< %(CONSTRUCT_NAME)s > obj = ns3::CreateObject< %(CONSTRUCT_NAME)s > (attrList);
        obj->Ref ();
        self->obj = ns3::PeekPointer (obj);
    }
''' % dict (CONSTRUCT_NAME=construct_name, HELPER_CLASS_NAME=cls.helper_class.name,
            PYTYPESTRUCT=cls.pytypestruct)
        else:
            raise AssertionError

        wrapper_name = "_wrap_create_object_%s" % (cls.full_name.replace(':', '_'),) 
        constructor = '''
static int %(WRAPPER_NAME)s (%(PYSTRUCT)s *self, PyObject *args, PyObject *kwargs, PyObject **return_exception)
{
    if (PyTuple_Size(args)) {
        PyErr_SetString(PyExc_TypeError, "positional arguments not supported "
                        "for ns3.Object constructors, only keyword arguments"
                        " should be used (AttributeName=Value)");
        {
            PyObject *exc_type, *traceback;
            PyErr_Fetch(&exc_type, return_exception, &traceback);
            Py_XDECREF(exc_type);
            Py_XDECREF(traceback);
        }
        return -1;
    }
    ns3::AttributeList attrList;
    if (kwargs && KwargsToAttributeList(kwargs, %(CLASS_NAME)s::GetTypeId(), attrList)) {
        {
            PyObject *exc_type, *traceback;
            PyErr_Fetch(&exc_type, return_exception, &traceback);
            Py_XDECREF(exc_type);
            Py_XDECREF(traceback);
        }
        return -1;
    }
    %(CONSTRUCT_CODE)s
    return 0;
}
''' % dict(WRAPPER_NAME=wrapper_name, PYSTRUCT=cls.pystruct, CLASS_NAME=cls.full_name,
           CONSTRUCT_CODE=construct_code, PURE_VIRTUALS=cls.have_pure_virtual_methods)
        cls.add_constructor(CustomCppConstructorWrapper(wrapper_name, constructor))


    # Generate conversion function from PyObject* to AttributeValue
#     sink = module.body
#     sink.writeln('''
# Ptr<AttributeValue> AttributeValueFromPyObject (PyObject *obj)
# {
#     // note: needs to check for bool first, because bool is a subclass of int
#     if (PyBool_Check(obj)) {
#         return Create<BooleanValue>(PyObject_IsTrue(obj));
#     } else if (PyInt_Check(obj)) {
#         return Create<IntegerValue>(PyInt_AsLong(obj));
#     } else if (PyLong_Check(obj)) {
#         return Create<IntegerValue>(PyLong_AsLongLong(obj));
#     } else if (PyFloat_Check(obj)) {
#         return Create<DoubleValue>(PyFloat_AsDouble(obj));
#     }

# ''')
    


    ## ---------------------------------------------------------------------
    ## -------------- write the KwargsToAttributeList function -------------
    ## ---------------------------------------------------------------------
    Attribute = module['ns3::AttributeValue']
    module.after_forward_declarations.writeln(
        'int KwargsToAttributeList(PyObject *kwargs, ns3::TypeId tid, ns3::AttributeList &oAttrList);')

    module.body.writeln(
'''
int KwargsToAttributeList(PyObject *kwargs, ns3::TypeId tid, ns3::AttributeList &oAttrList)
{
    PyObject *key, *value;
    Py_ssize_t pos = 0;

    while (PyDict_Next(kwargs, &pos, &key, &value)) {
        if (!PyString_Check(key)) {
            PyErr_SetString(PyExc_TypeError, "kwargs keys must be strings");
            return -1;
        }
        if (PyObject_IsInstance(value, (PyObject*) &%s)) {
            oAttrList.SetWithTid(tid, PyString_AsString(key), *((%s *) value)->obj);''' \
    % (Attribute.pytypestruct, Attribute.pystruct))

    for conversion_source in Attribute.get_all_implicit_conversions():
        module.body.writeln('''
        } else if (PyObject_IsInstance(value, (PyObject*) &%s)) {
            oAttrList.SetWithTid(tid, PyString_AsString(key), *((%s *) value)->obj);''' \
                        % (conversion_source.pytypestruct, conversion_source.pystruct))

    possible_type_names = ", ".join([cls.name for cls in [Attribute] + Attribute.get_all_implicit_conversions()])
    module.body.writeln('''
        } else {
            PyErr_Format(PyExc_TypeError, \"parameter must an instance of one of the types (%s), not %%s\", value->ob_type->tp_name);
            return -1;
        }''' % (possible_type_names))

    module.body.writeln(
'''
    }
    return 0;
}
''')
