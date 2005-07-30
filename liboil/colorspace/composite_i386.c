/*
 * LIBOIL - Library of Optimized Inner Loops
 * Copyright (c) 2005 David A. Schleef <ds@schleef.org>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT,
 * INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
 * IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <liboil/liboil.h>
#include <liboil/liboilfunction.h>

OIL_DECLARE_CLASS (composite_in_argb);
OIL_DECLARE_CLASS (composite_in_argb_const_src);
OIL_DECLARE_CLASS (composite_in_argb_const_mask);
OIL_DECLARE_CLASS (composite_over_argb);
OIL_DECLARE_CLASS (composite_over_argb_const_src);
OIL_DECLARE_CLASS (composite_add_argb);
OIL_DECLARE_CLASS (composite_add_argb_const_src);
OIL_DECLARE_CLASS (composite_in_over_argb);
OIL_DECLARE_CLASS (composite_in_over_argb_const_src);
OIL_DECLARE_CLASS (composite_in_over_argb_const_mask);

#if 0
static void
composite_in_argb_mmx (uint32_t *dest, uint32_t *src, uint8_t *mask, int n)
{
  int i;

  for(i=0;i<n;i++){
    dest[i] = ARGB(
        COMPOSITE_IN(ARGB_A(src[i]), mask[i]),
        COMPOSITE_IN(ARGB_R(src[i]), mask[i]),
        COMPOSITE_IN(ARGB_G(src[i]), mask[i]),
        COMPOSITE_IN(ARGB_B(src[i]), mask[i]));
  }
}
OIL_DEFINE_IMPL_FULL (composite_in_argb_mmx, composite_in_argb);

static void
composite_in_argb_const_src_mmx (uint32_t *dest, uint32_t *src, uint8_t *mask, int n)
{
  int i;

  for(i=0;i<n;i++){
    dest[i] = ARGB(
        COMPOSITE_IN(ARGB_A(src[0]), mask[i]),
        COMPOSITE_IN(ARGB_R(src[0]), mask[i]),
        COMPOSITE_IN(ARGB_G(src[0]), mask[i]),
        COMPOSITE_IN(ARGB_B(src[0]), mask[i]));
  }
}
OIL_DEFINE_IMPL_FULL (composite_in_argb_const_src_mmx, composite_in_argb_const_src);

static void
composite_in_argb_const_mask_mmx (uint32_t *dest, uint32_t *src, uint8_t *mask, int n)
{
  int i;

  for(i=0;i<n;i++){
    dest[i] = ARGB(
        COMPOSITE_IN(ARGB_A(src[i]), mask[0]),
        COMPOSITE_IN(ARGB_R(src[i]), mask[0]),
        COMPOSITE_IN(ARGB_G(src[i]), mask[0]),
        COMPOSITE_IN(ARGB_B(src[i]), mask[0]));
  }
}
OIL_DEFINE_IMPL_FULL (composite_in_argb_const_mask_mmx, composite_in_argb_const_mask);
#endif

static void
composite_over_argb_mmx (uint32_t *dest, uint32_t *src, int n)
{
  __asm__ __volatile__ ("  pxor %%mm7, %%mm7\n"   // mm7 = { 0, 0, 0, 0 }
      "  movl $0x80808080, %%eax\n"
      "  movd %%eax, %%mm6\n"  // mm6 = { 128, 128, 128, 128 }
      "  punpcklbw %%mm7, %%mm6\n"
      "  movl $0xffffffff, %%eax\n" // mm5 = { 255, 255, 255, 255 }
      "  movd %%eax, %%mm5\n"
      "  punpcklbw %%mm7, %%mm5\n"
      "1:\n"
      "  movl (%1), %%eax\n"
      "  testl $0xff000000, %%eax\n"
      "  jz 2f\n"

      "  movd %%eax, %%mm1\n"
      "  punpcklbw %%mm7, %%mm1\n"
      "  pshufw $0xff, %%mm1, %%mm0\n"
      "  pxor %%mm5, %%mm0\n"

      "  movd (%0), %%mm3\n"
      "  punpcklbw %%mm7, %%mm3\n"
      "  pmullw %%mm0, %%mm3\n"
      "  paddw %%mm6, %%mm3\n"
      "  movq %%mm3, %%mm2\n"
      "  psrlw $8, %%mm2\n"
      "  paddw %%mm2, %%mm3\n"
      "  psrlw $8, %%mm3\n"

      "  paddw %%mm1, %%mm3\n"
      "  packuswb %%mm3, %%mm3\n"

      "  movd %%mm3, %%eax\n"
      "  movl %%eax, (%0)\n"
      "2:\n"
      "  addl $4, %0\n"
      "  addl $4, %1\n"
      "  decl %2\n"
      "  jnz 1b\n"
      "  emms\n"
      :"+r" (dest), "+r" (src), "+r" (n)
      :
      :"eax");

}
OIL_DEFINE_IMPL_FULL (composite_over_argb_mmx, composite_over_argb, OIL_IMPL_FLAG_MMX | OIL_IMPL_FLAG_MMXEXT);

static void
composite_over_argb_sse2 (uint32_t *dest, uint32_t *src, int n)
{
  __asm__ __volatile__ ("  pxor %%xmm7, %%xmm7\n"   // mm7 = { 0, 0, 0, 0 }
      "  movl $0x80808080, %%eax\n"
      "  movd %%eax, %%xmm6\n"  // mm6 = { 128, 128, 128, 128 }
      "  punpcklbw %%xmm7, %%xmm6\n"
      "  movl $0xffffffff, %%eax\n" // mm5 = { 255, 255, 255, 255 }
      "  movd %%eax, %%xmm5\n"
      "  punpcklbw %%xmm7, %%xmm5\n"
      "1:\n"
      "  movl (%1), %%eax\n"
      "  testl $0xff000000, %%eax\n"
      "  jz 2f\n"

      "  movd %%eax, %%xmm1\n"
      "  punpcklbw %%xmm7, %%xmm1\n"
      "  pshuflw $0xff, %%xmm1, %%xmm0\n"
#if 1
      "  pxor %%xmm5, %%xmm0\n"

      "  movd (%0), %%xmm3\n"
      "  punpcklbw %%xmm7, %%xmm3\n"
      "  pmullw %%xmm0, %%xmm3\n"
      "  paddw %%xmm6, %%xmm3\n"
      "  movq %%xmm3, %%xmm2\n"
      "  psrlw $8, %%xmm2\n"
      "  paddw %%xmm2, %%xmm3\n"
      "  psrlw $8, %%xmm3\n"

      "  paddw %%xmm1, %%xmm3\n"
      "  packuswb %%xmm3, %%xmm3\n"
      "  movd %%xmm3, %%eax\n"
#else
      "  packuswb %%xmm1, %%xmm1\n"
      "  movd %%xmm1, %%eax\n"
#endif

      "  movl %%eax, (%0)\n"
      "2:\n"
      "  addl $4, %0\n"
      "  addl $4, %1\n"
      "  decl %2\n"
      "  jnz 1b\n"
      :"+r" (dest), "+r" (src), "+r" (n)
      :
      :"eax");

}
OIL_DEFINE_IMPL_FULL (composite_over_argb_sse2, composite_over_argb, OIL_IMPL_FLAG_SSE2);

#if 0
static void
composite_over_argb_const_src_mmx (uint32_t *dest, uint32_t *src, int n)
{
  int i;
  uint8_t a;

  a = ARGB_A(src[0]);
  for(i=0;i<n;i++){
    dest[i] = ARGB(
        COMPOSITE_OVER(ARGB_A(dest[i]),ARGB_A(src[0]),a),
        COMPOSITE_OVER(ARGB_R(dest[i]),ARGB_R(src[0]),a),
        COMPOSITE_OVER(ARGB_G(dest[i]),ARGB_G(src[0]),a),
        COMPOSITE_OVER(ARGB_B(dest[i]),ARGB_B(src[0]),a));
  }

}
OIL_DEFINE_IMPL_FULL (composite_over_argb_const_src_mmx, composite_over_argb_const_src);

static void
composite_add_argb_mmx (uint32_t *dest, uint32_t *src, int n)
{
  int i;

  for(i=0;i<n;i++){
    dest[i] = ARGB(
        COMPOSITE_ADD(ARGB_A(dest[i]),ARGB_A(src[i])),
        COMPOSITE_ADD(ARGB_R(dest[i]),ARGB_R(src[i])),
        COMPOSITE_ADD(ARGB_G(dest[i]),ARGB_G(src[i])),
        COMPOSITE_ADD(ARGB_B(dest[i]),ARGB_B(src[i])));
  }

}
OIL_DEFINE_IMPL_FULL (composite_add_argb_mmx, composite_add_argb);

static void
composite_add_argb_const_src_mmx (uint32_t *dest, uint32_t *src, int n)
{
  int i;

  for(i=0;i<n;i++){
    dest[i] = ARGB(
        COMPOSITE_ADD(ARGB_A(dest[i]),ARGB_A(src[0])),
        COMPOSITE_ADD(ARGB_R(dest[i]),ARGB_R(src[0])),
        COMPOSITE_ADD(ARGB_G(dest[i]),ARGB_G(src[0])),
        COMPOSITE_ADD(ARGB_B(dest[i]),ARGB_B(src[0])));
  }

}
OIL_DEFINE_IMPL_FULL (composite_add_argb_const_src_mmx, composite_add_argb_const_src);

static void
composite_in_over_argb_mmx (uint32_t *dest, uint32_t *src, uint8_t *mask, int n)
{
  int i;
  uint8_t a;
  uint32_t color;

  for(i=0;i<n;i++){
    color = ARGB(
        COMPOSITE_IN(ARGB_A(src[i]), mask[i]),
        COMPOSITE_IN(ARGB_R(src[i]), mask[i]),
        COMPOSITE_IN(ARGB_G(src[i]), mask[i]),
        COMPOSITE_IN(ARGB_B(src[i]), mask[i]));
    a = ARGB_A(color);
    dest[i] = ARGB(
        COMPOSITE_OVER(ARGB_A(dest[i]),ARGB_A(color),a),
        COMPOSITE_OVER(ARGB_R(dest[i]),ARGB_R(color),a),
        COMPOSITE_OVER(ARGB_G(dest[i]),ARGB_G(color),a),
        COMPOSITE_OVER(ARGB_B(dest[i]),ARGB_B(color),a));
  }

}
OIL_DEFINE_IMPL_FULL (composite_in_over_argb_mmx, composite_in_over_argb);

static void
composite_in_over_argb_const_src_mmx (uint32_t *dest, uint32_t *src, uint8_t *mask, int n)
{
  int i;
  uint8_t a;
  uint32_t color;

  for(i=0;i<n;i++){
    color = ARGB(
        COMPOSITE_IN(ARGB_A(src[0]), mask[i]),
        COMPOSITE_IN(ARGB_R(src[0]), mask[i]),
        COMPOSITE_IN(ARGB_G(src[0]), mask[i]),
        COMPOSITE_IN(ARGB_B(src[0]), mask[i]));
    a = ARGB_A(color);
    dest[i] = ARGB(
        COMPOSITE_OVER(ARGB_A(dest[i]),ARGB_A(color),a),
        COMPOSITE_OVER(ARGB_R(dest[i]),ARGB_R(color),a),
        COMPOSITE_OVER(ARGB_G(dest[i]),ARGB_G(color),a),
        COMPOSITE_OVER(ARGB_B(dest[i]),ARGB_B(color),a));
  }

}
OIL_DEFINE_IMPL_FULL (composite_in_over_argb_const_src_mmx, composite_in_over_argb_const_src);

static void
composite_in_over_argb_const_mask_mmx (uint32_t *dest, uint32_t *src, uint8_t *mask, int n)
{
  int i;
  uint8_t a;
  uint32_t color;

  for(i=0;i<n;i++){
    color = ARGB(
        COMPOSITE_IN(ARGB_A(src[i]), mask[0]),
        COMPOSITE_IN(ARGB_R(src[i]), mask[0]),
        COMPOSITE_IN(ARGB_G(src[i]), mask[0]),
        COMPOSITE_IN(ARGB_B(src[i]), mask[0]));
    a = ARGB_A(color);
    dest[i] = ARGB(
        COMPOSITE_OVER(ARGB_A(dest[i]),ARGB_A(color),a),
        COMPOSITE_OVER(ARGB_R(dest[i]),ARGB_R(color),a),
        COMPOSITE_OVER(ARGB_G(dest[i]),ARGB_G(color),a),
        COMPOSITE_OVER(ARGB_B(dest[i]),ARGB_B(color),a));
  }

}
OIL_DEFINE_IMPL_FULL (composite_in_over_argb_const_mask_mmx, composite_in_over_argb_const_mask);

#endif
