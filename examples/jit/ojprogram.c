
#include "config.h"

#include <glib.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "ojprogram.h"


OJProgram *
oj_program_new (void)
{
  OJProgram *p;
  p = g_malloc0(sizeof(OJProgram));
  return p;
}

int
oj_program_add_temporary (OJProgram *program, const char *type, const char *name)
{
  int i = program->n_vars;

  program->vars[i].vartype = OJ_VAR_TYPE_TEMP;
  program->vars[i].type = oj_type_get(type);
  program->vars[i].name = strdup(name);
  program->n_vars++;

  return i;
}

int
oj_program_dup_temporary (OJProgram *program, int var, int j)
{
  int i = program->n_vars;

  program->vars[i].vartype = OJ_VAR_TYPE_TEMP;
  program->vars[i].type = program->vars[var].type;
  program->vars[i].name = g_strdup_printf("%s.dup%d",
      program->vars[var].name, j);
  program->n_vars++;

  return i;
}

int
oj_program_add_source (OJProgram *program, const char *type, const char *name)
{
  int i = program->n_vars;

  program->vars[i].vartype = OJ_VAR_TYPE_SRC;
  program->vars[i].type = oj_type_get(type);
  program->vars[i].name = strdup(name);
  program->n_vars++;

  return i;
}

int
oj_program_add_destination (OJProgram *program, const char *type, const char *name)
{
  int i = program->n_vars;

  program->vars[i].vartype = OJ_VAR_TYPE_DEST;
  program->vars[i].type = oj_type_get(type);
  program->vars[i].name = strdup(name);
  program->n_vars++;

  return i;
}

int
oj_program_add_constant (OJProgram *program, const char *type, int value, const char *name)
{
  int i = program->n_vars;

  program->vars[i].vartype = OJ_VAR_TYPE_CONST;
  program->vars[i].type = oj_type_get(type);
  program->vars[i].s16 = value;
  program->vars[i].name = strdup(name);
  program->n_vars++;

  return i;
}

int
oj_program_add_parameter (OJProgram *program, OJType *type, int value, const char *name)
{

  return 0;
}

void
oj_program_append (OJProgram *program, const char *name, int arg0,
    int arg1, int arg2)
{
  OJInstruction *insn;

  insn = program->insns + program->n_insns;

  insn->opcode = oj_opcode_find_by_name (name);
  if (!insn->opcode) {
    printf("unknown opcode: %s\n", name);
  }
  insn->args[0] = arg0;
  insn->args[1] = arg1;
  insn->args[2] = arg2;
  
  program->n_insns++;
}



OJType *types;
static int n_types;
static int n_alloc_types;

OJType *
oj_type_get (const char *name)
{
  int i;
  for(i=0;i<n_types;i++){
    if (!strcmp (types[i].name, name)) {
      return types + i;
    }
  }
  return NULL;
}

void
oj_type_register (const char *name, int size)
{
  OJType *type;

  if (n_types == n_alloc_types) {
    n_alloc_types += 100;
    types = realloc (types, sizeof(OJType) * n_alloc_types);
  }

  type = types + n_types;
  type->name = strdup (name);
  type->size = size;

  n_types++;
}



OJExecutor *
oj_executor_new (OJProgram *program)
{
  OJExecutor *ex;

  ex = g_malloc0(sizeof(OJExecutor));

  ex->program = program;

  memcpy (ex->vars, program->vars, 10*sizeof(OJVariable));

  return ex;
}

void
oj_executor_free (OJExecutor *ex)
{
  g_free (ex);
}

void
oj_executor_run (OJExecutor *ex)
{


}

void
oj_executor_set_array (OJExecutor *ex, int var, void *ptr)
{
  ex->arrays[var] = ptr;
}

void
oj_executor_set_n (OJExecutor *ex, int n)
{
  ex->n = n;
}

void
oj_executor_emulate (OJExecutor *ex)
{
  int i;
  int j;
  int k;
  OJProgram *program = ex->program;
  OJInstruction *insn;
  OJOpcode *opcode;

  printf("n %d\n", ex->n);
  printf("n_insns %d\n", program->n_insns);

  for(i=0;i<ex->n;i++){
    for(j=0;j<program->n_insns;j++){
      insn = program->insns + j;
      opcode = insn->opcode;

      printf("%d: %s\n", j, insn->opcode->name);

      /* set up args */
      for(k=0;k<opcode->n_src + opcode->n_dest;k++){
        ex->args[k] = ex->vars + insn->args[k];

        printf("setting up arg %d as var %d vartype %d\n",
            k, insn->args[k], ex->args[k]->vartype);

        if (ex->args[k]->vartype == OJ_VAR_TYPE_SRC) {
          void *ptr = ex->arrays[insn->args[k]] + 2*i;

          printf("load %p\n", ptr);
          ex->args[k]->s16 = *(int16_t *)ptr;
        }
      }

      opcode->emulate (ex, opcode->emulate_user);
      printf("emulate: %d %d %d\n", ex->args[0]->s16,
          ex->args[1]->s16, ex->args[2]->s16);

      for(k=0;k<opcode->n_src + opcode->n_dest;k++){
        if (ex->args[k]->vartype == OJ_VAR_TYPE_DEST) {
          void *ptr = ex->arrays[insn->args[k]] + 2*i;

          printf("store %p\n", ptr);
          *(int16_t *)ptr = ex->args[k]->s16;
        }
      }
    }
  }
}

/* rule list */

OJRuleList *
oj_rule_list_new (void)
{
  OJRuleList *rule_list;

  rule_list = malloc (sizeof(OJRuleList));
  memset (rule_list, 0, sizeof(OJRuleList));

  return rule_list;
}

void
oj_rule_list_free (OJRuleList *rule_list)
{
  free (rule_list->rules);
  free (rule_list);
}

void
oj_rule_list_register (OJRuleList *rule_list, const char *opcode_name,
    OJRuleEmitFunc emit, void *emit_user, unsigned int flags)
{
  int i;

  if (rule_list->n_rules == rule_list->n_alloc) {
    rule_list->n_alloc += 100;
    rule_list->rules = realloc (rule_list, sizeof(OJRule) * rule_list->n_alloc);
  }

  i = rule_list->n_rules;
  rule_list->rules[i].opcode = oj_opcode_find_by_name (opcode_name);
  rule_list->rules[i].emit = emit;
  rule_list->rules[i].emit_user = emit_user;
  rule_list->rules[i].flags = flags;

  rule_list->n_rules++;
}

OJRule *
oj_rule_list_get (OJRuleList *rule_list, OJOpcode *opcode)
{
  int i;
  for (i=0;i<rule_list->n_rules;i++){
    if (rule_list->rules[i].opcode == opcode) {
      return rule_list->rules + i;
    }
  }
  return NULL;
}

void
oj_program_rewrite_vars (OJProgram *program)
{
  int i;
  int j;
  int k;
  OJInstruction *insn;
  OJOpcode *opcode;
  int var;
  int actual_var;
  int alloc[8] = { 0, 1, 0, 0, 1, 1, 0, 0 };

  for(j=0;j<program->n_insns;j++){
    insn = program->insns + j;
    opcode = insn->opcode;

    /* set up args */
    for(k=opcode->n_dest;k<opcode->n_src + opcode->n_dest;k++){
      var = insn->args[k];
      if (program->vars[var].vartype == OJ_VAR_TYPE_DEST) {
        g_print("ERROR: using dest var as source\n");
      }

      actual_var = var;
      if (program->vars[var].replaced) {
        actual_var = program->vars[var].replacement;
        insn->args[k] = actual_var;
      }

      if (!program->vars[var].used) {
        if (program->vars[var].vartype == OJ_VAR_TYPE_TEMP) {
          g_print("ERROR: using uninitialized temp var\n");
        }
        program->vars[var].used = TRUE;
        program->vars[var].first_use = j;
      }
      program->vars[actual_var].last_use = j;
    }

    for(k=0;k<opcode->n_dest;k++){
      var = insn->args[k];

      if (program->vars[var].vartype == OJ_VAR_TYPE_SRC) {
        g_print("ERROR: using src var as dest\n");
      }
      if (program->vars[var].vartype == OJ_VAR_TYPE_CONST) {
        g_print("ERROR: using const var as dest\n");
      }
      if (program->vars[var].vartype == OJ_VAR_TYPE_PARAM) {
        g_print("ERROR: using param var as dest\n");
      }

      actual_var = var;
      if (program->vars[var].replaced) {
        actual_var = program->vars[var].replacement;
        insn->args[k] = actual_var;
      }

      if (!program->vars[var].used) {
        program->vars[actual_var].used = TRUE;
        program->vars[actual_var].first_use = j;
      } else {
        if (program->vars[var].vartype == OJ_VAR_TYPE_DEST) {
          g_print("ERROR: writing dest more than once\n");
        }
        if (program->vars[var].vartype == OJ_VAR_TYPE_TEMP) {
          actual_var = oj_program_dup_temporary (program, var, j);
          program->vars[var].replaced = TRUE;
          program->vars[var].replacement = actual_var;
          insn->args[k] = actual_var;
          program->vars[actual_var].used = TRUE;
          program->vars[actual_var].first_use = j;
        }
      }
      program->vars[actual_var].last_use = j;
    }
  }

  for(j=0;j<program->n_insns;j++){
    for(i=0;i<program->n_vars;i++){
      if (program->vars[i].first_use == j) {
        for(k=0;k<8;k++){
          if (!alloc[k]) {
            program->vars[i].alloc = k + OJ_GP_REG_BASE;
            alloc[k] = 1;
            break;
          }
        }
        if (k==8) {
          g_print("register overflow\n");
        }
      }
    }
    for(i=0;i<program->n_vars;i++){
      if (program->vars[i].last_use == j) {
        alloc[program->vars[i].alloc - OJ_GP_REG_BASE] = 0;
      }
    }
  }

#if 0
  for(i=0;i<program->n_vars;i++){
    g_print("%2d: %2d %2d %s\n",
        i,
        program->vars[i].first_use,
        program->vars[i].last_use,
        x86_get_regname(program->vars[i].alloc));
  }
#endif

}

void
oj_program_dump_code (OJProgram *program)
{
  FILE *file;

  file = fopen("dump","w");

  fwrite (program->code, 1, program->codeptr - program->code, file);
  fclose (file);
}

void
oj_program_dump (OJProgram *program)
{
  int i;
  int j;
  OJOpcode *opcode;
  OJInstruction *insn;

  for(i=0;i<program->n_insns;i++){
    insn = program->insns + i;
    opcode = insn->opcode;

    g_print("insn: %d\n", i);
    g_print("  opcode: %s\n", opcode->name);

    for(j=0;j<opcode->n_dest;j++){
      g_print("  dest%d: %d %s\n", j, insn->args[j],
          program->vars[insn->args[j]].name);
    }
    for(j=0;j<opcode->n_src;j++){
      g_print("  src%d: %d %s\n", j, insn->args[opcode->n_dest + j],
          program->vars[insn->args[opcode->n_dest + j]].name);
    }

    g_print("\n");
  }

  for(i=0;i<program->n_vars;i++){
    g_print("var: %d %s\n", i, program->vars[i].name);
    g_print("first_use: %d\n", program->vars[i].first_use);
    g_print("last_use: %d\n", program->vars[i].last_use);

    g_print("\n");
  }

}

