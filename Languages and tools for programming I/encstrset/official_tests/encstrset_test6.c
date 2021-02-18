#include "../encstrset.h"

#ifdef NDEBUG
    #undef NDEBUG
#endif

#include <assert.h>

#define NMIN 4321
#define NMAX 8765
#define NUMDIGITS 4

static void ndig_uint2str(char *str, unsigned int n) {
    for (unsigned int k = 0; k < NUMDIGITS; k++) {
        str[NUMDIGITS - k - 1] = '0' + n % 10;
        n /= 10;
    }
}

int main() {
    static char value[NUMDIGITS + 1];
    static char key[NUMDIGITS + 1];
    value[NUMDIGITS] = '\0';
    key[NUMDIGITS] = '\0';
    unsigned long es1, es2;

    es1 = encstrset_new();
    for (unsigned int j = NMAX; j >= NMIN; j--) {
        ndig_uint2str(key, j);
        for (unsigned int i = NMIN; i <= NMAX; i++) {
            ndig_uint2str(value, i);
            encstrset_insert(es1, value, key);
        }
    }
    es2 = encstrset_new();
    encstrset_copy(es1, es2);
    assert(encstrset_size(es1) == encstrset_size(es2));
    encstrset_clear(es1);
    assert(encstrset_size(es1) == 0);
    encstrset_delete(es1);
    encstrset_delete(es2);

    return 0;
}
