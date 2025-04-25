#include "bitwise_op.h"

unsigned char applyXOR(const unsigned char* pixel, unsigned char mask) {
    return pixel[0] ^ mask;
}

unsigned char rotateRightByte(unsigned char byte, int n) {
    return (byte >> n) | (byte << (8 - n));
}

unsigned char rotateLeftByte(unsigned char byte, int n) {
    return (byte << n) | (byte >> (8 - n));
}

unsigned char shiftRightBuffer(const unsigned char* pixel) {
    return pixel[0] >> 1;
}

unsigned char shiftLeftBuffer(const unsigned char* pixel) {
    return pixel[0] << 1;
}
