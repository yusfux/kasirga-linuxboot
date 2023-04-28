#include "kasirga.h"

volatile uint32_t* UART_CONTROL  = (uint32_t*) (0x20000000);
volatile uint32_t* UART_STATUS   = (uint32_t*) (0x20000004);
volatile uint32_t* UART_READ     = (uint32_t*) (0x20000008);
volatile uint32_t* UART_WRITE    = (uint32_t*) (0x2000000c);

void handle_trap() {
    static char error_msg[7] = "Hata!\n";
    uart_print(error_msg);
}

uint32_t kasirga_random(int seed) {
    unsigned int state = seed * 747796405u + 2891336453u;
    unsigned int word = ((state >> ((state >> 28u) + 4u)) ^ state) * 277803737u;
    return (word >> 22u) ^ word;
}

void uart_set_ctrl(uint16_t baud_div, uint8_t rx_en, uint8_t tx_en) {
    uint32_t ctrl = baud_div << 16 | rx_en << 1 | tx_en;
    *UART_CONTROL = ctrl;
}

int uart_tx_full() {
    uint32_t status = *UART_STATUS;
    return (status & 1 << TX_FULL_BIT) == 1 << TX_FULL_BIT;
}

int uart_rx_full() {
    uint32_t status = *UART_STATUS;
    return (status & 1 << RX_FULL_BIT) == 1 << RX_FULL_BIT;
}

int uart_tx_empty() {
    uint32_t status = *UART_STATUS;
    return (status & 1 << TX_EMPTY_BIT) == 1 << TX_EMPTY_BIT;
}

int uart_rx_empty() {
    uint32_t status = *UART_STATUS;
    return (status & 1 << RX_EMPTY_BIT) == 1 << RX_EMPTY_BIT;
}

void uart_write(uint8_t data) {
    while(uart_tx_full());
    *UART_WRITE = data;
}

uint8_t uart_read() {
    while(uart_rx_empty());
    return *UART_READ;
}

void uart_print(const char* str) {
    int idx = 0;
    while (str[idx] != '\0') {
        uart_write(str[idx]);
        idx++;
    }
}
// Return the absolute value of a.
// Note LLINT_MIN cannot be negated.
arith64_s64 __absvdi2(arith64_s64 a)
{
    return arith64_abs(a);
}

// Return the result of shifting a left by b bits.
arith64_s64 __ashldi3(arith64_s64 a, int b)
{
    arith64_word w = {.s64 = a};

    b &= 63;

    if (b >= 32)
    {
        w.u32.hi = w.u32.lo << (b - 32);
        w.u32.lo = 0;
    } else if (b)
    {
        w.u32.hi = (w.u32.lo >> (32 - b)) | (w.u32.hi << b);
        w.u32.lo <<= b;
    }
    return w.s64;
}

// Return the result of arithmetically shifting a right by b bits.
arith64_s64 __ashrdi3(arith64_s64 a, int b)
{
    arith64_word w = {.s64 = a};

    b &= 63;

    if (b >= 32)
    {
        w.s32.lo = w.s32.hi >> (b - 32);
        w.s32.hi >>= 31; // 0xFFFFFFFF or 0
    } else if (b)
    {
        w.u32.lo = (w.u32.hi << (32 - b)) | (w.u32.lo >> b);
        w.s32.hi >>= b;
    }
    return w.s64;
}

// These functions return the number of leading 0-bits in a, starting at the
// most significant bit position. If a is zero, the result is undefined.
int __clzsi2(arith64_u32 a)
{
    int b, n = 0;
    b = !(a & 0xffff0000) << 4; n += b; a <<= b;
    b = !(a & 0xff000000) << 3; n += b; a <<= b;
    b = !(a & 0xf0000000) << 2; n += b; a <<= b;
    b = !(a & 0xc0000000) << 1; n += b; a <<= b;
    return n + !(a & 0x80000000);
}

int __clzdi2(arith64_u64 a)
{
    int b, n = 0;
    b = !(a & 0xffffffff00000000ULL) << 5; n += b; a <<= b;
    b = !(a & 0xffff000000000000ULL) << 4; n += b; a <<= b;
    b = !(a & 0xff00000000000000ULL) << 3; n += b; a <<= b;
    b = !(a & 0xf000000000000000ULL) << 2; n += b; a <<= b;
    b = !(a & 0xc000000000000000ULL) << 1; n += b; a <<= b;
    return n + !(a & 0x8000000000000000ULL);
}

// These functions return the number of trailing 0-bits in a, starting at the
// least significant bit position. If a is zero, the result is undefined.
int __ctzsi2(arith64_u32 a)
{
    int b, n = 0;
    b = !(a & 0x0000ffff) << 4; n += b; a >>= b;
    b = !(a & 0x000000ff) << 3; n += b; a >>= b;
    b = !(a & 0x0000000f) << 2; n += b; a >>= b;
    b = !(a & 0x00000003) << 1; n += b; a >>= b;
    return n + !(a & 0x00000001);
}

int __ctzdi2(arith64_u64 a)
{
    int b, n = 0;
    b = !(a & 0x00000000ffffffffULL) << 5; n += b; a >>= b;
    b = !(a & 0x000000000000ffffULL) << 4; n += b; a >>= b;
    b = !(a & 0x00000000000000ffULL) << 3; n += b; a >>= b;
    b = !(a & 0x000000000000000fULL) << 2; n += b; a >>= b;
    b = !(a & 0x0000000000000003ULL) << 1; n += b; a >>= b;
    return n + !(a & 0x0000000000000001ULL);
}

// Calculate both the quotient and remainder of the unsigned division of a and
// b. The return value is the quotient, and the remainder is placed in variable
// pointed to by c (if it's not NULL).
arith64_u64 __divmoddi4(arith64_u64 a, arith64_u64 b, arith64_u64 *c)
{
    if (b > a)                                  // divisor > numerator?
    {
        if (c) *c = a;                          // remainder = numerator
        return 0;                               // quotient = 0
    }
    if (!arith64_hi(b))                         // divisor is 32-bit
    {
        if (b == 0)                             // divide by 0
        {
            volatile char x = 0; x = 1 / x;     // force an exception
        }
        if (b == 1)                             // divide by 1
        {
            if (c) *c = 0;                      // remainder = 0
            return a;                           // quotient = numerator
        }
        if (!arith64_hi(a))                     // numerator is also 32-bit
        {
            if (c)                              // use generic 32-bit operators
                *c = arith64_lo(a) % arith64_lo(b);
            return arith64_lo(a) / arith64_lo(b);
        }
    }

    // let's do long division
    char bits = __clzdi2(b) - __clzdi2(a) + 1;  // number of bits to iterate (a and b are non-zero)
    arith64_u64 rem = a >> bits;                   // init remainder
    a <<= 64 - bits;                            // shift numerator to the high bit
    arith64_u64 wrap = 0;                          // start with wrap = 0
    while (bits-- > 0)                          // for each bit
    {
        rem = (rem << 1) | (a >> 63);           // shift numerator MSB to remainder LSB
        a = (a << 1) | (wrap & 1);              // shift out the numerator, shift in wrap
        wrap = ((arith64_s64)(b - rem - 1) >> 63);  // wrap = (b > rem) ? 0 : 0xffffffffffffffff (via sign extension)
        rem -= b & wrap;                        // if (wrap) rem -= b
    }
    if (c) *c = rem;                            // maybe set remainder
    return (a << 1) | (wrap & 1);               // return the quotient
}

// Return the quotient of the signed division of a and b.
arith64_s64 __divdi3(arith64_s64 a, arith64_s64 b)
{
    arith64_u64 q = __divmoddi4(arith64_abs(a), arith64_abs(b), (void *)0);
    return arith64_neg(q, a^b); // negate q if a and b signs are different
}

// Return the index of the least significant 1-bit in a, or the value zero if a
// is zero. The least significant bit is index one.
int __ffsdi2(arith64_u64 a)
{
    return a ? __ctzdi2(a) + 1 : 0;
}

// Return the result of logically shifting a right by b bits.
arith64_u64 __lshrdi3(arith64_u64 a, int b)
{
    arith64_word w = {.u64 = a};

    b &= 63;

    if (b >= 32)
    {
        w.u32.lo = w.u32.hi >> (b - 32);
        w.u32.hi = 0;
    } else if (b)
    {
        w.u32.lo = (w.u32.hi << (32 - b)) | (w.u32.lo >> b);
        w.u32.hi >>= b;
    }
    return w.u64;
}

// Return the remainder of the signed division of a and b.
arith64_s64 __moddi3(arith64_s64 a, arith64_s64 b)
{
    arith64_u64 r;
    __divmoddi4(arith64_abs(a), arith64_abs(b), &r);
    return arith64_neg(r, a); // negate remainder if numerator is negative
}

// Return the number of bits set in a.
int __popcountsi2(arith64_u32 a)
{
    // collect sums into two low bytes
    a = a - ((a >> 1) & 0x55555555);
    a = ((a >> 2) & 0x33333333) + (a & 0x33333333);
    a = (a + (a >> 4)) & 0x0F0F0F0F;
    a = (a + (a >> 16));
    // add the bytes, return bottom 6 bits
    return (a + (a >> 8)) & 63;
}

// Return the number of bits set in a.
int __popcountdi2(arith64_u64 a)
{
    // collect sums into two low bytes
    a = a - ((a >> 1) & 0x5555555555555555ULL);
    a = ((a >> 2) & 0x3333333333333333ULL) + (a & 0x3333333333333333ULL);
    a = (a + (a >> 4)) & 0x0F0F0F0F0F0F0F0FULL;
    a = (a + (a >> 32));
    a = (a + (a >> 16));
    // add the bytes, return bottom 7 bits
    return (a + (a >> 8)) & 127;
}

// Return the quotient of the unsigned division of a and b.
arith64_u64 __udivdi3(arith64_u64 a, arith64_u64 b)
{
    return __divmoddi4(a, b, (void *)0);
}

// Return the remainder of the unsigned division of a and b.
arith64_u64 __umoddi3(arith64_u64 a, arith64_u64 b)
{
    arith64_u64 r;
    __divmoddi4(a, b, &r);
    return r;
}