#ifndef NS3_DEPRECATED_H
#define NS3_DEPRECATED_H

#if defined(__GNUC__) && (__GNUC__ == 4) && (__GNUC_MINOR__ > 1)
#define NS_DEPRECATED __attribute__ ((deprecated))
#else
#define NS_DEPRECATED
#endif

#endif /* NS3_DEPRECATED_H */
