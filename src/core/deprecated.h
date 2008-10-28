#ifndef NS3_DEPRECATED_H
#define NS3_DEPRECATED_H

#ifdef __GNUC__
#define NS_DEPRECATED __attribute__ ((deprecated))
#else
#define NS_DEPRECATED
#endif

#endif /* NS3_DEPRECATED_H */
