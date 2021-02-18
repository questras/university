#include "../encstrset.h"

int main() {
    unsigned long eset;
    static char value[] = "Frodo Baggins";
    static char key[] = "the ring";

    eset = encstrset_new();
    encstrset_insert(eset, value, NULL);
    encstrset_insert(eset, value, key);
    value[5] = '\t';
    encstrset_insert(eset, value, NULL);
    encstrset_insert(eset, value, key);
    value[5] = '\n';
    key[3] = '\b';
    encstrset_insert(eset, value, key);
    value[5] = '\x7f';
    key[3] = '\a';
    encstrset_insert(eset, value, key);
    encstrset_delete(eset);

    return 0;
}
