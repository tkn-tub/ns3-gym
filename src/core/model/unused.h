/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2012 Andrey Mazo
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation;
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * Author: Andrey Mazo <ahippo@yandex.com>
 */

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
