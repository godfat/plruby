
# http://www.postgresql.org/docs/9.3/static/extend-pgxs.html

PG_CONFIG = pg_config
PGXS := $(shell $(PG_CONFIG) --pgxs)

MODULE_big = plruby
EXTENSION = plruby
DATA = plruby.control plruby--0.0.1.sql
REGRESS = init call inline

SRCS = plruby.c
OBJS = $(SRCS:.c=.o)
SHLIB_LINK = -L/usr/local/opt/ruby/lib -lruby
PG_CPPFLAGS = -Wall -O2 -I/usr/local/opt/ruby/include/ruby-2.0.0 -I/usr/local/opt/ruby/include/ruby-2.0.0/x86_64-darwin12.5.0/

include $(PGXS)
