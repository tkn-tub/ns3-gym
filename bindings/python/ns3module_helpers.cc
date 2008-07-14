#include "ns3module.h"


namespace ns3{

void PythonCompleteConstruct (Ptr<Object> object, TypeId typeId, const AttributeList &attributes)
{
  object->SetTypeId (typeId);
  object->Object::Construct (attributes);
}

}



class PythonEventImpl : public ns3::EventImpl
{
private:
    PyObject *m_callback;
    PyObject *m_args;
public:
    PythonEventImpl (PyObject *callback, PyObject *args)
    {
        m_callback = callback;
        Py_INCREF(m_callback);
        m_args = args;
        Py_INCREF(m_args);
    }
    virtual ~PythonEventImpl ()
    {
        Py_DECREF(m_callback);
        Py_DECREF(m_args);
    }
    virtual void Notify ()
    {
        PyObject *retval = PyObject_CallObject(m_callback, m_args);
        if (retval) {
            if (retval != Py_None) {
                PyErr_SetString(PyExc_TypeError, "event callback should return None");
                PyErr_Print();
            }
            Py_DECREF(retval);
        } else {
             PyErr_Print();
        }
    }
};


PyObject *
_wrap_Simulator_Schedule(PyNs3Simulator *PYBINDGEN_UNUSED(dummy), PyObject *args, PyObject *kwargs,
                         PyObject **return_exception)
{
    PyObject *exc_type, *traceback;
    PyObject *py_time;
    PyObject *py_callback;
    PyObject *user_args;
    ns3::Ptr<PythonEventImpl> py_event_impl;
    PyNs3EventId *py_EventId;

    if (kwargs && PyObject_Length(kwargs) > 0) {
        PyErr_SetString(PyExc_TypeError, "keyword arguments not supported");
        goto error;
    }

    if (PyTuple_GET_SIZE(args) < 2) {
        PyErr_SetString(PyExc_TypeError, "ns3.Simulator.Schedule needs at least 2 arguments");
        goto error;
    }
    py_time = PyTuple_GET_ITEM(args, 0);
    py_callback = PyTuple_GET_ITEM(args, 1);

    if (!PyObject_IsInstance(py_time, (PyObject*) &PyNs3Time_Type)) {
        PyErr_SetString(PyExc_TypeError, "Parameter 1 should be a ns3.Time instance");
        goto error;
    }
    if (!PyCallable_Check(py_callback)) {
        PyErr_SetString(PyExc_TypeError, "Parameter 2 should be callable");
        goto error;
    }
    user_args = PyTuple_GetSlice(args, 2, PyTuple_GET_SIZE(args));
    py_event_impl = ns3::Create<PythonEventImpl>(py_callback, user_args);
    Py_DECREF(user_args);

    py_EventId = PyObject_New(PyNs3EventId, &PyNs3EventId_Type);
    py_EventId->obj = new ns3::EventId(
       ns3::Simulator::Schedule(*((PyNs3Time *) py_time)->obj, py_event_impl));
    return (PyObject *) py_EventId;

error:
    PyErr_Fetch(&exc_type, return_exception, &traceback);
    Py_XDECREF(exc_type);
    Py_XDECREF(traceback);
    return NULL;
}


PyObject *
_wrap_Simulator_ScheduleNow(PyNs3Simulator *PYBINDGEN_UNUSED(dummy), PyObject *args, PyObject *kwargs,
                            PyObject **return_exception)
{
    PyObject *exc_type, *traceback;
    PyObject *py_callback;
    PyObject *user_args;
    ns3::Ptr<PythonEventImpl> py_event_impl;
    PyNs3EventId *py_EventId;

    if (kwargs && PyObject_Length(kwargs) > 0) {
        PyErr_SetString(PyExc_TypeError, "keyword arguments not supported");
        goto error;
    }

    if (PyTuple_GET_SIZE(args) < 1) {
        PyErr_SetString(PyExc_TypeError, "ns3.Simulator.Schedule needs at least 1 argument");
        goto error;
    }
    py_callback = PyTuple_GET_ITEM(args, 0);

    if (!PyCallable_Check(py_callback)) {
        PyErr_SetString(PyExc_TypeError, "Parameter 2 should be callable");
        goto error;
    }
    user_args = PyTuple_GetSlice(args, 1, PyTuple_GET_SIZE(args));
    py_event_impl = ns3::Create<PythonEventImpl>(py_callback, user_args);
    Py_DECREF(user_args);

    py_EventId = PyObject_New(PyNs3EventId, &PyNs3EventId_Type);
    py_EventId->obj = new ns3::EventId(ns3::Simulator::ScheduleNow(py_event_impl));
    return (PyObject *) py_EventId;

error:
    PyErr_Fetch(&exc_type, return_exception, &traceback);
    Py_XDECREF(exc_type);
    Py_XDECREF(traceback);
    return NULL;
}


PyObject *
_wrap_Simulator_ScheduleDestroy(PyNs3Simulator *PYBINDGEN_UNUSED(dummy), PyObject *args, PyObject *kwargs,
                                PyObject **return_exception)
{
    PyObject *exc_type, *traceback;
    PyObject *py_callback;
    PyObject *user_args;
    ns3::Ptr<PythonEventImpl> py_event_impl;
    PyNs3EventId *py_EventId;

    if (kwargs && PyObject_Length(kwargs) > 0) {
        PyErr_SetString(PyExc_TypeError, "keyword arguments not supported");
        goto error;
    }

    if (PyTuple_GET_SIZE(args) < 1) {
        PyErr_SetString(PyExc_TypeError, "ns3.Simulator.Schedule needs at least 1 argument");
        goto error;
    }
    py_callback = PyTuple_GET_ITEM(args, 0);

    if (!PyCallable_Check(py_callback)) {
        PyErr_SetString(PyExc_TypeError, "Parameter 2 should be callable");
        goto error;
    }
    user_args = PyTuple_GetSlice(args, 1, PyTuple_GET_SIZE(args));
    py_event_impl = ns3::Create<PythonEventImpl>(py_callback, user_args);
    Py_DECREF(user_args);

    py_EventId = PyObject_New(PyNs3EventId, &PyNs3EventId_Type);
    py_EventId->obj = new ns3::EventId(ns3::Simulator::ScheduleDestroy(py_event_impl));
    return (PyObject *) py_EventId;

error:
    PyErr_Fetch(&exc_type, return_exception, &traceback);
    Py_XDECREF(exc_type);
    Py_XDECREF(traceback);
    return NULL;
}

