/*!
\page callbacks Using ns-3 callbacks
\anchor howtos-callbacks

\section null_callbacks Null Callbacks

<b>Question:</b> The API I am using calls for using a callback (in the 
function signature), but I do not
want to provide one.  Is there a way to provide a null callback?

<b>Answer:</b> Use the ns3::MakeNullCallback construct:
\code
template<typename R>
Callback< R, T1, T2, T3, T4, T5, T6 > ns3::MakeNullCallback (void)
\endcode

Example usage:  The ns3::Socket class uses callbacks to indicate completion
of events such as a successful TCP connect().  These callbacks are set
in the following function:
\code
  void Socket::SetConnectCallback (Callback<void, Ptr<Socket> > connectionSucceeded,
                        Callback<void, Ptr<Socket> > connectionFailed,
                        Callback<void, Ptr<Socket> > halfClose);

\endcode
But suppose you do not care about registering a callback for the 
halfClose event (but you want to register one for the 
connectionSucceeded and connectionFailed cases).  In that case, you
can pass a null callback as the third argument.  You just need to
pass a callback with the matching signature, as follows:
\code
  localSocket->SetConnectCallback (
  MakeCallback (&ConnectionSucceededCallback),
  MakeCallback (&ConnectionFailedCallback),
  MakeNullCallback<void, Ptr<Socket> > () );
\endcode

*/
