CC = gcc
CFLAGS = -std=gnu99 -Wall -Wextra -Werror -pedantic
LDFLAGS = -lrt -pthread

EXEC = proj2
SRCFILES = $(EXEC).c TheSenateBusProblem.c
OBJFILES = $(SRCFILES:.c=.o)

.PHONY : all pack clean cleanall

all : $(EXEC) 

%.o : %.c 
	$(CC) $(CFLAGS) -c $< -o $@

$(EXEC) : $(OBJFILES)
	$(CC) $(CFLAGS) -o $@ $(OBJFILES) $(LDFLAGS)

pack:
	zip $(EXEC).zip *.h *.c Makefile

clean:
	rm -f *.o core *.out

cleanall: clean
	rm -f $(EXEC)
