CC = clang

CFLAGS = -Wall -Wno-unused-command-line-argument -fPIC -pie -ffast-math -O3
LDFLAGS = 

OBJS = easy_calc.o

TOOLNAME = easy-calc

all: $(OBJS)
	$(CC) $(CFLAGS) $(LDFLAGS) $(OBJS) -o $(TOOLNAME)

clean:
	rm -f $(TOOLNAME) $(OBJS)

SOURCES:=$(OBJS:.o=.c)
HDEADERS:=$(OBJS:.o=.h)

format:
	clang-format -sort-includes -style=file -i $(SOURCES)
	clang-format -sort-includes -style=file -i $(HDEADERS)

