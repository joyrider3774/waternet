#ifndef CURSOR_H
#define CURSOR_H

#define MAXCURSORFRAMECOUNT 10
#define CURSORANIMCOUNT 4

void updateCursorFrame();
void hideCursors();
void showCursors();
void setCursorPos(uint8_t cursorNr, uint8_t xPos, uint8_t yPos);
void initCursors();

#endif