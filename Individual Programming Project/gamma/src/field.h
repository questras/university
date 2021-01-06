/** @file
 * Interfejs modułu opisującego pojedyńcze pole w grze
 * i operacje związane z grupowaniem pól w zbiory.
 *
 * @author Michał Wiśniewski 418486
 * @date 14.04.2020
 */

#ifndef GAMMA_FIELD_H
#define GAMMA_FIELD_H

#include <stdint.h>
#include <stdbool.h>


/**
 * Struktura przechowująca informacje o pojedyńczym polu w grze.
 */
struct field {
    /**
     * Właściciel pola.
     */
    uint32_t owner;

    /**
     * Rodzic pola, skierowany na reprezentanta pola.
     */
    struct field *parent;
};

/**
 * Typ przechowujący wskaźnik na pole.
 */
typedef struct field* Field;


/** @brief Zwraca reprezentanta pola.
 * Zwraca reprezentanta pola @p x i kompresuje ścieżkę pola
 * @p x do niego, ustawiając go jako swojego bezpośredniego rodzica.
 * @param[in] x   – pole, którego reprezentant ma być zwrócony.
 * @return Reprezentant pola @p x.
 */
Field find_representative(Field x);

/** @brief Łączy zbiór jednego pola ze zbiorem drugiego pola.
 * Łączy zbiór do którego należy pole @p x ze zbiorem, do którego
 * należy pole @p y i ustawia im wspólnego reprezentanta.
 * @param[in] x   – pole, należące do zbioru, który ma być połączony,
 * @param[in] y   – pole, należące do zbioru, który ma być połączony.
 * @return Wartość @p true, jeśli @p x i @p y mieli różnych reprezentantów i
 *        ich zbiory zostały połączone lub wartość @p false jeśli @p x i @p y
 *        mają tych samych reprezentantów.
*/
bool union_fields(Field x, Field y);

#endif //GAMMA_FIELD_H
