#pragma once

#include <typedefs.h>

void vga_init(u8 threshold);
static u16 vga_entry(u8 ch, u8 color);
static void vga_scroll();
void vga_printchar(u8 ch, u8 color);
void vga_printstr(char* str, u8 color);
void vga_println(char* str, u8 color);
void vga_clear();
void vga_paintbg(u8 color);
void vga_paintline(u8 line, u8 color);
