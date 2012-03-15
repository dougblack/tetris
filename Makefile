################################################################################
# These are variables for the GBA toolchain build
# You can add others if you wish to
# NOTICE: Douglas Black
################################################################################

# The name of your desired GBA game
# This should be a just a name i.e MyFirstGBAGame
# No SPACES AFTER THE NAME.
PROGNAME = Tetris

# The object files you want to compile into your program
# This should be a space (SPACE!) separated list of .o files
OFILES = main.o mylib.o

# The header files you have created.
# This is necessary to determine when to recompile for files.
# This should be a space (SPACE!) separated list of .h files
HFILES = main.h mylib.h

################################################################################
# These are various settings used to make the GBA toolchain work
# DO NOT EDIT BELOW.
################################################################################

TOOLDIR  = /usr/local/cs2110-tools
CFLAGS   = -Wall -Werror -std=c99 -pedantic -g -MMD -MP
CFLAGS  += -mthumb-interwork -mlong-calls -nostartfiles
LDFLAGS = -L $(TOOLDIR)/lib \
		  -L $(TOOLDIR)/lib/gcc/arm-thumb-eabi/4.4.1/thumb \
		  -L $(TOOLDIR)/arm-thumb-eabi/lib \
		  --script $(TOOLDIR)/arm-thumb-eabi/lib/arm-gba.ld \

CC      = $(TOOLDIR)/bin/arm-thumb-eabi-gcc
AS      = $(TOOLDIR)/bin/arm-thumb-eabi-as
LD      = $(TOOLDIR)/bin/arm-thumb-eabi-ld
OBJCOPY = $(TOOLDIR)/bin/arm-thumb-eabi-objcopy
GDB     = $(TOOLDIR)/bin/arm-thumb-eabi-gdb
CFILES  = $(OFILES:.o=.c)

################################################################################
# These are the targets for the GBA build system
################################################################################

all: $(PROGNAME).gba

$(PROGNAME).gba: $(PROGNAME).elf
	@echo "[LINK] Linking files together to create $(PROGNAME).gba"
	@$(OBJCOPY) -O binary $(PROGNAME).elf $(PROGNAME).gba
	@rm -f *.d

$(PROGNAME).elf: crt0.o $(OFILES)
	@$(LD) $(LDFLAGS) -o $(PROGNAME).elf $^ -lgcc -lc -lgcc

crt0.o: $(TOOLDIR)/arm-thumb-eabi/lib/crt0.s
	@$(AS) -mthumb-interwork $^ -o crt0.o

%.o: %.c
	@echo "[COMPILE] Compiling $<"
	@$(CC) $(CFLAGS) -c $< -o $@ -I.

clean:
	@echo "[CLEAN] Removing all generated files"
	@rm -f *.o *.elf *.gba *.d $(GBADEPS)

vba: $(PROGNAME).gba
	@echo "[RUN] Running Emulator"
	@vbam $(PROGNAME).gba

rebuild: clean $(PROGNAME).gba

rebuildvba: clean vba

-include $(CFILES:%.c=%.d)
