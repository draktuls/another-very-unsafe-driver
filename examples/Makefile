SRCS = $(wildcard *.c)
OBJS = $(SRCS:.c=.o)

name := main

all: $(name)

$(name): $(OBJS)
	$(CC) -static -o $@ $^

%.o: %.c ../utils.c
	$(CC) $(CFLAGS) -c $< -o $@

clean: 
	$(RM) -f $(OBJS)
	$(RM) $(name)
