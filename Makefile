# path to snesdev root directory (for emulators, devkitsnes, libsnes)
export DEVKITSNES := /home/utilisateur

# path to devkitsnes root directory for compiler
export DEVKIT65XX := /home/utilisateur/devkitsnes

#---------------------------------------------------------------------------------
.SUFFIXES:
#---------------------------------------------------------------------------------

ifeq ($(strip $(DEVKIT65XX)),)
$(error "Please set DEVKIT65XX in your environment. export DEVKIT65XX=<path to>devkit65XX")
endif

include $(DEVKIT65XX)/snes_rules

#---------------------------------------------------------------------------------
# TARGET is the name of the output
# BUILD is the directory where object files & intermediate files will be placed
# SOURCES is a list of directories containing source code
# INCLUDES is a list of directories containing extra header files
#---------------------------------------------------------------------------------
TARGET		:=	$(shell basename $(CURDIR))
SOURCES		:=	.

#---------------------------------------------------------------------------------
# options for code generation
#---------------------------------------------------------------------------------
CFLAGS	+=	$(INCLUDE) 

#---------------------------------------------------------------------------------
# list of directories containing libraries, this must be the top level containing
# include and lib
#---------------------------------------------------------------------------------
LIBDIRS	:=	$(PVSNESLIB)
LIBOBJS +:=	
 
export OUTPUT	:=	$(CURDIR)/$(TARGET)
 
CFILES		:=	$(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.c)))
SFILES		:=	$(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.asm)))
 
export AUDIOFILES :=	$(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.it)))

ifneq ($(AUDIOFILES),)
ifeq ($(wildcard soundbank.asm),)
	SFILES		:=	soundbank.asm $(SFILES)
endif
endif

#---------------------------------------------------------------------------------
export OFILES	:=	$(BINFILES:.bin=.obj) $(CFILES:.c=.obj) $(SFILES:.asm=.obj)
 
export INCLUDE	:=	$(foreach dir,$(INCLUDES),-I$(CURDIR)/$(dir)) \
					$(foreach dir,$(LIBDIRS),-I$(dir)/include) \
					-I$(CURDIR)/$(BUILD)

SMCONVFLAGS	:= -l -s -o soundbank -v -b 5
					
GTITLE 		:= -ht"$(TARGET)"
 
.PHONY: bitmaps all
 
#---------------------------------------------------------------------------------
all	:	bitmaps $(OUTPUT).sfc
	$(SNTOOLS) -hi! $(GTITLE) $(TARGET).sfc

clean:
	@echo clean ...
	@rm -f $(OFILES) $(TARGET).sfc  *.pic *.pal *.map *.bnk *.brr soundbank.asm soundbank.h

sfc	:	bitmaps $(OUTPUT).sfc
	$(SNTOOLS) -hi! $(GTITLE) $(TARGET).sfc
	@rm -f $(OFILES) *.pic *.pal *.map *.bnk *.brr soundbank.asm soundbank.h

#---------------------------------------------------------------------------------
soundbank.asm : $(AUDIOFILES)
	@echo Compiling Soundbank ...
	@$(SMCONV) $(SMCONVFLAGS) $^	

#--------------------------------------------------------------------------------
snesfont.pic: snesfont.bmp
	@echo convert font with no tile reduction ... $(notdir $@)
	$(GFXCONV) -n -gs8 -po4 -pc16 -pe1 -mR! -m! -p! $<

sprites.pic: sprites.bmp
	@echo convert bitmap ... $(notdir $@)
	$(GFXCONV) -gs8 -pc16 -po16 -n $<	
	
bitmaps : soundbank.asm hihi.brr sprites.pic snesfont.pic kawaii.pic tilekawaii.pic

#---------------------------------------------------------------------------------
$(OUTPUT).sfc	: $(OFILES)
