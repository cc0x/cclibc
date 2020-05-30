objects = vector.o

INCDIR := $(PWD)/../include

CFLAGS += -std=gnu99 -Wall
CFLAGS += -I$(INCDIR)

LDFLAGS := -L. -l:libcc.so

SRCS = $(wildcard *.c)
OBJS = $(SRCS:%.c=%.o)

all: vector_sample

vector_sample: $(objects)
	ln -sf ../src/libcc.so.1.0.0 libcc.so
	$(CC) -o $@ $^ $(LDFLAGS)

-include $(DEPS)

clean:
	-@$(RM) *.so* $(objects) *.d

.PHONY: all clean install uninstall
