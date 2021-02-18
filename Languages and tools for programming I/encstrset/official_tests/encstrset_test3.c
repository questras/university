#include "../encstrset.h"

#ifdef NDEBUG
    #undef NDEBUG
#endif

#include <assert.h>

int main() {
    unsigned long eset1 = encstrset_new();
    unsigned long eset2 = encstrset_new();
    unsigned long eset3 = encstrset_new();

    assert(encstrset_insert(eset1, "7777777", "72wW4u"));
    assert(encstrset_insert(eset1, "eset1", "eset1"));
    assert(encstrset_insert(eset2, "72wW4u", "7777777"));
    assert(encstrset_insert(eset2, "eset2", "eset2"));
    assert(!encstrset_insert(eset2, "eset2", "eset2"));
    encstrset_copy(eset2, eset1);
    assert(encstrset_size(eset1) == 3);
    assert(encstrset_remove(eset2, "72wW4u", "7"));
    assert(encstrset_remove(eset2, "eset2", "eset2"));
    assert(!encstrset_test(eset2, "eset2", "eset2"));
    encstrset_delete(eset2);
    encstrset_delete(eset3);
    encstrset_delete(eset3);
    encstrset_copy(eset3, eset2);
    encstrset_delete(eset2);
    assert(!encstrset_test(eset3, "NULL", NULL));
    assert(!encstrset_test(eset3, NULL, "NULL"));
    assert(!encstrset_test(eset3, NULL, NULL));
    assert(encstrset_insert(eset1, "", "abc"));
    assert(encstrset_insert(eset1, "abc", ""));
    assert(encstrset_test(eset1, "", "abc"));
    assert(encstrset_test(eset1, "abc", ""));
    assert(encstrset_remove(eset1, "", "abc"));
    assert(encstrset_remove(eset1, "abc", ""));
    encstrset_copy(eset1, eset2);
    encstrset_copy(eset2, eset1);
    encstrset_delete(eset1);
    assert(!encstrset_insert(eset1, "a", "b"));
    assert(!encstrset_test(eset1, "a", "b"));
    assert(!encstrset_remove(eset1, "a", "b"));
    encstrset_copy(eset1, eset2);
    assert(!encstrset_test(eset1, NULL, ""));

    return 0;
}
