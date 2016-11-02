PRG=gnu.exe
GCC=gcc
GCCFLAGS=-Werror -Wall -Wextra -ansi -pedantic -pthread -O

OBJECTS0=matrix-thread.c
DRIVER0=main-thread.c

VALGRIND_OPTIONS=-q --leak-check=full
DIFF_OPTIONS=-y --strip-trailing-cr --suppress-common-lines -b

OSTYPE := $(shell uname)
ifeq ($(OSTYPE),Linux)
CYGWIN=
else
CYGWIN=-Wl,--enable-auto-import
endif

gcc0:
	$(GCC) -o $(PRG) $(CYGWIN) $(DRIVER0) $(OBJECTS0) $(GCCFLAGS)
0:
	#echo "running test$@"
	#@echo "should run in less than 200 ms"
	@echo "should run in less than 200 ms"
	./$(PRG) input.txt >studentout$@
	#diff output$@.txt studentout$@ $(DIFF_OPTIONS) > difference$@
clean : 
	rm *.exe student* difference*
