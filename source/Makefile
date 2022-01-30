# If you move this project you can change the directory
# to match your GBDK root directory (ex: GBDK_HOME = "C:/GBDK/"


ifeq ($(strip $(GBDK_HOME)),) 
GBDK_HOME = c:/gbdk/
endif

LCC = $(GBDK_HOME)bin/lcc

# Set platforms to build here, spaced separated. (These are in the separate Makefile.targets)
# They can also be built/cleaned individually: "make gg" and "make gg-clean"
# Possible are: gb gbc pocket megaduck sms gg
TARGETS=gbc gbc-batteryless-DMG_Generic_WR_555_A9 pocket megaduck gg sms

#when changing this value don't forget to change _CODE_4 in dummydata.s, this value is the (rom) bank nr and must be multiple of 4 + make sure it not overwrites current code
START_SAVE=4

# Configure platform specific LCC flags here:

# For normal Rom with cartridge with battery
# -Wl-yt0x1B  Set cartridge type (ROM+MBC5+RAM+BATT)
# -Wl-ya1     1x 8k ram bank
# -Wm-yoA     Auto RomBanking

# For Batteryless Saving roms, catridges without battery
# -Wl-yt0x1A  Set cartridge type (ROM+MBC5+RAM) 
# -Wl-ya4     4x 8k ram bank the flasher will always spoil 4 rom banks because of 64k sector size 
# -Wm-yo8     8x 16k rom bank startsave bank is 4 + 4 spoiled rombanks from flasher (must make sure rom does not exceed 4 rom banks in this case)

# Other things
# -Wm-ys for sgb border stuff etc
# -Wm-yc (gb & gbc) or Wm-yC (gbc exclusive)
# -Wm-xj4 Sms Export region, 
# -Wm-xj7 GG international region
# -Wm-xv0 version 0 (or 1st version)

LCCFLAGS_gb     = -Wm-yn"WATERNET" -Wl-ya1 -Wm-yoA -Wl-yt0x1B -Wm-ys 
LCCFLAGS_duck   = -Wm-yn"WATERNET" -Wl-ya1 -Wm-yoA -Wl-yt0x1B #NO SGB STUFF HERE
LCCFLAGS_pocket = -Wm-yn"WATERNET" -Wl-ya1 -Wm-yoA -Wl-yt0x1B -Wm-ys -Wm-yc
LCCFLAGS_gbc    = -Wm-yn"WATERNET" -Wl-ya1 -Wm-yoA -Wl-yt0x1B -Wm-ys -Wm-yc 
LCCFLAGS_sms    = -Wl-ya1 -Wm-yoA -Wm-xj4 -Wm-xv0 #Master System needs bigger than 32k rom or sram does not work in emulicious, my game is bigger
LCCFLAGS_gg     = -Wl-ya1 -Wm-yoA -Wm-xj7 -Wm-xv0 #Game gear needs bigger than 32k rom or sram does not work in emulicious, my game is bigger

LCCFLAGS += $(LCCFLAGS_$(EXT)) # This adds the current platform specific LCC Flags

#batteryless saves gameboy color settings defined when CARTTYPEDIR is supplied so that i don't have to keep repeating the settings
ifneq ($(strip $(CARTTYPEDIR)),) 
	LCCFLAGS += -Wm-yn"WATERNET" -Wl-ya4 -Wm-yo8 -Wl-yt0x1A -Wm-ys -Wm-yc
endif

LCCFLAGS += -autobank -Wb-ext=.rel -Wb-v # Autobanking related flags
LCCFLAGS += -Wl-m -Wa-l # Wl-m = generate map files, -Wa-l = generate assembler listing
# LCCFLAGS += -v   # Uncomment for lcc verbose output

CFLAGS += -debug # Uncomment to enable debug output, (this sets --debug for sdcc (lcc equiv: -Wf-debug), -y enables .cdb output for sdldgb (lcc equiv: -Wl-y), -j enables .noi output for sdldgb (lcc equiv: -Wl-j))
CFLAGS_RELEASE = -Wf"--max-allocs-per-node 1000000" # will be added to CFLAGS with make ##-release targets like for example `make gb-release` or `make release` for all platforms

# You can set the name of the ROM file here
PROJECTNAME = waternet

# EXT?=gb # Only sets extension to default (game boy .gb) if not populated
SRCDIR        = src
OBJDIR        = obj/$(EXT)
RESDIR        = res
BINDIR        = build/$(EXT)
MKDIRS        = $(OBJDIR) $(BINDIR) # See bottom of Makefile for directory auto-creation
BINS          = $(OBJDIR)/$(PROJECTNAME).$(EXT)
ASMFLASHERDIR =
SRCFLASHERDIR =
ASMFLASHERSRM = 

#include flasher sources from TOXA adapted to my cartridge when using batterylesssave
ifneq ($(strip $(CARTTYPEDIR)),) 
    SRCFLASHERDIR = flasher
    ASMFLASHERDIR = flasher/$(CARTTYPEDIR)
    ASMFLASHERSRM = flasher/sram_init
    LCCFLAGS += -Wl-g.start_save=$(START_SAVE)
	CFLAGS += -DBATTERYLESSSAVE
endif

CSOURCES    = $(foreach dir,$(SRCDIR),$(notdir $(wildcard $(dir)/*.c))) $(foreach dir,$(SRCFLASHERDIR),$(notdir $(wildcard $(dir)/*.c))) $(foreach dir,$(RESDIR),$(notdir $(wildcard $(dir)/*.c)))
ASMSOURCES  = $(foreach dir,$(SRCDIR),$(notdir $(wildcard $(dir)/*.s))) $(foreach dir,$(ASMFLASHERDIR),$(notdir $(wildcard $(dir)/*.s))) $(foreach dir,$(ASMFLASHERSRM),$(notdir $(wildcard $(dir)/*.s)))
OBJS        = $(CSOURCES:%.c=$(OBJDIR)/%.o) $(ASMSOURCES:%.s=$(OBJDIR)/%.o)

# Builds all targets sequentially
all: $(TARGETS)

# Compile .c files in "flasher/" to .o object files
$(OBJDIR)/%.o: $(SRCFLASHERDIR)/%.c $(SRCFLASHERDIR)/%.h
	$(LCC) $(CFLAGS) -c -o $@ $<

# Compile .c files in "src/" to .o object files
$(OBJDIR)/%.o: $(SRCDIR)/%.c $(SRCDIR)/%.h
	$(LCC) $(CFLAGS) -c -o $@ $<

# Compile .c files in "res/" to .o object files
$(OBJDIR)/%.o: $(RESDIR)/%.c  $(RESDIR)/%.h
	$(LCC) $(CFLAGS) -c -o $@ $<

# Compile .s assembly files in "Flasher/sram_init" to .o object files
$(OBJDIR)/%.o: $(ASMFLASHERSRM)/%.s
	$(LCC) $(CFLAGS) -c -o $@ $<

# Compile .s assembly files in "Flasher/XXXX" to .o object files
$(OBJDIR)/%.o: $(ASMFLASHERDIR)/%.s
	$(LCC) $(CFLAGS) -c -o $@ $<

# Compile .s assembly files in "src/" to .o object files
$(OBJDIR)/%.o: $(SRCDIR)/%.s
	$(LCC) $(CFLAGS) -c -o $@ $<

# If needed, compile .c files in "src/" to .s assembly files
# (not required if .c is compiled directly to .o)
$(OBJDIR)/%.s: $(SRCDIR)/%.c
	$(LCC) $(CFLAGS) -S -o $@ $<

# If needed, compile .c files in "FLASHER/" to .s assembly files
# (not required if .c is compiled directly to .o)
$(OBJDIR)/%.s: $(SRCFLASHERDIR)/%.c
	$(LCC) $(CFLAGS) -S -o $@ $<

# Link the compiled object files into a .gb ROM file
$(BINS): $(OBJS)
	$(LCC) $(LCCFLAGS) $(CFLAGS) -o $(BINDIR)/$(PROJECTNAME).$(EXT) $(OBJS)

clean:
	@echo Cleaning
	@for target in $(TARGETS); do \
		$(MAKE) $$target-clean; \
	done

release:
	@for target in $(TARGETS); do \
		$(MAKE) $$target-release; \
	done

# Include available build targets
include Makefile.targets

# create necessary directories after Makefile is parsed but before build
# info prevents the command from being pasted into the makefile
ifneq ($(strip $(EXT)),)                                # Only make the directories if EXT has been set by a target
    $(info $(shell mkdir -p $(MKDIRS)))
endif
