/* liboil - Library of Optimized Inner Loops
 * Copyright (C) 2004  David A. Schleef <ds@schleef.org>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of version 2.1 of the GNU Lesser General
 * Public License as published by the Free Software Foundation.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place, Suite 330,
 * Boston, MA 02111-1307 USA.
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <liboil/liboil.h>
#include "md5.h"

OIL_DEFINE_CLASS (md5, "uint32_t *state, uint32_t *src");

#ifdef WORDS_BIGENDIAN
#define uint32_to_host(a) \
  ((((a)&0xff)<<24)|(((a)&0xff00)<<8)|(((a)&0xff0000)>>8)|(((a)>>24)&0xff))
#else
#define uint32_to_host(a) (a)
#endif

#define F1(x, y, z) (z ^ (x & (y ^ z)))
#define F2(x, y, z) F1(z, x, y)
#define F3(x, y, z) (x ^ y ^ z)
#define F4(x, y, z) (y ^ (x | ~z))

#define MD5STEP(f,w,x,y,z,in,offset,s) \
  (w += f(x,y,z) + uint32_to_host(in) + offset, w = (w<<s | w>>(32-s)) + x)


static void
md5_c(uint32_t *state, uint32_t *src)
{
  uint32_t a,b,c,d;

  a = state[0];
  b = state[1];
  c = state[2];
  d = state[3];

  MD5STEP(F1, a, b, c, d, src[0], 0xd76aa478, 7);
  MD5STEP(F1, d, a, b, c, src[1], 0xe8c7b756, 12);
  MD5STEP(F1, c, d, a, b, src[2], 0x242070db, 17);
  MD5STEP(F1, b, c, d, a, src[3], 0xc1bdceee, 22);
  MD5STEP(F1, a, b, c, d, src[4], 0xf57c0faf, 7);
  MD5STEP(F1, d, a, b, c, src[5], 0x4787c62a, 12);
  MD5STEP(F1, c, d, a, b, src[6], 0xa8304613, 17);
  MD5STEP(F1, b, c, d, a, src[7], 0xfd469501, 22);
  MD5STEP(F1, a, b, c, d, src[8], 0x698098d8, 7);
  MD5STEP(F1, d, a, b, c, src[9], 0x8b44f7af, 12);
  MD5STEP(F1, c, d, a, b, src[10], 0xffff5bb1, 17);
  MD5STEP(F1, b, c, d, a, src[11], 0x895cd7be, 22);
  MD5STEP(F1, a, b, c, d, src[12], 0x6b901122, 7);
  MD5STEP(F1, d, a, b, c, src[13], 0xfd987193, 12);
  MD5STEP(F1, c, d, a, b, src[14], 0xa679438e, 17);
  MD5STEP(F1, b, c, d, a, src[15], 0x49b40821, 22);

  MD5STEP(F2, a, b, c, d, src[1], 0xf61e2562, 5);
  MD5STEP(F2, d, a, b, c, src[6], 0xc040b340, 9);
  MD5STEP(F2, c, d, a, b, src[11], 0x265e5a51, 14);
  MD5STEP(F2, b, c, d, a, src[0], 0xe9b6c7aa, 20);
  MD5STEP(F2, a, b, c, d, src[5], 0xd62f105d, 5);
  MD5STEP(F2, d, a, b, c, src[10], 0x02441453, 9);
  MD5STEP(F2, c, d, a, b, src[15], 0xd8a1e681, 14);
  MD5STEP(F2, b, c, d, a, src[4], 0xe7d3fbc8, 20);
  MD5STEP(F2, a, b, c, d, src[9], 0x21e1cde6, 5);
  MD5STEP(F2, d, a, b, c, src[14], 0xc33707d6, 9);
  MD5STEP(F2, c, d, a, b, src[3], 0xf4d50d87, 14);
  MD5STEP(F2, b, c, d, a, src[8], 0x455a14ed, 20);
  MD5STEP(F2, a, b, c, d, src[13], 0xa9e3e905, 5);
  MD5STEP(F2, d, a, b, c, src[2], 0xfcefa3f8, 9);
  MD5STEP(F2, c, d, a, b, src[7], 0x676f02d9, 14);
  MD5STEP(F2, b, c, d, a, src[12], 0x8d2a4c8a, 20);

  MD5STEP(F3, a, b, c, d, src[5], 0xfffa3942, 4);
  MD5STEP(F3, d, a, b, c, src[8], 0x8771f681, 11);
  MD5STEP(F3, c, d, a, b, src[11], 0x6d9d6122, 16);
  MD5STEP(F3, b, c, d, a, src[14], 0xfde5380c, 23);
  MD5STEP(F3, a, b, c, d, src[1], 0xa4beea44, 4);
  MD5STEP(F3, d, a, b, c, src[4], 0x4bdecfa9, 11);
  MD5STEP(F3, c, d, a, b, src[7], 0xf6bb4b60, 16);
  MD5STEP(F3, b, c, d, a, src[10], 0xbebfbc70, 23);
  MD5STEP(F3, a, b, c, d, src[13], 0x289b7ec6, 4);
  MD5STEP(F3, d, a, b, c, src[0], 0xeaa127fa, 11);
  MD5STEP(F3, c, d, a, b, src[3], 0xd4ef3085, 16);
  MD5STEP(F3, b, c, d, a, src[6], 0x04881d05, 23);
  MD5STEP(F3, a, b, c, d, src[9], 0xd9d4d039, 4);
  MD5STEP(F3, d, a, b, c, src[12], 0xe6db99e5, 11);
  MD5STEP(F3, c, d, a, b, src[15], 0x1fa27cf8, 16);
  MD5STEP(F3, b, c, d, a, src[2], 0xc4ac5665, 23);

  MD5STEP(F4, a, b, c, d, src[0], 0xf4292244, 6);
  MD5STEP(F4, d, a, b, c, src[7], 0x432aff97, 10);
  MD5STEP(F4, c, d, a, b, src[14], 0xab9423a7, 15);
  MD5STEP(F4, b, c, d, a, src[5], 0xfc93a039, 21);
  MD5STEP(F4, a, b, c, d, src[12], 0x655b59c3, 6);
  MD5STEP(F4, d, a, b, c, src[3], 0x8f0ccc92, 10);
  MD5STEP(F4, c, d, a, b, src[10], 0xffeff47d, 15);
  MD5STEP(F4, b, c, d, a, src[1], 0x85845dd1, 21);
  MD5STEP(F4, a, b, c, d, src[8], 0x6fa87e4f, 6);
  MD5STEP(F4, d, a, b, c, src[15], 0xfe2ce6e0, 10);
  MD5STEP(F4, c, d, a, b, src[6], 0xa3014314, 15);
  MD5STEP(F4, b, c, d, a, src[13], 0x4e0811a1, 21);
  MD5STEP(F4, a, b, c, d, src[4], 0xf7537e82, 6);
  MD5STEP(F4, d, a, b, c, src[11], 0xbd3af235, 10);
  MD5STEP(F4, c, d, a, b, src[2], 0x2ad7d2bb, 15);
  MD5STEP(F4, b, c, d, a, src[9], 0xeb86d391, 21);

  state[0] += a;
  state[1] += b;
  state[2] += c;
  state[3] += d;
}


OIL_DEFINE_IMPL_REF (md5_c, md5);

