objects = cc_dirent.o cc_stdio.o cc_stdlib.o cc_string.o cc_type.o

LIBCCLIBC = libcc.so.1.0.0
LIBSONAME = libcc.so.1

INCDIR := $(PWD)/../include

CFLAGS += -std=gnu99 -Wall -Werror -MMD -fPIC
CFLAGS += -I$(INCDIR)

LDFLAGS := -shared

SRCS = $(wildcard *.c)
OBJS = $(SRCS:%.c=%.o)
DEPS = $(OBJS:%.o=%.d)

all: $(LIBCCLIBC)

$(LIBCCLIBC): $(objects)
	$(CC) -Wl,-soname,$(LIBSONAME) -o $@ $^ $(LDFLAGS)

-include $(DEPS)

clean:
	-@$(RM) *.so* $(objects) *.d

install:
	install -d $(DESTDIR)$(libdir)
	install -m 0644 $(LIBCCLIBC) $(DESTDIR)$(libdir)
	install -d $(DESTDIR)$(includedir)
	install -m 0644 $(INCDIR)/*  $(DESTDIR)$(includedir)

uninstall:
	-@$(RM) $(DESTDIR)$(libdir)/$(LIBCCLIBC)
	-@$(RM) -r $(DESTDIR)$(includedir)/cc_*.h

.PHONY: all clean install uninstall
