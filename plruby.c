
#include "postgres.h"
#include "ruby.h"

#include "catalog/pg_proc.h" // Anum_pg_proc_prosrc

#include "utils/builtins.h"  // PG_MODULE_MAGIC
#include "utils/syscache.h"  // HeapTuple, PROCOID

PG_MODULE_MAGIC;

PG_FUNCTION_INFO_V1(plruby_call_handler);
PG_FUNCTION_INFO_V1(plruby_inline_handler);
PG_FUNCTION_INFO_V1(plruby_call_validator);

Datum plruby_call_handler(PG_FUNCTION_ARGS);
Datum plruby_inline_handler(PG_FUNCTION_ARGS);
Datum plruby_call_validator(PG_FUNCTION_ARGS);

void _PG_init(void);

Datum plruby_call_handler(PG_FUNCTION_ARGS)
{
  printf("plruby_call_handler\n");

  Oid fn_oid = fcinfo->flinfo->fn_oid;

  HeapTuple procTup =
    SearchSysCache(PROCOID, ObjectIdGetDatum(fn_oid), 0, 0, 0);
  if (!HeapTupleIsValid(procTup))
    elog(ERROR, "cache lookup failed for function %u", fn_oid);

  bool isnull;
  Datum prosrc =
    SysCacheGetAttr(PROCOID, procTup, Anum_pg_proc_prosrc, &isnull);
  if (isnull)
    elog(ERROR, "null prosrc");

  //
  char *source_text = TextDatumGetCString(prosrc);
  ReleaseSysCache(procTup);
  //

  VALUE ret = rb_eval_string(source_text);
  StringValue(ret);
  return CStringGetTextDatum(RSTRING_PTR(ret));
}

Datum plruby_inline_handler(PG_FUNCTION_ARGS)
{
  printf("plruby_inline_handler\n");
  MemoryContext ctx = CurrentMemoryContext;

  InlineCodeBlock *codeblock =
    (InlineCodeBlock*) DatumGetPointer(PG_GETARG_DATUM(0));

  Assert(IsA(codeblock, InlineCodeBlock));

  //
  char *source_text = codeblock->source_text;
  PG_TRY();
  {
    elog(NOTICE, "%s", source_text);
  }
  PG_CATCH();
  {
    MemoryContextSwitchTo(ctx);
    ErrorData *edata = CopyErrorData();
    FlushErrorState();
    FreeErrorData(edata);
    return (Datum) NULL;
  }
  PG_END_TRY();
  //

  VALUE ret = rb_eval_string(source_text);
  StringValue(ret);
  return CStringGetTextDatum(RSTRING_PTR(ret));
}

Datum plruby_call_validator(PG_FUNCTION_ARGS)
{
  printf("plruby_call_validator\n");
  return (Datum) NULL;
}

void _PG_init(void)
{
  printf("_PG_init\n");
  ruby_init();
}
