# Hey Emacs, this is a -*- makefile -*-
#
# WinARM template makefile 

# based on the WinAVR makefile written by Eric B. Weddington, J�rg Wunsch, et al.
# Released to the Public Domain
# Please read the make user manual!
#
#
# On command line:
#
# make all = Make software.
#
# make clean = Clean out built project files.
#
# make program = Download the hex file to the device, using isp21lpc
#
# make filename.s = Just compile filename.c into the assembler code only
#
# To rebuild project do "make clean" then "make all".
#

#TOOLCHAIN = /usr/local/arm/arm-2008q3/bin/arm-none-eabi-
TOOLCHAIN = arm-none-eabi-

# MCU name and submodel
MCU = arm7tdmi
SUBMDL = LPC2103
#THUMB    = -mthumb
#THUMB_IW = -mthumb-interwork


## Create ROM-Image (final)
RUN_MODE=ROM_RUN
## Create RAM-Image (debugging)
#RUN_MODE=RAM_RUN


# Output format. (can be srec, ihex, binary)
FORMAT = ihex


# Target file name (without extension).
TARGET = drv8402demo


# List C source files here. (C dependencies are automatically generated.)
SRC = main.c
SRC += system/system.c
SRC += system/irq.c
SRC += utils.c
SRC += uart.c
SRC += terminal.c
SRC += ecmd.c
SRC += pwm-motor.c

# List Assembler source files here.
# Make them always end in a capital .S.  Files ending in a lowercase .s
# will not be considered source files but generated files (assembler
# output from the compiler), and will be deleted upon "make clean"!
# Even though the DOS/Win* filesystem matches both .s and .S the same,
# it will preserve the spelling of the filenames, and gcc itself does
# care about how the name is spelled on its command-line.
ASRC = system/crt0.S


# Optimization level, can be [0, 1, 2, 3, s]. 
# 0 = turn off optimization. s = optimize for size.
# (Note: 3 is not always the best optimization level. See avr-libc FAQ.)
OPT = s

# Debugging format.
# Native formats for AVR-GCC's -g are stabs [default], or dwarf-2.
# AVR (extended) COFF requires stabs, plus an avr-objcopy run.
#DEBUG = stabs
DEBUG = dwarf-2

# List any extra directories to look for include files here.
#     Each directory must be seperated by a space.
EXTRAINCDIRS =

# Compiler flag to set the C Standard level.
# c89   - "ANSI" C
# gnu89 - c89 plus GCC extensions
# c99   - ISO C99 standard (not yet fully implemented)
# gnu99 - c99 plus GCC extensions
CSTANDARD = -std=gnu99

# Place -D or -U options for C here
CDEFS =  -D$(RUN_MODE)

# Place -I options here
CINCS =

# Place -D or -U options for ASM here
ADEFS =  -D$(RUN_MODE)


# Compiler flags.
#  -g*:          generate debugging information
#  -O*:          optimization level
#  -f...:        tuning, see GCC manual and avr-libc documentation
#  -Wall...:     warning level
#  -Wa,...:      tell GCC to pass this to the assembler.
#    -adhlns...: create assembler listing
CFLAGS = -g$(DEBUG)
CFLAGS += $(CDEFS) $(CINCS)
CFLAGS += -O$(OPT)
CFLAGS += -Wall -Wstrict-prototypes -Wcast-align -Wcast-qual -Wimplicit 
CFLAGS += -Wmissing-declarations
CFLAGS += -Wmissing-prototypes -Wnested-externs -Wpointer-arith -Wswitch
CFLAGS += -Wredundant-decls -Wreturn-type -Wshadow -Wstrict-prototypes -Wunused
CFLAGS += -Wa,-adhlns=$(<:.c=.lst)
CFLAGS += $(patsubst %,-I%,$(EXTRAINCDIRS))
CFLAGS += $(CSTANDARD)
## NONO CFLAGS += -funsigned-char -funsigned-bitfields -fpack-struct -fshort-enums

# Assembler flags.
#  -Wa,...:   tell GCC to pass this to the assembler.
#  -ahlms:    create listing
#  -gstabs:   have the assembler create line number information; note that
#             for use in COFF files, additional information about filenames
#             and function names needs to be present in the assembler source
#             files -- see avr-libc docs [FIXME: not yet described there]
##ASFLAGS = -Wa,-adhlns=$(<:.S=.lst),-gstabs 
ASFLAGS = $(ADEFS) -Wa,-adhlns=$(<:.S=.lst),-g$(DEBUG)

#Additional libraries.

#Support for newlibc-lpc (file: libnewlibc-lpc.a)
#NEWLIBLPC = -lnewlib-lpc
NEWLIBCLPC =

MATH_LIB = -lm

# Linker flags.
#  -Wl,...:     tell GCC to pass this to linker.
#    -Map:      create map file
#    --cref:    add cross reference to  map file
LDFLAGS = -nostartfiles -Wl,-Map=$(TARGET).map,--cref
LDFLAGS += -lc
LDFLAGS += $(NEWLIBLPC) $(MATH_LIB)
LDFLAGS += -lc -lgcc

# Set Linker-Script Depending On Selected Memory
ifeq ($(RUN_MODE),RAM_RUN)
LDFLAGS +=-Lsystem -T$(SUBMDL)-RAM.ld
else 
LDFLAGS +=-Lsystem -T$(SUBMDL)-ROM.ld
endif



# ---------------------------------------------------------------------------
# Flash-Programming support using lpc21isp by Martin Maurer 

# Settings and variables:
LPC21ISP = lpc21isp
LPC21ISP_PORT1 = /dev/ttyUSB0
LPC21ISP_PORT2 = /dev/ttyUSB1
LPC21ISP_PORT3 = /dev/ttyUSB2
LPC21ISP_BAUD = 115200
LPC21ISP_XTAL = 14746
#LPC21ISP_BAUD = 38400
#LPC21ISP_XTAL = 12000
#LPC21ISP_FLASHFILE = $(TARGET).hex
# verbose output:
## LPC21ISP_DEBUG = -debug
# enter bootloader via RS232 DTR/RTS (only if hardware supports this
# feature - see Philips AppNote):
LPC21ISP_CONTROL = -control


# ---------------------------------------------------------------------------

# Define directories, if needed.
## DIRARM = c:/WinARM/
## DIRARMBIN = $(DIRAVR)/bin/
## DIRAVRUTILS = $(DIRAVR)/utils/bin/

# Define programs and commands.
SHELL = sh
CC = $(TOOLCHAIN)gcc
OBJCOPY = $(TOOLCHAIN)objcopy
OBJDUMP = $(TOOLCHAIN)objdump
SIZE = $(TOOLCHAIN)size
NM = $(TOOLCHAIN)nm
REMOVE = rm -f
COPY = cp


# Define Messages
# English
MSG_ERRORS_NONE = Errors: none
MSG_BEGIN = -------- begin --------
MSG_END = --------  end  --------
MSG_SIZE_BEFORE = Size before: 
MSG_SIZE_AFTER = Size after:
MSG_FLASH = Creating load file for Flash:
MSG_EXTENDED_LISTING = Creating Extended Listing:
MSG_SYMBOL_TABLE = Creating Symbol Table:
MSG_LINKING = Linking:
MSG_COMPILING = Compiling:
MSG_ASSEMBLING = Assembling:
MSG_CLEANING = Cleaning project:
MSG_LPC21_RESETREMINDER = You may have to bring the target in bootloader-mode now.


# Define all object files.
OBJ = $(SRC:.c=.o) $(ASRC:.S=.o) 


# Define all listing files.
LST = $(ASRC:.S=.lst) $(SRC:.c=.lst)


# Compiler flags to generate dependency files.
### GENDEPFLAGS = -Wp,-M,-MP,-MT,$(*F).o,-MF,.dep/$(@F).d
GENDEPFLAGS = -MD -MP -MF .dep/$(@F).d

# Combine all necessary flags and optional flags.
# Add target processor to flags.
ALL_CFLAGS = -mcpu=$(MCU) $(THUMB_IW) -I. -Isystem $(CFLAGS) $(GENDEPFLAGS)
ALL_ASFLAGS = -mcpu=$(MCU) $(THUMB_IW) -I. -Isystem -x assembler-with-cpp $(ASFLAGS)


# Default target.
all: begin gccversion sizebefore build sizeafter finished end

build: elf hex lss sym

elf: $(TARGET).elf
hex: $(TARGET).hex
lss: $(TARGET).lss 
sym: $(TARGET).sym

# Eye candy.
begin:
	@echo
	@echo $(MSG_BEGIN)

finished:
	@echo $(MSG_ERRORS_NONE)

end:
	@echo $(MSG_END)
	@echo


# Display size of file.
HEXSIZE = $(SIZE) --target=$(FORMAT) $(TARGET).hex
ELFSIZE = $(SIZE) -A $(TARGET).elf
sizebefore:
	@if [ -f $(TARGET).elf ]; then echo; echo $(MSG_SIZE_BEFORE); $(ELFSIZE); echo; fi

sizeafter:
	@if [ -f $(TARGET).elf ]; then echo; echo $(MSG_SIZE_AFTER); $(ELFSIZE); echo; fi


# Display compiler version information.
gccversion : 
	@$(CC) --version


# Program the device.  
load program: $(TARGET).hex
	@echo
	@echo $(MSG_LPC21_RESETREMINDER)
	$(LPC21ISP) $(LPC21ISP_CONTROL) $(LPC21ISP_DEBUG) $< $(LPC21ISP_PORT1) $(LPC21ISP_BAUD) $(LPC21ISP_XTAL)

lterm1: $(TARGET).hex
	$(LPC21ISP) -term $(LPC21ISP_CONTROL) $(LPC21ISP_DEBUG) $< $(LPC21ISP_PORT1) $(LPC21ISP_BAUD) $(LPC21ISP_XTAL)

lterm2: $(TARGET).hex
	$(LPC21ISP) -term $(LPC21ISP_CONTROL) $(LPC21ISP_DEBUG) $< $(LPC21ISP_PORT2) $(LPC21ISP_BAUD) $(LPC21ISP_XTAL)

lterm3: $(TARGET).hex
	$(LPC21ISP) -term $(LPC21ISP_CONTROL) $(LPC21ISP_DEBUG) $< $(LPC21ISP_PORT3) $(LPC21ISP_BAUD) $(LPC21ISP_XTAL)

# Create final output files (.hex, .eep) from ELF output file.
# TODO: handling the .eeprom-section should be redundant
%.hex: %.elf
	@echo
	@echo $(MSG_FLASH) $@
	$(OBJCOPY) -O $(FORMAT) $< $@


# Create extended listing file from ELF output file.
%.lss: %.elf
	@echo
	@echo $(MSG_EXTENDED_LISTING) $@
	$(OBJDUMP) -h -S $< > $@


# Create a symbol table from ELF output file.
%.sym: %.elf
	@echo
	@echo $(MSG_SYMBOL_TABLE) $@
	$(NM) -n $< > $@


# Link: create ELF output file from object files.
.SECONDARY : $(TARGET).elf
.PRECIOUS : $(OBJ)
%.elf: $(OBJ)
	@echo
	@echo $(MSG_LINKING) $@
	$(CC) $(THUMB) $(ALL_CFLAGS) $(OBJ) --output $@ $(LDFLAGS)


# Compile: create object files from C source files.
%.o : %.c
	@echo
	@echo $(MSG_COMPILING) $<
	$(CC) -c $(THUMB) $(ALL_CFLAGS) $< -o $@ 


# Compile: create assembler files from C source files.
%.s : %.c
	$(CC) $(THUMB) -S $(ALL_CFLAGS) $< -o $@


# Assemble: create object files from assembler source files.
%.o : %.S
	@echo
	@echo $(MSG_ASSEMBLING) $<
	$(CC) -c $(ALL_ASFLAGS) $< -o $@


# Target: clean project.
clean: begin clean_list finished end


clean_list :
	@echo
	@echo $(MSG_CLEANING)
	$(REMOVE) $(TARGET).hex
	$(REMOVE) $(TARGET).obj
	$(REMOVE) $(TARGET).elf
	$(REMOVE) $(TARGET).map
	$(REMOVE) $(TARGET).obj
	$(REMOVE) $(TARGET).a90
	$(REMOVE) $(TARGET).sym
	$(REMOVE) $(TARGET).lnk
	$(REMOVE) $(TARGET).lss
	$(REMOVE) $(OBJ)
	$(REMOVE) $(LST)
	$(REMOVE) $(SRC:.c=.s)
	$(REMOVE) $(SRC:.c=.d)
	$(REMOVE) .dep/*
	$(REMOVE) utils/lpc21isp


# Include the dependency files.
-include $(shell mkdir .dep 2>/dev/null) $(wildcard .dep/*)


# Listing of phony targets.
.SILENT:
.PHONY : all begin finish end sizebefore sizeafter gccversion \
build elf hex lss sym \
clean clean_list program tags

tags:
	ctags -R --c++-kinds=+p --fields=+iaS --extra=+q .

