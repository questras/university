/** @file
 * Definicja modułu encstrset.
 *
 * @author Ignacy Makowski   418408
 * @author Michał Wiśniewski 418486
 */

#ifndef ENCSTRSET_H
#define ENCSTRSET_H

#ifdef __cplusplus
    #include <cstdlib>
    #include <iostream>
    extern "C" {
        namespace jnp1 {
#else
    #include <stdbool.h>
    #include <stdlib.h>
#endif

/**
* Tworzy nowy zbiór i zwraca jego identyfikator.
*
* @return identyfikator nowego zbioru.
*/
unsigned long encstrset_new();

/**
 * Jeżeli istnieje zbiór o identyfikatorze id, usuwa go, a w
 * przeciwnym przypadku nie robi nic.
 *
 * @param id - identyfikator zbioru do usunięcia.
 */
void encstrset_delete(unsigned long id);

/**
 * Jeżeli istnieje zbiór o identyfikatorze id, zwraca liczbę jego elementów,
 * a w przeciwnym przypadku zwraca 0.
 *
 * @param id - identyfikator zbioru.
 * @return liczba elementów zbioru lub 0, jeśli dany zbiór nie istnieje.
 */
size_t encstrset_size(unsigned long id);

/**
 * Jeżeli istnieje zbiór o identyfikatorze id i element value po
 * zaszyfrowaniu kluczem key nie należy do tego zbioru, to dodaje
 * ten zaszyfrowany element do zbioru, a w przeciwnym przypadku nie robi nic.
 * Szyfrowanie jest symetryczne, za pomocą operacji bitowej XOR.
 * Gdy klucz key jest krótszy od value, to jest cyklicznie powtórzony.
 *
 * @param id    - identyfikator zbioru do którego należy dodać wartość,
 * @param value - wartość do zaszyfrowania, a następnie dodania do zbioru,
 * @param key   - klucz do zaszyfrowania wartości.
 * @return true, gdy element został dodany, false w przeciwnym przypadku.
 */
bool encstrset_insert(unsigned long id, const char *value, const char *key);

/**
 * Jeżeli istnieje zbiór o identyfikatorze id i element value zaszyfrowany
 * kluczem key należy do tego zbioru, to usuwa element ze zbioru, a w
 * przeciwnym przypadku nie robi nic.
 *
 * @param id    - identyfikator zbioru z którego należy usunąć wartość,
 * @param value - wartość do zaszyfrowania, a następnie usunięcia ze zbioru,
 * @param key   - klucz do zaszyfrowania wartości.
 * @return true, gdy element został usunięty, a false w przeciwnym przypadku.
 */
bool encstrset_remove(unsigned long id, const char *value, const char *key);

/**
 * Jeżeli istnieje zbiór o identyfikatorze id i element value zaszyfrowany
 * kluczem key należy do tego zbioru, to zwraca true, a w przeciwnym
 * przypadku zwraca false.
 *
 * @param id    - identyfikator zbioru, w którym będzie sprawdzona wartość,
 * @param value - wartość do zaszyfrowania, a następnie sprawdzenia,
 * @param key   - klucz do zaszyfrowania wartości.
 * @return true jeśli zaszyfrowana wartość należy do danego zbioru,
 * inaczej false.
 */
bool encstrset_test(unsigned long id, const char *value, const char *key);

/**
 * Jeżeli istnieje zbiór o identyfikatorze id, usuwa wszystkie jego elementy,
 * a w przeciwnym przypadku nie robi nic.
 *
 * @param id - identyfikator zbioru, którego elementy będą usunięte.
 */
void encstrset_clear(unsigned long id);

/**
 * Jeżeli istnieją zbiory o identyfikatorach src_id oraz dst_id, to kopiuje
 * zawartość zbioru o identyfikatorze src_id do zbioru o identyfikatorze
 * dst_id, a w przeciwnym przypadku nic nie robi.

 * @param src_id    - identyfikator zbioru, z którego wartości będą kopiowane,
 * @param dst_id    - identyfikator zbioru, do którego wartości będą kopiowane.
 */
void encstrset_copy(unsigned long src_id, unsigned long dst_id);


#ifdef __cplusplus
        }
    }
#endif

#endif
