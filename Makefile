# John Thoms, gvr812, 11357558
# Dalton Beauchesne,nqr19,11353160
CC = gcc
CFLAGS = -g
CPPFLAGS = -std=gnu90 -Wall -pedantic -Wextra

# for part c cross compilation
MACHINE = $(shell uname -n)
ARCH = $(shell uname -m)
ifeq ($(MACHINE), cmpt332-amd64)
BINARY = s-chat-x86 s-chat-arm s-chat-ppc
else ifeq ($(ARCH), x86_64)
BINARY = reader_writer_test s-chat-x86
endif

RTT_LIB_X86 =/student/cmpt332/rtt/lib/Linuxx86_64

CROSS_COMPILE_ARM = arm-linux-gnueabihf-
RTT_LIB_ARM=/student/cmpt332/rtt/lib/Linuxarmv7l

CROSS_COMPILE_PPC = powerpc-linux-gnu-
RTT_LIB_PPC=/student/cmpt332/rtt/lib/Linuxppc


UBC_HEADER_DIR=/student/cmpt332/pthreads/
UBC_LIB=/student/cmpt332/pthreads/lib/Linuxx86_64

RTT_HEADER_DIR=/student/cmpt332/rtt/include
#RTT_LIB=/student/cmpt332/rtt/lib/Linuxx86_64
RTT_DEP=/usr/include/tirpc
#libRtt.a libRttUtils.a

# Platform specific directories
X86_OBJ=build/obj/x86_64
X86_LIB=build/lib/x86_64
X86_BIN=build/bin/x86_64

ARM_OBJ=build/obj/arm
ARM_LIB=build/lib/arm
ARM_BIN=build/bin/arm

PPC_OBJ=build/obj/ppc
PPC_LIB=build/lib/ppc
PPC_BIN=build/bin/ppc


X86_LIBLIST_OBJECTS= $(X86_OBJ)/list_util.o $(X86_OBJ)/list_adders.o \
		$(X86_OBJ)/list_movers.o $(X86_OBJ)/list_removers.o

ARM_LIBLIST_OBJECTS= $(ARM_OBJ)/list_util.o $(ARM_OBJ)/list_adders.o \
		$(ARM_OBJ)/list_movers.o $(ARM_OBJ)/list_removers.o

PPC_LIBLIST_OBJECTS= $(PPC_OBJ)/list_util.o $(PPC_OBJ)/list_adders.o \
		$(PPC_OBJ)/list_movers.o $(PPC_OBJ)/list_removers.o
# When make is run, it will only compile the executable for the
# current platform
all: $(BINARY)

clean:
	rm -rf build s-chat-x86 s-chat-arm s-chat-ppc reader_writer_test

# directories
$(X86_OBJ):
	mkdir -p $(X86_OBJ)
$(X86_LIB):
	mkdir -p $(X86_LIB)
$(X86_BIN):
	mkdir -p $(X86_BIN)


$(ARM_OBJ):
	mkdir -p $(ARM_OBJ)
$(ARM_LIB):
	mkdir -p $(ARM_LIB)
$(ARM_BIN):
	mkdir -p $(ARM_BIN)


$(PPC_OBJ):
	mkdir -p $(PPC_OBJ)
$(PPC_LIB):
	mkdir -p $(PPC_LIB)
$(PPC_BIN):
	mkdir -p $(PPC_BIN)
## LIBLIST #################################################################

# object files
# X86
$(X86_OBJ)/list_util.o : list_util.c | $(X86_OBJ)
	$(CC) -c $(CFLAGS) $(CPPFLAGS) list_util.c -o \
	$(X86_OBJ)/list_util.o -I.

$(X86_OBJ)/list_adders.o : list_adders.c | $(X86_OBJ)
	$(CC) -c $(CFLAGS) $(CPPFLAGS) list_adders.c \
	-o $(X86_OBJ)/list_adders.o -I.

$(X86_OBJ)/list_movers.o : list_movers.c | $(X86_OBJ)
	$(CC) -c $(CFLAGS) $(CPPFLAGS) list_movers.c -o \
	$(X86_OBJ)/list_movers.o -I.

$(X86_OBJ)/list_removers.o : list_removers.c | $(X86_OBJ)
	$(CC) -c $(CFLAGS) $(CPPFLAGS) list_removers.c -o \
	$(X86_OBJ)/list_removers.o -I.

# ARM
$(ARM_OBJ)/list_util.o : list_util.c | $(ARM_OBJ)
	$(CROSS_COMPILE_ARM)$(CC) -c $(CFLAGS) $(CPPFLAGS) list_util.c -o \
	$(ARM_OBJ)/list_util.o -I.

$(ARM_OBJ)/list_adders.o : list_adders.c | $(ARM_OBJ)
	$(CROSS_COMPILE_ARM)$(CC) -c $(CFLAGS) $(CPPFLAGS) list_adders.c \
	-o $(ARM_OBJ)/list_adders.o -I.

$(ARM_OBJ)/list_movers.o : list_movers.c | $(ARM_OBJ)
	$(CROSS_COMPILE_ARM)$(CC) -c $(CFLAGS) $(CPPFLAGS) \
	list_movers.c -o $(ARM_OBJ)/list_movers.o -I.

$(ARM_OBJ)/list_removers.o : list_removers.c | $(ARM_OBJ)
	$(CROSS_COMPILE_ARM)$(CC) -c $(CFLAGS) $(CPPFLAGS) \
	list_removers.c -o $(ARM_OBJ)/list_removers.o -I.


# PPC
$(PPC_OBJ)/list_util.o : list_util.c | $(PPC_OBJ)
	$(CROSS_COMPILE_PPC)$(CC) -c $(CFLAGS) $(CPPFLAGS) list_util.c -o \
	$(PPC_OBJ)/list_util.o -I.

$(PPC_OBJ)/list_adders.o : list_adders.c | $(PPC_OBJ)
	$(CROSS_COMPILE_PPC)$(CC) -c $(CFLAGS) $(CPPFLAGS) list_adders.c \
	-o $(PPC_OBJ)/list_adders.o -I.

$(PPC_OBJ)/list_movers.o : list_movers.c | $(PPC_OBJ)
	$(CROSS_COMPILE_PPC)$(CC) -c $(CFLAGS) $(CPPFLAGS) \
	list_movers.c -o $(PPC_OBJ)/list_movers.o -I.

$(PPC_OBJ)/list_removers.o : list_removers.c | $(PPC_OBJ)
	$(CROSS_COMPILE_PPC)$(CC) -c $(CFLAGS) $(CPPFLAGS) \
	list_removers.c -o $(PPC_OBJ)/list_removers.o -I.


# library
# X86
$(X86_LIB)/liblist.a : $(X86_LIBLIST_OBJECTS) | $(X86_LIB)
	ar -rcs $(X86_LIB)/liblist.a $(X86_LIBLIST_OBJECTS)


# ARM
$(ARM_LIB)/liblist.a : $(ARM_LIBLIST_OBJECTS) | $(ARM_LIB)
	ar -rcs $(ARM_LIB)/liblist.a $(ARM_LIBLIST_OBJECTS)

# PPC
$(PPC_LIB)/liblist.a : $(PPC_LIBLIST_OBJECTS) | $(PPC_LIB)
	ar -rcs $(PPC_LIB)/liblist.a $(PPC_LIBLIST_OBJECTS)


# LIBMONITOR ##############################################

# object files
$(X86_OBJ)/Monitor.o: Monitor.c | $(X86_OBJ)
	$(CC) -c $(CFLAGS) $(CPPFLAGS)  Monitor.c -o \
	$(X86_OBJ)/Monitor.o -I. -I$(UBC_HEADER_DIR)

# library
$(X86_LIB)/libMonitor.a : $(X86_OBJ)/Monitor.o | $(X86_LIB)
	ar -rcs $(X86_LIB)/libMonitor.a  $(X86_OBJ)/Monitor.o

## READER/WRITER ###############################################

# object file 

$(X86_OBJ)/reader_writer_monitor.o : reader_writer_monitor.c | $(X86_OBJ)
	$(CC) -c $(CFLAGS) $(CPPFLAGS) \
	reader_writer_monitor.c -o $(X86_OBJ)/reader_writer_monitor.o -I.


$(X86_OBJ)/reader_writer.o : reader_writer.c | $(X86_OBJ)
	$(CC) -c $(CFLAGS) $(CPPFLAGS)  \
	reader_writer.c -o $(X86_OBJ)/reader_writer.o -I. -I$(UBC_HEADER_DIR)


# binary
$(X86_BIN)/reader_writer_test : $(X86_OBJ)/reader_writer_monitor.o \
  $(X86_OBJ)/reader_writer.o $(X86_LIB)/libMonitor.a \
  $(X86_LIB)/liblist.a | $(X86_BIN)
	$(CC) $(CFLAGS)  \
	$(X86_OBJ)/reader_writer_monitor.o $(X86_OBJ)/reader_writer.o \
	 -o $(X86_BIN)/reader_writer_test \
	-L$(X86_LIB) -L$(UBC_LIB) -lMonitor -llist -lpthreads

reader_writer_test: $(X86_BIN)/reader_writer_test
	ln -sf $(X86_BIN)/reader_writer_test reader_writer_test

## S-CHAT (version specific) ###############################################

# object file

$(X86_OBJ)/s-chat-x86.o : s-chat.c | $(X86_OBJ)
	$(CC) -c $(CFLAGS) $(CPPFLAGS)  \
	s-chat.c -o $(X86_OBJ)/s-chat-x86.o -I. -I$(RTT_HEADER_DIR) \
	-I$(RTT_DEP)

$(ARM_OBJ)/s-chat-arm.o : s-chat.c | $(ARM_OBJ)
	$(CROSS_COMPILE_ARM)$(CC) -c $(CFLAGS) $(CPPFLAGS) \
	 s-chat.c -o $(ARM_OBJ)/s-chat-arm.o -I. -I$(RTT_HEADER_DIR) \
	-I$(RTT_DEP)

$(PPC_OBJ)/s-chat-ppc.o : s-chat.c | $(PPC_OBJ)
	$(CROSS_COMPILE_PPC)$(CC) -c $(CFLAGS) $(CPPFLAGS) \
	 s-chat.c -o $(PPC_OBJ)/s-chat-ppc.o -I. -I$(RTT_HEADER_DIR) \
	-I$(RTT_DEP)


# binaries
$(X86_BIN)/s-chat-x86 : $(X86_OBJ)/s-chat-x86.o $(X86_LIB)/liblist.a | \
$(X86_BIN)
	$(CC) $(CFLAGS)  $(X86_OBJ)/s-chat-x86.o \
	 -o $(X86_BIN)/s-chat-x86 -L$(X86_LIB) -L$(RTT_LIB_X86) -lRtt \
	-lRttUtils -ltirpc -llist

$(ARM_BIN)/s-chat-arm : $(ARM_OBJ)/s-chat-arm.o $(ARM_LIB)/liblist.a | \
$(ARM_BIN)
	$(CROSS_COMPILE_ARM)$(CC) $(CFLAGS)  \
	$(ARM_OBJ)/s-chat-arm.o  -o $(ARM_BIN)/s-chat-arm -L$(ARM_LIB) \
	-L$(RTT_LIB_ARM) -lRtt -lRttUtils -ltirpc -llist

$(PPC_BIN)/s-chat-ppc : $(PPC_OBJ)/s-chat-ppc.o $(PPC_LIB)/liblist.a | \
$(PPC_BIN)
	$(CROSS_COMPILE_PPC)$(CC) $(CFLAGS)  \
	$(PPC_OBJ)/s-chat-ppc.o  -o \
	$(PPC_BIN)/s-chat-ppc -L$(PPC_LIB) -L$(RTT_LIB_PPC) -lRtt -lRttUtils \
	-ltirpc -llist

s-chat-x86: $(X86_BIN)/s-chat-x86
	ln -sf $(X86_BIN)/s-chat-x86 s-chat-x86
s-chat-arm: $(ARM_BIN)/s-chat-arm
	ln -sf $(ARM_BIN)/s-chat-arm s-chat-arm
s-chat-ppc: $(PPC_BIN)/s-chat-ppc
	ln -sf $(PPC_BIN)/s-chat-ppc s-chat-ppc
