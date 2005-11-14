/*
 * LIBOIL - Library of Optimized Inner Loops
 * Copyright (c) 2003,2004 David A. Schleef <ds@schleef.org>
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

#ifndef _LIBOIL_RANDOM_H_
#define _LIBOIL_RANDOM_H_

#include <stdlib.h>

/**
 * SECTION:liboilrandom
 * @title: Random Number Generation
 * @short_description: Random number generation
 */

/**
 * oil_rand_s32:
 *
 * Evaluates to a random integer in the range [-(1<<31), (1<<31)-1].
 */
#define oil_rand_s32() ((rand()&0xffff)<<16 | (rand()&0xffff))
/**
 * oil_rand_s32_l31:
 *
 * Evaluates to a random integer in the range [-(1<<30), (1<<30)-1].
 */
#define oil_rand_s32_l31() (((int32_t)rand())-0x40000000)

/**
 * oil_rand_s64:
 *
 * Evaluates to a random integer in the range [-(1<<63), (1<<63)-1].
 */
#define oil_rand_s64() ((int64_t)(oil_rand_s32())<<32 | oil_rand_s32())

/**
 * oil_rand_s16:
 *
 * Evaluates to a random integer in the range [-(1<<15), (1<<15)-1].
 */
#define oil_rand_s16() ((int16_t)(rand()&0xffff))
/**
 * oil_rand_s16_l15:
 *
 * Evaluates to a random integer in the range [-(1<<14), (1<<14)-1].
 */
#define oil_rand_s16_l15() (oil_rand_s16()>>1)
/**
 * oil_rand_s16_l9:
 *
 * Evaluates to a random integer in the range [-(1<<8), (1<<8)-1].
 */
#define oil_rand_s16_l9() (oil_rand_s16()>>7)
/**
 * oil_rand_s16_l8:
 *
 * Evaluates to a random integer in the range [-(1<<7), (1<<7)-1].
 */
#define oil_rand_s16_l8() (oil_rand_s16()>>8)
/**
 * oil_rand_s16_l4:
 *
 * Evaluates to a random integer in the range [-(1<<3), (1<<3)-1].
 */
#define oil_rand_s16_l4() (oil_rand_s16()>>12)

/**
 * oil_rand_s8:
 *
 * Evaluates to a random integer in the range [-(1<<7), (1<<7)-1].
 */
#define oil_rand_s8() ((int8_t)(rand()&0xffff))

/**
 * oil_rand_u32:
 *
 * Evaluates to a random integer in the range [0, (1<<32)-1].
 */
#define oil_rand_u32() ((uint32_t)((rand()&0xffff)<<16 | (rand()&0xffff)))
/**
 * oil_rand_u32_l31:
 *
 * Evaluates to a random integer in the range [0, (1<<31)-1].
 */
#define oil_rand_u32_l31() ((uint32_t)rand())

/**
 * oil_rand_u64:
 *
 * Evaluates to a random integer in the range [0, (1<<64)-1].
 */
#define oil_rand_u64() ((uint64_t)(oil_rand_u32())<<32 | oil_rand_u32())

/**
 * oil_rand_u16:
 *
 * Evaluates to a random integer in the range [0, (1<<16)-1].
 */
#define oil_rand_u16() ((uint16_t)(rand()&0xffff))

/**
 * oil_rand_u8:
 *
 * Evaluates to a random integer in the range [0, (1<<8)-1].
 */
#define oil_rand_u8() ((uint8_t)(rand()&0xffff))


/**
 * oil_rand_f64_0_1:
 *
 * Evaluates to a random double-precision floating point number
 * in the range [0, 1.0).
 */
#define oil_rand_f64_0_1() (((rand()/(RAND_MAX+1.0))+rand())/(RAND_MAX+1.0))

/**
 * oil_rand_f64_s32:
 *
 * Evaluates to a random double-precision floating point number
 * in the range [-(1<<31), (1<<31)).  Note that when rounded to the
 * nearest integer, this exceeds the range of int32_t.
 */
#define oil_rand_f64_s32() (oil_rand_f64_0_1()*4294967296.0-2147483648.0)
/**
 * oil_rand_f64_s16:
 *
 * Evaluates to a random double-precision floating point number
 * in the range [-(1<<15), (1<<15)).  Note that when rounded to the
 * nearest integer, this exceeds the range of int16_t.
 */
#define oil_rand_f64_s16() (oil_rand_f64_0_1()*65536.0-32768.0)
/**
 * oil_rand_f64_s8:
 *
 * Evaluates to a random double-precision floating point number
 * in the range [-(1<<7), (1<<7)).  Note that when rounded to the
 * nearest integer, this exceeds the range of int8_t.
 */
#define oil_rand_f64_s8() (oil_rand_f64_0_1()*256.0-128.0)

/**
 * oil_rand_f64_u32:
 *
 * Evaluates to a random double-precision floating point number
 * in the range [0, (1<<32)).  Note that when rounded to the
 * nearest integer, this exceeds the range of uint32_t.
 */
#define oil_rand_f64_u32() (oil_rand_f64_0_1()*4294967296.0)
/**
 * oil_rand_f64_u16:
 *
 * Evaluates to a random double-precision floating point number
 * in the range [0, (1<<16)).  Note that when rounded to the
 * nearest integer, this exceeds the range of uint16_t.
 */
#define oil_rand_f64_u16() (oil_rand_f64_0_1()*65536.0)
/**
 * oil_rand_f64_u8:
 *
 * Evaluates to a random double-precision floating point number
 * in the range [0, (1<<8)).  Note that when rounded to the
 * nearest integer, this exceeds the range of uint8_t.
 */
#define oil_rand_f64_u8() (oil_rand_f64_0_1()*256.0)

/**
 * oil_rand_f32_0_1:
 *
 * Evaluates to a random single-precision floating point number
 * in the range [0, 1.0).
 */
#define oil_rand_f32_0_1() (rand()/(RAND_MAX+1.0))

/**
 * oil_rand_f32_s32:
 *
 * Evaluates to a random double-precision floating point number
 * in the range [-(1<<31), (1<<31)).  Note that when rounded to the
 * nearest integer, this exceeds the range of int32_t.
 */
#define oil_rand_f32_s32() (oil_rand_f64_0_1()*4294967296.0-2147483648.0)
/**
 * oil_rand_f32_s16:
 *
 * Evaluates to a random double-precision floating point number
 * in the range [-(1<<15), (1<<15)).  Note that when rounded to the
 * nearest integer, this exceeds the range of int16_t.
 */
#define oil_rand_f32_s16() (oil_rand_f64_0_1()*65536.0-32768.0)
/**
 * oil_rand_f32_s8:
 *
 * Evaluates to a random double-precision floating point number
 * in the range [-(1<<7), (1<<7)).  Note that when rounded to the
 * nearest integer, this exceeds the range of int8_t.
 */
#define oil_rand_f32_s8() (oil_rand_f64_0_1()*256.0-128.0)

/**
 * oil_rand_f32_u32:
 *
 * Evaluates to a random double-precision floating point number
 * in the range [0, (1<<32)).  Note that when rounded to the
 * nearest integer, this exceeds the range of uint32_t.
 */
#define oil_rand_f32_u32() (oil_rand_f64_0_1()*4294967296.0)
/**
 * oil_rand_f32_u16:
 *
 * Evaluates to a random double-precision floating point number
 * in the range [0, (1<<16)).  Note that when rounded to the
 * nearest integer, this exceeds the range of uint16_t.
 */
#define oil_rand_f32_u16() (oil_rand_f64_0_1()*65536.0)
/**
 * oil_rand_f32_u8:
 *
 * Evaluates to a random double-precision floating point number
 * in the range [0, (1<<8)).  Note that when rounded to the
 * nearest integer, this exceeds the range of uint8_t.
 */
#define oil_rand_f32_u8() (oil_rand_f64_0_1()*256.0)

#endif

