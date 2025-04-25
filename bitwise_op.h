#ifndef BITWISE_OP_H
#define BITWISE_OP_H

unsigned char applyXOR(const unsigned char* pixel, unsigned char mask);
unsigned char rotateRightByte(unsigned char byte, int n);
unsigned char rotateLeftByte(unsigned char byte, int n);
unsigned char shiftRightBuffer(const unsigned char* pixel);
unsigned char shiftLeftBuffer(const unsigned char* pixel);

#endif // BITWISE_OP_H
