
\echo Use "CREATE EXTENSION plruby" to load this file. \quit

CREATE FUNCTION plruby_call_handler() RETURNS language_handler
  AS 'MODULE_PATHNAME' LANGUAGE C;

CREATE FUNCTION plruby_inline_handler(internal) RETURNS void
  AS 'MODULE_PATHNAME' LANGUAGE C;

CREATE FUNCTION plruby_call_validator(oid) RETURNS void
  AS 'MODULE_PATHNAME' LANGUAGE C;

CREATE TRUSTED LANGUAGE plruby
  HANDLER   plruby_call_handler
  INLINE    plruby_inline_handler
  VALIDATOR plruby_call_validator;
