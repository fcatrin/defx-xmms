MAKE=make
CC=gcc
CFLAGS=-c $(RPM_OPT_FLAGS) -fPIC `gtk-config --cflags`
OBJS = $(SRCS:.c=.o)
.SUFFIXES: .c .o
