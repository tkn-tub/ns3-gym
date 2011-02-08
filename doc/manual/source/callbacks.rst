.. include:: replace.txt

Callbacks
---------

Some new users to |ns3| are unfamiliar with an extensively used programming
idiom used throughout the code: the *ns-3 callback*. This chapter provides some
motivation on the callback, guidance on how to use it, and details on its
implementation.

Callbacks Motivation
********************

Consider that you have two simulation models A and B, and you wish to have them
pass information between them during the simulation. One way that you can do
that is that you can make A and B each explicitly knowledgeable about the other,
so that they can invoke methods on each other::

  class A {
  public:
    void ReceiveInput ( // parameters );
    ...
  }

  (in another source file:)

  class B {
  public:
    void DoSomething (void);
    ...

  private:
    A* a_instance; // pointer to an A
  }

  void
  B::DoSomething()
  {
    // Tell a_instance that something happened
    a_instance->ReceiveInput ( // parameters);
    ...
  }

This certainly works, but it has the drawback that it introduces a dependency on
A and B to know about the other at compile time (this makes it harder to have
independent compilation units in the simulator) and is not generalized; if in a
later usage scenario, B needs to talk to a completely different C object, the
source code for B needs to be changed to add a ``c_instance`` and so forth. It
is easy to see that this is a brute force mechanism of communication that can
lead to programming cruft in the models.  

This is not to say that objects should not know about one another if there is a
hard dependency between them, but that often the model can be made more flexible
if its interactions are less constrained at compile time.

This is not an abstract problem for network simulation research, but rather it
has been a source of problems in previous simulators, when researchers want to
extend or modify the system to do different things (as they are apt to do in
research). Consider, for example, a user who wants to add an IPsec security
protocol sublayer between TCP and IP::

  ------------                   -----------
  |   TCP    |                   |  TCP    |
  ------------                   -----------
       |           becomes ->        |
  -----------                    -----------
  |   IP    |                    | IPsec   |
  -----------                    -----------
                                     |
                                 -----------
                                 |   IP    |
                                 -----------


If the simulator has made assumptions, and hard coded into the code, that IP
always talks to a transport protocol above, the user may be forced to hack the
system to get the desired interconnections. This is clearly not an optimal way
to design a generic simulator.

Callbacks Background
********************

.. note:: Readers familiar with programming callbacks may skip this tutorial
    section.

The basic mechanism that allows one to address the problem above is known as a
*callback*. The ultimate goal is to allow one piece of code to call a function
(or method in C++) without any specific inter-module dependency.

This ultimately means you need some kind of indirection -- you treat the address
of the called function as a variable.  This variable is called a
pointer-to-function variable. The relationship between function and
pointer-to-function pointer is really no different that that of object and
pointer-to-object.

In C the canonical example of a pointer-to-function is a
pointer-to-function-returning-integer (PFI). For a PFI taking one int parameter,
this could be declared like,::

  int (*pfi)(int arg) = 0;

What you get from this is a variable named simply ``pfi`` that is initialized to
the value 0. If you want to initialize this pointer to something meaningful, you
have to have a function with a matching signature. In this case::

  int MyFunction (int arg) {}

If you have this target, you can initialize the variable to point to your
function like::

  pfi = MyFunction;

You can then call MyFunction indirectly using the more suggestive form of the
call::

  int result = (*pfi) (1234);

This is suggestive since it looks like you are dereferencing the function
pointer just like you would dereference any pointer. Typically, however, people
take advantage of the fact that the compiler knows what is going on and will
just use a shorter form::

  int result = pfi (1234);

Notice that the function pointer obeys value semantics, so you can pass it
around like any other value. Typically, when you use an asynchronous interface
you will pass some entity like this to a function which will perform an action
and *call back* to let you know it completed. It calls back by following the
indirection and executing the provided function.

In C++ you have the added complexity of objects. The analogy with the PFI above
means you have a pointer to a member function returning an int (PMI) instead of
the pointer to function returning an int (PFI).

The declaration of the variable providing the indirection looks only slightly
different::

  int (MyClass::*pmi) (int arg) = 0;

This declares a variable named ``pmi`` just as the previous example declared a
variable named ``pfi``. Since the will be to call a method of an instance of a
particular class, one must declare that method in a class::

  class MyClass {
  public:
    int MyMethod (int arg);
  };

Given this class declaration, one would then initialize that variable like
this::

  pmi = &MyClass::MyMethod;

This assigns the address of the code implementing the method to the variable,
completing the indirection. In order to call a method, the code needs a ``this``
pointer. This, in turn, means there must be an object of MyClass to refer to. A
simplistic example of this is just calling a method indirectly (think virtual
function)::

  int (MyClass::*pmi) (int arg) = 0;  // Declare a PMI
  pmi = &MyClass::MyMethod;           // Point at the implementation code

  MyClass myClass;                    // Need an instance of the class
  (myClass.*pmi) (1234);              // Call the method with an object ptr

Just like in the C example, you can use this in an asynchronous call to another
module which will *call back* using a method and an object pointer. The
straightforward extension one might consider is to pass a pointer to the object
and the PMI variable. The module would just do::

  (*objectPtr.*pmi) (1234);

to execute the callback on the desired object.

One might ask at this time, *what's the point*? The called module will have to
understand the concrete type of the calling object in order to properly make the
callback. Why not just accept this, pass the correctly typed object pointer and
do ``object->Method(1234)`` in the code instead of the callback?  This is
precisely the problem described above. What is needed is a way to decouple the
calling function from the called class completely. This requirement led to the
development of the *Functor*.

A functor is the outgrowth of something invented in the 1960s called a closure.
It is basically just a packaged-up function call, possibly with some state.  

A functor has two parts, a specific part and a generic part, related through
inheritance. The calling code (the code that executes the callback) will execute
a generic overloaded ``operator ()`` of a generic functor to cause the callback
to be called. The called code (the code that wants to be called back) will have
to provide a specialized implementation of the ``operator ()`` that performs the
class-specific work that caused the close-coupling problem above.  

With the specific functor and its overloaded ``operator ()`` created, the called
code then gives the specialized code to the module that will execute the
callback (the calling code).

The calling code will take a generic functor as a parameter, so an implicit cast
is done in the function call to convert the specific functor to a generic
functor.  This means that the calling module just needs to understand the
generic functor type. It is decoupled from the calling code completely.

The information one needs to make a specific functor is the object pointer and
the pointer-to-method address. 

The essence of what needs to happen is that the system declares a generic part
of the functor::

  template <typename T>
  class Functor
  {
  public:
    virtual int operator() (T arg) = 0;
  };

The caller defines a specific part of the functor that really is just there to 
implement the specific ``operator()`` method::

  template <typename T, typename ARG>
  class SpecificFunctor : public Functor<ARG>
  {
  public:
    SpecificFunctor(T* p, int (T::*_pmi)(ARG arg))
    {
      m_p = p;
      m_pmi = _pmi;
    }

    virtual int operator() (ARG arg)
    {
      (*m_p.*m_pmi)(arg);
    }
  private:
    int (T::*m_pmi)(ARG arg);
    T* m_p;
  };

Here is an example of the usage::

  class A
  {
  public:
  A (int ao) : a (a0) {}
  int Hello (int b0)
  {
    std::cout << "Hello from A, a = " << a << " b0 = " << b0 << std::endl;
  }
  int a;
  };

  int main()
  {
    A a(10);
    SpecificFunctor<A, int> sf(&a, &A::Hello);
    sf(5);
  }
