/** @file
 * Implementacja modułu field.h
 *
 * @author Michał Wiśniewski 418486
 * @date 14.04.2020
 */

#include "field.h"


Field find_representative(Field x) {
    if (x->parent != x) {
        x->parent = find_representative(x->parent);
    }

    return x->parent;
}

bool union_fields(Field x, Field y) {
    Field x_parent = find_representative(x);
    Field y_parent = find_representative(y);

    if (x_parent == y_parent) {
        return false;
    }

    x_parent->parent = y_parent;
    return true;
}