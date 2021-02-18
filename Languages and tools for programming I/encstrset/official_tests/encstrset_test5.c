#include "../encstrset.h"

#ifdef NDEBUG
    #undef NDEBUG
#endif

#include <assert.h>

#define SIZEMAX 81
#define KEY "Lorem ipsum dolor sit amet, consectetur adipiscing elit."
#define NUMCAPITALS 26

int main() {
    unsigned long eset[SIZEMAX];
    static char value1[SIZEMAX];
    static char value2[SIZEMAX];

    for (unsigned int k = 1; k < SIZEMAX; k++) {
        eset[k] = encstrset_new();
        value1[k] = '\0';
        value1[k - 1] = 'A' + (k - 1) % NUMCAPITALS;
        assert(encstrset_insert(eset[k], value1, NULL));
        assert(encstrset_insert(eset[k], value1, KEY));
    }
    for (unsigned int k = 1; k < SIZEMAX; k++) {
        value2[k] = '\0';
        value2[k - 1] = 'A' + (k - 1) % NUMCAPITALS;
        assert(encstrset_test(eset[k], value2, NULL));
        assert(encstrset_test(eset[k], value2, KEY));
        assert(encstrset_remove(eset[k], value2, NULL));
        assert(encstrset_remove(eset[k], value2, KEY));
        encstrset_delete(eset[k]);
    }

    return 0;
}
