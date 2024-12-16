uint64_t
ft_strlen(const char *str) {
    const char *char_ptr;

    // Process `str` byte-by-byte until `char_ptr` is aligned to a word boundary.
    //
    // Why does the pointer need to be aligned?
    // - CPUs fetch memory in chunks (32 or 64 bits) rather than byte-by-byte.
    // - Starting at an unaligned address may cause additional overhead or slower fetches, because
    //   the CPU could have to fetch twice for the same word.
    //
    // We iterate over the input string one byte at a time until `char_ptr` points
    // to a memory address that is a multiple of `OPTSIZ` (the word size). Once we know it is aligned,
    // we can process the string more efficiently.
    for (char_ptr = str; ((op_t)char_ptr % OPTSIZ) != 0; ++char_ptr) {

        // If we find a null byte before aligment (aka the string length is less than `OPTSIZ - 1`),
        // we return the length.
        if (*char_ptr == '\0') {
            return char_ptr - str;
        }
    }

    const op_t *op_t_ptr = (op_t *)char_ptr;

    // All MSB set.
    op_t himagic = 0x80808080L;
    // All LSB set.
    op_t lomagic = 0x01010101L;

#if __SIZEOF_POINTER__ == 8
    // 64-bit architecture, extend magic numbers.
    himagic = ((himagic << 16) << 16) | himagic;
    lomagic = ((lomagic << 16) << 16) | lomagic;
#endif

    for (;;) {
        op_t lw = *op_t_ptr++;

        // This condition is the magic behind the optimization. It detects NULL bytes in 32/64-bit
        // words (depending on the CPU architecture). Instead of checking each byte individually, this
        // approach allows scanning multiple bytes (word-sized chunks) at once, significantly
        // reducing the number of checks needed.
        //
        // Step-by-step explanation:
        // 1. `(lw - lomagic)`
        //    - `lomagic` is `0x01010101...`, where each byte is `0x01`.
        //    - Subtracting `lomagic` from `lw` decreases each byte in `lw` by 1.
        //    - If a byte in `lw` was `0x00`, it underflows to `0xFF`.
        //
        // 2. `~lw`
        //    - Bitwise NOT of `lw`. Bytes with `0x00` in `lw` become `0xFF`.
        //
        // 3. `& ~lw & himagic`
        //    - `himagic` is `0x80808080...`, where the most significant bit (MSB) of each byte
        //      is set.
        //    - This step isolates bytes in `lw` where:
        //      a. Subtraction resulted in `0xFF` (from the null byte in `lw`).
        //      b. The MSB is set due to the underflow.
        //    - When a match is found, the condition evaluates to true.
        //
        // If this condition evaluates to true, we know a null byte exists in the current word. To find
        // its exact position, we iterate over it until we find the null byte.
        //
        // A written explanation can only do so much - if you still do not understand what is going on here,
        // take a pen and paper and calculate it by hand a few times, with numbers that do/do not contain null bytes.
        // It will help you see the pattern.
        if (((lw - lomagic) & ~lw & himagic) != 0) {
            const char *char_block_ptr = (const char *)(op_t_ptr - 1);

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
