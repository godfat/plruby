CREATE OR REPLACE FUNCTION plruby_call()
RETURNS text AS $$
  p('CALL')
$$ LANGUAGE plruby IMMUTABLE STRICT;
SELECT plruby_call();
