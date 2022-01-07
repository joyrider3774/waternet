@REM you should pass --debug to frontend (Wf--debug)
@REM you should pass -y to the linker to make it create a CDB file used for debugging (-Wl-y )
@REM and also pass -m -w to the linker to produce "wide maps" that are useful for emulicious (-Wl-m)
@REM add -Wl-j if you want to create NoICE symbol file .NOI for use in BGB emulator

@REM Main Source files
c:\gbdk\bin\lcc -Wa-l -Wl-m -Wf--debug -Wl-y -Wl-w -Wm-yc -c -o main.o main.c

@REM SaveState for SRAM
c:\gbdk\bin\lcc -Wa-l -Wl-m -Wf--debug -Wl-y -Wl-w -Wm-yc -Wf-ba0 -c -o savestate.o savestate.c

@REM Tiles
c:\gbdk\bin\lcc -Wa-l -Wl-m -Wf--debug -Wl-y -Wl-w -Wm-yc -c -o tiles/blocks.o tiles/blocks.c
c:\gbdk\bin\lcc -Wa-l -Wl-m -Wf--debug -Wl-y -Wl-w -Wm-yc -c -o tiles/titlescreen.o tiles/titlescreen.c
c:\gbdk\bin\lcc -Wa-l -Wl-m -Wf--debug -Wl-y -Wl-w -Wm-yc -c -o tiles/selector.o tiles/selector.c

@REM Maps
c:\gbdk\bin\lcc -Wa-l -Wl-m -Wf--debug -Wl-y -Wl-w -Wm-yc -c -o maps/gamebackground.o maps/gamebackground.c
c:\gbdk\bin\lcc -Wa-l -Wl-m -Wf--debug -Wl-y -Wl-w -Wm-yc -c -o maps/titlescreen_map.o maps/titlescreen_map.c
c:\gbdk\bin\lcc -Wa-l -Wl-m -Wf--debug -Wl-y -Wl-w -Wm-yc -c -o maps/othermaps.o maps/othermaps.c

@REM Link it to binary
c:\gbdk\bin\lcc -Wa-l -Wl-m -Wf--debug -Wl-y -Wl-w -Wl-yt0x1b -Wl-yo2 -Wl-ya1 -Wm-yc -o waternet.gb maps/othermaps.o maps/titlescreen_map.o maps/gamebackground.o tiles/selector.o tiles/titlescreen.o tiles/blocks.o savestate.o main.o