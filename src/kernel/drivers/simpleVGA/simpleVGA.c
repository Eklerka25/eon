#include <typedefs.h>

#define VGA_WIDTH 80
#define VGA_HEIGHT 25
#define VGA_ADDRESS 0xB8000

static u8 vga_curX = 0;
static u8 vga_curY = 0;
static u8 vga_scrollThreshold = VGA_HEIGHT;
static u16* vga_buffer = (u16*)VGA_ADDRESS;

void vga_init(u8 threshold) {
    vga_scrollThreshold = threshold;
}

static u16 vga_entry(u8 ch, u8 color) {
    return (u16)ch | (u16)color << 8;
}

static void vga_scroll() {
    u8 row;
    u8 col;
    u16 blank = vga_entry(' ', 0);
    for (row = vga_scrollThreshold; row < VGA_HEIGHT - 1; row++) {
        for (col = 0; col < VGA_WIDTH; col++) {
            vga_buffer[row * VGA_WIDTH + col] = vga_buffer[(row + 1) * VGA_WIDTH + col];
        }
    }
    for (col = 0; col < VGA_WIDTH; col++) {
        vga_buffer[(VGA_HEIGHT - 1) * VGA_WIDTH + col] = blank;
    }
    vga_curY = VGA_HEIGHT - 1;
    vga_curX = 0;
}

void vga_printchar(u8 ch, u8 color) {
    if (ch == '\n') {
        vga_curX = 0;
        vga_curY++;
        if (vga_curY >= vga_scrollThreshold) vga_scroll();
        return;
    }
    if (vga_curX >= VGA_WIDTH) {
        vga_curX = 0;
        vga_curY++;
    }
    if (vga_curY >= vga_scrollThreshold) {
        vga_scroll();
    }
    ut index = vga_curY * VGA_WIDTH + vga_curX;
    vga_buffer[index] = vga_entry(ch, color);
    vga_curX++;
}

void vga_printstr(char* str, u8 color) {
    while (*str) {
        vga_printchar(*str++, color);
    }
}

void vga_println(char* str, u8 color) {
    vga_printstr(str, color);
    vga_printchar('\n', color);
}

void vga_clear() {
    ut i;
    u16 blank = vga_entry(' ', 0);
    for (i = 0; i < VGA_WIDTH * VGA_HEIGHT; i++) {
        vga_buffer[i] = blank;
    }
    vga_curX = 0;
    vga_curY = 0;
}

void vga_paintbg(u8 color) {
    ut i;
    u16 entry;
    for (i = 0; i < VGA_WIDTH * VGA_HEIGHT; i++) {
        entry = vga_buffer[i] & 0x00FF;
        vga_buffer[i] = entry | (u16)color << 8;
    }
}

void vga_paintline(u8 line, u8 color) {
    u8 col;
    u16 entry;
    if (line >= VGA_HEIGHT) return;
    for (col = 0; col < VGA_WIDTH; col++) {
        entry = vga_buffer[line * VGA_WIDTH + col] & 0x00FF;
        vga_buffer[line * VGA_WIDTH + col] = entry | (u16)color << 8;
    }
}
