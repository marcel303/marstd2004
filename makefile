#
# CONFIGURATION
#

AR = ar
CC = g++

INCLUDE = -I"c:/Devel/App/Dev-Cpp/include/c++"  -I"c:/Devel/App/Dev-Cpp/include" -I"c:/Devel/App/Dev-Cpp/include/c++/mingw32"
CFLAGS = -O2 -march=i486 -ffast-math -fomit-frame-pointer -Wall -Werror -s $(INCLUDE)
SRCDIR = marstd2
OBJDIR = obj
LIBDIR = lib
TESTDIR = test

#
# FILES
#

INCLUDE += -I"include"

MARSTD_OBJS_LIST = marstd.o

MARSTD_OBJS = $(addprefix $(OBJDIR)/, $(MARSTD_OBJS_LIST))

LIB_MARSTD_LIST = libmarstd2.a

LIB_MARSTD = $(addprefix $(LIBDIR)/, $(LIB_MARSTD_LIST))

LIBS = $(LIB_MARSTD)

#
# TARGETS
#

all : $(LIBS)
	@echo Done!

$(LIB_MARSTD) : $(MARSTD_OBJS)
	$(AR) rs $(LIB_MARSTD) $(MARSTD_OBJS)

$(OBJDIR)/%.o : $(SRCDIR)/%.cpp
	$(CC) -c $< -o $@ $(CFLAGS)