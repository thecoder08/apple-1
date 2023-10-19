#include "mos6502.h"
#include "loadfile.h"
#include <stdio.h>
#include <string.h>
#include <sys/select.h>
#include <termios.h>
#include <unistd.h>
#include <stdlib.h>

uint8_t memory[0x10000];

// PIA registers
uint8_t crA = 0xa7;
uint8_t ddrA;
uint8_t prA;
uint8_t crB = 0xa7;
uint8_t ddrB;
uint8_t prB;

struct termios orig_termios;

void reset_terminal_mode()
{
    tcsetattr(0, TCSANOW, &orig_termios);
}

void set_conio_terminal_mode()
{
    struct termios new_termios;

    /* take two copies - one for now, one for later */
    tcgetattr(0, &orig_termios);
    memcpy(&new_termios, &orig_termios, sizeof(new_termios));

    /* register cleanup handler, and set the new terminal mode */
    atexit(reset_terminal_mode);
    cfmakeraw(&new_termios);
    tcsetattr(0, TCSANOW, &new_termios);
}

int kbhit()
{
    struct timeval tv = { 0L, 0L };
    fd_set fds;
    FD_ZERO(&fds);
    FD_SET(0, &fds);
    return select(1, &fds, NULL, NULL, &tv) > 0;
}

int getch()
{
    int r;
    unsigned char c;
    if ((r = read(0, &c, sizeof(c))) < 0) {
        return r;
    } else {
        return c;
    }
}

void busWrite(uint16_t address, uint8_t data) {
  if (address == 0xd010) {
    if (crA & 0x4) {
      prA = data;
    }
    else {
      ddrA = data;
    }
    return;
  }
  if (address == 0xd011) {
    crA = data;
    return;
  }
  if (address == 0xd012) {
    if (crB & 0x4) {
      prB = data & 0x7f;
      if (prB == '\r') {
        printf("\r\n");
      }
      else {
        putchar(prB);
      }
      fflush(stdout);
    }
    else {
      ddrB = data;
    }
    return;
  }
  if (address == 0xd013) {
    crB = data;
    return;
  }
  if (address == 0xd020) {

  }
  memory[address] = data;
}

uint8_t busRead(uint16_t address) {
  if (address == 0xd010) {
    if (crA & 0x4) {
      prA = getch() | 0x80;
      return prA;
    }
    else {
      return ddrA;
    }
  }
  if (address == 0xd011) {
    if (kbhit()) {
      crA |= 0x80;
    }
    else {
      crA &= 0x7f;
    }
    return crA;
  }
  if (address == 0xd012) {
    if (crB & 0x4) {
      return prB;
    }
    else {
      return ddrB;
    }
  }
  if (address == 0xd013) {
    return crB;
  }
  return memory[address];
}

int main() {
  if (loadFile("a1basic.bin", memory + 0xe000)) {
    printf("Error loading BASIC ROM!\n");
    return 1;
  }
  if (loadFile("wozmon.bin", memory + 0xff00)) {
    printf("Error loading WozMon!\n");
    return 1;
  }
  set_conio_terminal_mode();
  mos6502 processor = mos6502(busRead, busWrite);
  uint64_t cycles;
  processor.Reset();
  processor.RunEternally();
  return 0;
}
