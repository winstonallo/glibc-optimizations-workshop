#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#ifndef FAST
#define FAST 1
#endif

#if FAST == 0
size_t
ft_strlen(const char *str) {
    char *it = (char *)str;

    while (*it) {
        it++;
    }

    return it - str;
}

void
ft_memset(void *dest, uint8_t val, size_t n_bytes) {
    for (int idx = 0; idx < n_bytes; ++idx) {
        ((uint8_t *)dest)[idx] = val;
    }
}
#else

uint64_t
ft_strlen(const char *str) {
    const char *char_ptr;

    for (char_ptr = str; ((size_t)char_ptr % __SIZEOF_POINTER__) != 0; ++char_ptr) {

        if (*char_ptr == '\0') {
            return char_ptr - str;
        }
    }

    const size_t *size_t_ptr = (size_t *)char_ptr;

    size_t himagic = 0x80808080L;
    size_t lomagic = 0x01010101L;

#if __SIZEOF_POINTER__ == 8
    himagic = ((himagic << 16) << 16) | himagic;
    lomagic = ((lomagic << 16) << 16) | lomagic;
#endif

    for (;;) {
        size_t lw = *size_t_ptr++;

        if (((lw - lomagic) & ~lw & himagic) != 0) {
            const char *char_block_ptr = (const char *)(size_t_ptr - 1);

            if (char_block_ptr[0] == '\0') {
                return char_block_ptr - str;
            } else if (char_block_ptr[1] == '\0') {
                return char_block_ptr + 1 - str;
            } else if (char_block_ptr[2] == '\0') {
                return char_block_ptr + 2 - str;
            } else if (char_block_ptr[3] == '\0') {
                return char_block_ptr + 3 - str;
            }

#if __SIZEOF_POINTER__ == 8
            if (char_block_ptr[4] == '\0') {
                return char_block_ptr + 4 - str;
            } else if (char_block_ptr[5] == '\0') {
                return char_block_ptr + 5 - str;
            } else if (char_block_ptr[6] == '\0') {
                return char_block_ptr + 6 - str;
            } else if (char_block_ptr[7] == '\0') {
                return char_block_ptr + 7 - str;
            }
#endif
        }
    }
}

void *
ft_memset(void *dest, int c, uint64_t len) {
    long dst = (long)dest;

    if (len >= 8) {

        size_t C = (uint8_t)c;
        C |= C << 8;
        C |= C << 16;
        if (__SIZEOF_POINTER__ == 8) {
            C |= (C << 16) << 16;
        }

        while (dst % __SIZEOF_POINTER__ != 0) {
            ((uint8_t *)dst)[0] = c;
            ++dst;
            --len;
        }

        uint64_t n_blocks = len / (__SIZEOF_POINTER__ * 8);
        while (n_blocks > 0) {
            ((size_t *)dst)[0] = C;
            ((size_t *)dst)[1] = C;
            ((size_t *)dst)[2] = C;
            ((size_t *)dst)[3] = C;
            ((size_t *)dst)[4] = C;
            ((size_t *)dst)[5] = C;
            ((size_t *)dst)[6] = C;
            ((size_t *)dst)[7] = C;

            dst += __SIZEOF_POINTER__ * 8;
            --n_blocks;
        }

        len %= __SIZEOF_POINTER__ * 8;

        uint64_t n_remaining_u64 = len / __SIZEOF_POINTER__;
        while (n_remaining_u64 > 0) {
            ((size_t *)dst)[0] = C;
            dst += __SIZEOF_POINTER__;
            --n_remaining_u64;
        }

        len %= __SIZEOF_POINTER__;
    }

    while (len > 0) {
        ((uint8_t *)dst)[0] = c;
        ++dst;
        --len;
    }

    return dest;
}
#endif

int
main() {
    clock_t start, end;
    double time_ms;

    size_t size = INT32_MAX;

    char *str = malloc(size * sizeof(char));
    if (!str) return EXIT_FAILURE;

    start = clock();
    ft_memset(str, 0x42, size);
    end = clock();
    time_ms = ((double)(end - start) / CLOCKS_PER_SEC) * 1000;
    printf("ft_memset execution time for %lu bytes: %f ms\n", size, time_ms);

    str[size - 1] = '\0';

    start = clock();
    size_t len = ft_strlen(str);
    end = clock();
    time_ms = ((double)(end - start) / CLOCKS_PER_SEC) * 1000;
    printf("ft_strlen execution time for a string of length %lu:: %f ms\n", len, time_ms);

    free(str);
}
