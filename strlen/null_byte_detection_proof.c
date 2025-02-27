#include <stdint.h>
#include <assert.h>

int main() {
    for (int i = 1; i <= 0xFF; ++i) {
        uint8_t byte = (uint8_t)i;
        uint8_t a = (uint8_t)byte - 0x01;
        uint8_t b = ~byte;

        if (byte <= 0x80) {
            assert(!(a & 0x80));
        } else {
            assert(!(b & 0x80));
        }
    }

    uint8_t null_byte = 0;
    uint8_t a = null_byte - 0x01;
    uint8_t b = ~null_byte;

    assert(a & 0x80 && b & 0x80);
}
