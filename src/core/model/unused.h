#ifndef UNUSED_H
#define UNUSED_H

/**
 * \file
 * \ingroup core
 * Definition of the NS_UNUSED NS_UNUSED_GLOBAL macros.
 */

/**
 * \ingroup core
 * \def NS_UNUSED()
 * Mark a local variable as unused.
 */
#ifndef NS_UNUSED
# define NS_UNUSED(x) ((void)(x))
#endif

/**
 * \ingroup core
 * \def NS_UNUSED_GLOBAL()
 * Mark a variable at file scope as unused.
 */
#ifndef NS_UNUSED_GLOBAL
#if defined(__GNUC__)
# define NS_UNUSED_GLOBAL(x) x __attribute__((unused))
#elif defined(__LCLINT__)
# define NS_UNUSED_GLOBAL(x) /*@unused@*/ x
#else
# define NS_UNUSED_GLOBAL(x) x
#endif
#endif

#endif /* UNUSED_H */
