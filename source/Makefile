# If you move this project you can change the directory
# to match your GBDK root directory (ex: GBDK_HOME = "C:/GBDK/"


ifeq ($(strip $(GBDK_HOME)),) 
GBDK_HOME = c:/gbdk/
endif

LCC = $(GBDK_HOME)bin/lcc

# Set platforms to build here, spaced separated. (These are in the separate Makefile.targets)
# They can also be built/cleaned individually: "make gg" and "make gg-clean"
# Possible are: gb gbc pocket megaduck sms gg
TARGETS=gb gbc pocket megaduck gg sms

# Configure platform specific LCC flags here:
LCCFLAGS_gb      = -Wm-yn"WATERNET" -Wl-yo2 -Wl-ya1 -Wl-yt0x1B # (-Wl-yt0x1B Set an MBC for banking (1B-ROM+MBC5+RAM+BATT)) (-Wl-yo2 = 2 x 16k rom bank) (-Wl-ya1 = 1x 8k ram bank)
LCCFLAGS_pocket  = -Wm-yn"WATERNET" -Wl-yo2 -Wl-ya1 -Wl-yt0x1B -Wm-yc # Usually the same as required for .gbc
LCCFLAGS_duck    = -Wm-yn"WATERNET" -Wl-yo2 -Wl-ya1 -Wl-yt0x1B # Usually the same as required for .gb
LCCFLAGS_gbc     = -Wm-yn"WATERNET" -Wl-yo2 -Wl-ya1 -Wl-yt0x1B -Wm-yc # Same as .gb with: -Wm-yc (gb & gbc) or Wm-yC (gbc exclusive)
LCCFLAGS_sms     = -Wl-yo4 -Wl-ya1 -Wm-xj4 -Wm-xv0 #-Wl-yo4 Master System needs bigger than 32k rom or sram does not work in emulicious, -Wl-ya1 ram bank (not sure it's used with sega), -Wm-xj4 Sms Export region, -Wm-xo0xe, -Wm-xv0 version 0 (or 1st version)
LCCFLAGS_gg      = -Wl-yo4 -Wl-ya1 -Wm-xj7 -Wm-xv0 #-Wl-yo4 Game gear needs bigger than 32k rom or sram does not work in emulicious, -Wl-ya1 ram bank (not sure it's used with sega), -Wm-xj7 GG international region, -Wm-xo0xe, -Wm-xv0 version 0 (or 1st version)

LCCFLAGS += $(LCCFLAGS_$(EXT)) # This adds the current platform specific LCC Flags

#LCCFLAGS += -Wl-j -Wm-yoA -Wm-ya1 -autobank -Wb-ext=.rel -Wb-v # MBC + Autobanking related flags
LCCFLAGS += -Wl-m -Wa-l # Wl-m = generate map files, -Wa-l = generate assembler listing
# LCCFLAGS += -v   # Uncomment for lcc verbose output

CFLAGS +=  -debug # Uncomment to enable debug output, (this sets --debug for sdcc (lcc equiv: -Wf-debug), -y enables .cdb output for sdldgb (lcc equiv: -Wl-y), -j enables .noi output for sdldgb (lcc equiv: -Wl-j))

CFLAGS_RELEASE = -Wf"--max-allocs-per-node 1000000" # will be added to CFLAGS with make ##-release targets like for example `make gb-release` or `make release` for all platforms

# You can set the name of the ROM file here
PROJECTNAME = waternet

# EXT?=gb # Only sets extension to default (game boy .gb) if not populated
SRCDIR      = src
OBJDIR      = obj/$(EXT)
RESDIR      = res
BINDIR      = build/$(EXT)
MKDIRS      = $(OBJDIR) $(BINDIR) # See bottom of Makefile for directory auto-creation

BINS	    = $(OBJDIR)/$(PROJECTNAME).$(EXT)
CSOURCES    = $(foreach dir,$(SRCDIR),$(notdir $(wildcard $(dir)/*.c))) $(foreach dir,$(RESDIR),$(notdir $(wildcard $(dir)/*.c)))
ASMSOURCES  = $(foreach dir,$(SRCDIR),$(notdir $(wildcard $(dir)/*.s)))
OBJS       = $(CSOURCES:%.c=$(OBJDIR)/%.o) $(ASMSOURCES:%.s=$(OBJDIR)/%.o)

# Builds all targets sequentially
all: $(TARGETS)

# Compile .c files in "src/" to .o object files
$(OBJDIR)/%.o:	$(SRCDIR)/%.c
	$(LCC) $(CFLAGS) -c -o $@ $<

# Compile .c files in "res/" to .o object files
$(OBJDIR)/%.o:	$(RESDIR)/%.c
	$(LCC) $(CFLAGS) -c -o $@ $<

# Compile .s assembly files in "src/" to .o object files
$(OBJDIR)/%.o:	$(SRCDIR)/%.s
	$(LCC) $(CFLAGS) -c -o $@ $<

# If needed, compile .c files i n"src/" to .s assembly files
# (not required if .c is compiled directly to .o)
$(OBJDIR)/%.s:	$(SRCDIR)/%.c
	$(LCC) $(CFLAGS) -S -o $@ $<

# Link the compiled object files into a .gb ROM file
$(BINS):	$(OBJS)
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
