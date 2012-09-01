#ifndef MATH_H
#define MATH_H

#include <cmath>

#ifdef __FreeBSD__

#if __FreeBSD_version <= 704101 ||				 \
  (__FreeBSD_version >= 800000 && __FreeBSD_version < 802502) || \
  (__FreeBSD_version >= 900000 && __FreeBSD_version < 900027)
#define log2(x) (std::log(x) / M_LN2)
#endif /* __FreeBSD_version */

#endif /* __FreeBSD__ */

#endif /* MATH_H */
