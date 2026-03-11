#
# Makefile
#

# set the executable name
EXEC=app

CC=gcc
CFLAGS+= -std=c99 -Wall -g
CFLAGS+= -Iinclude
LDLIBS:= -lm

ODIR:=obj

# Selection des sources avec répertoire
#SRC := $(wildcard *.c)
#SRC += $(wildcard test/*.c)

# Tout les sous-repertoire
SRC := $(wildcard *.c) $(wildcard **/*.c)


OBJS = $(patsubst %,$(ODIR)/%,$(SRC:.c=.o))

all: $(EXEC)

-include $(ODIR)/*.d

$(EXEC): $(OBJS)
	$(CC) -o $@ $^ $(LDFLAGS) $(LDLIBS)

$(ODIR)/%.o: %.c | $(ODIR)
	mkdir -p $(@D)
	$(CC) $(CFLAGS) -c $< -o $@ -MMD -MF $(@:.o=.d)

$(ODIR):
	mkdir $@
	$(warning A mkdir)

clean:
	$(RM) $(EXEC)
	$(RM) -rf $(ODIR)

test: $(EXEC)
	baygon -v -t test.json ./$(EXEC)

testf: $(EXEC)
	baygon -v -t testf.json ./$(EXEC)

format: main.c
	find -name '*.c' -o -name '*.h' | xargs -n1 clang-format -i -style=file

.PHONY:	clean all test
.DEFAULT: all
