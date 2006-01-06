/*
 * LIBOIL - Library of Optimized Inner Loops
 * Copyright (c) 2004 David A. Schleef <ds@schleef.org>
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

#include <stdio.h>
#include <liboil/liboil.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

#include <liboil/liboilprototype.h>

static char * xstrdup (const char *s);

void print_header (void);
void print_footer (void);

int main (int argc, char *argv[])
{
  OilFunctionClass *klass;
  OilPrototype *proto;
  int i;
  int n;
  char *string;

  oil_init_no_optimize ();

  print_header ();

  n = oil_class_get_n_classes ();
  for (i=0;i<n; i++ ){
    klass = oil_class_get_by_index (i);

    if(klass->prototype) {
      proto = oil_prototype_from_string (klass->prototype);
      if (proto) {
        string = oil_prototype_to_string (proto);
        if (strlen (string) == 0) {
          free (string);
          string = xstrdup("void");
        }

        printf ("extern OilFunctionClass *oil_function_class_ptr_%s;\n",
            klass->name);
        printf ("typedef void (*_oil_type_%s)(%s);\n",klass->name,string);
        printf ("#define oil_%s ((_oil_type_%s)(*(void **)oil_function_class_ptr_%s))\n",
            klass->name, klass->name, klass->name);

        oil_prototype_free (proto);
        free (string);
      } else {
        printf("/* ERROR: could not parse %s(%s) */\n", klass->name, klass->prototype);
      }
    }
  }

  print_footer ();

  return 0;
}

void print_header (void)
{
  printf ("/*\n");
  printf (" * LIBOIL - Library of Optimized Inner Loops\n");
  printf (" * Copyright (c) 2004 David A. Schleef <ds@schleef.org>\n");
  printf (" * All rights reserved.\n");
  printf (" *\n");
  printf (" * Redistribution and use in source and binary forms, with or without\n");
  printf (" * modification, are permitted provided that the following conditions\n");
  printf (" * are met:\n");
  printf (" * 1. Redistributions of source code must retain the above copyright\n");
  printf (" *    notice, this list of conditions and the following disclaimer.\n");
  printf (" * 2. Redistributions in binary form must reproduce the above copyright\n");
  printf (" *    notice, this list of conditions and the following disclaimer in the\n");
  printf (" *    documentation and/or other materials provided with the distribution.\n");
  printf (" * \n");
  printf (" * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR\n");
  printf (" * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED\n");
  printf (" * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE\n");
  printf (" * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT,\n");
  printf (" * INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES\n");
  printf (" * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR\n");
  printf (" * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)\n");
  printf (" * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,\n");
  printf (" * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING\n");
  printf (" * IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE\n");
  printf (" * POSSIBILITY OF SUCH DAMAGE.\n");
  printf (" */\n");
  printf ("\n");
  printf ("/* This file is automatically generated.  Do not edit. */\n");
  printf ("\n");
  printf ("#ifndef _LIBOIL_FUNCS_H_\n");
  printf ("#define _LIBOIL_FUNCS_H_\n");
  printf ("\n");
  printf ("#include <liboil/liboiltypes.h>\n");
  printf ("\n");
  printf ("#ifdef __cplusplus\n");
  printf ("extern \"C\" {\n");
  printf ("#endif\n");
  printf ("\n");
}

void print_footer (void)
{
  printf ("\n");
  printf ("#ifdef __cplusplus\n");
  printf ("}\n");
  printf ("#endif\n");
  printf ("\n");
  printf ("#endif\n");
  printf ("\n");
}

static char *
xstrdup (const char *s)
{
  int n = strlen(s);
  char *t;

  n = strlen(s);
  t = malloc(n + 1);
  memcpy (t, s, n);
  t[n] = 0;

  return t;
}

