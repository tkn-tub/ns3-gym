/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */

#ifndef NS3_DEPRECATED_H
#define NS3_DEPRECATED_H

/**
 * \file
 * \ingroup core
 * Definition of the NS_DEPRECATED macro.
 */

/**
 * \ingroup core
 * \def NS_DEPRECATED
 * Mark a function as deprecated.
 *
 * Users should expect deprecated features to be removed eventually.
 *
 * When deprecating a feature, please update the documentation
 * with information for users on how to update their code.
 */
#if defined(__GNUC__) && (__GNUC__ == 4) && (__GNUC_MINOR__ > 1)
#define NS_DEPRECATED __attribute__ ((deprecated))
#else
#define NS_DEPRECATED
#endif

#endif /* NS3_DEPRECATED_H */
