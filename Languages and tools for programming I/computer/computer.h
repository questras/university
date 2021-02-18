/** @file
 * Rozwiązanie zadania computer.
 *
 * @author Krzysztof Rogowski 418382
 * @author Michał Wiśniewski 418486
 */

#ifndef COMPUTER_H
#define COMPUTER_H

#include <cstdint>
#include <string>
#include <type_traits>
#include <array>
#include <exception>
#include <limits>


using Id_t = uint64_t;

constexpr bool isId(const char* val, const char head, std::size_t size) {
    return ((head >= 'a' && head <= 'z') || (head >= 'A' && head <= 'Z') ||
        (head >= '0' && head <= '9')) &&
        (size == 0 || isId(val + 1, *val, size - 1));
}

constexpr bool isId(const char* val, std::size_t size) {
    return size > 0 && size <= 6 && isId(val + 1, *val, size - 1);
}

constexpr uint8_t upperCase(uint8_t val) {
    return (val >= 'a' && val <= 'z') ? (val - 'a' + 'A') : val;
}

constexpr Id_t calcId(const char* val, const char head, std::size_t size) {
    return upperCase(uint8_t(head)) + 256 *
        (size == 0 ? 0 : calcId(val + 1, *val, size - 1));
}

template<std::size_t N>
constexpr Id_t Id(const char(&val)[N]) {
    return isId(val, N - 1) ? calcId(val + 1, *val, N - 1) : throw std::exception();
}

// Pusta struktura, z której dziedziczą instrukcje.
struct Instruction {
};

// Struktura programu z instrukcjami.
template<typename ...Instructions>
struct Program {
};

// Literał numeryczny.
template<auto T>
struct Num {
    static_assert(std::is_integral_v<decltype(T)>, "Incorrect Num type.");
    constexpr static auto value = T;
};

// Pamięć Mem.
template<typename T>
struct Mem {
};

// Typy dwu-argumentowych instrukcji z przypisaniem.
enum class OpType {
    MOV,
    ADD,
    SUB,
    AND,
    OR,
};

// Szablon dwu-argumentowej instrukcji z przypisaniem.
template<typename Dst, typename Src, OpType Type>
struct BinaryAssignment : Instruction {
};

// Operacja kopiowania.
template<typename Dst, typename Src>
using Mov = BinaryAssignment<Dst, Src, OpType::MOV>;

// Operacja arytmetyczna dodawania z przypisaniem.
template<typename Dst, typename Src>
using Add = BinaryAssignment<Dst, Src, OpType::ADD>;

// Specjalizacja dodawania: dodawanie wartości 1.
template<typename Src>
using Inc = Add<Src, Num<1>>;

// Operacja arytmetyczna odejmowania z przypisaniem.
template<typename Dst, typename Src>
using Sub = BinaryAssignment<Dst, Src, OpType::SUB>;

// Specjalizacja odejmowania: odejmowanie wartości 1.
template<typename Dst>
using Dec = Sub<Dst, Num<1>>;

// Operacja logiczna bitowej operacji "and" z przypisaniem.
template<typename Dst, typename Src>
using And = BinaryAssignment<Dst, Src, OpType::AND>;

// Operacja logiczna bitowej operacji "or" z przypisaniem.
template<typename Dst, typename Src>
using Or = BinaryAssignment<Dst, Src, OpType::OR>;

// Operacja logiczna bitowej negacji z przypisaniem.
template<typename Arg>
struct Not : Instruction {
};

// Operacja porównania bez przypisania.
template<typename Arg1, typename Arg2>
struct Cmp : Instruction {
};

// Typy operacji skoku.
enum class JumpType {
    JMP,
    JZ,
    JS
};

// Szablon operacji skoku.
template<Id_t IdVal, JumpType Type>
struct Jump : Instruction {
};

// Skok bezwarunkowy do etykiety o identyfikatorze IdVal.
template<Id_t IdVal>
using Jmp = Jump<IdVal, JumpType::JMP>;

// Skok warunkowy do etykiety IdVal w przypadku gdy flaga ZF jest
// ustawiona na 1.
template<Id_t IdVal>
using Jz = Jump<IdVal, JumpType::JZ>;

// Skok warunkowy do etykiety IdVal w przypadku gdy flaga SF jest
// ustawiona na 1.
template<Id_t IdVal>
using Js = Jump<IdVal, JumpType::JS>;

// Oznaczenie etykiety przez IdVal.
template<Id_t IdVal>
struct Label : Instruction {
};

// Element zwracający wartość efektywnego adresu zmiennej IdVal.
template<Id_t IdVal>
struct Lea {
};

// Deklaracja zmiennej o identyfikatorze IdVal i wartości InitialVal.
template<Id_t IdVal, typename InitialVal>
struct D : Instruction {
};

// Szablon klasy zawierający wielkość pamięci i typ słowa reprezentujący
// podstawową jednostkę pamięci.
template<size_t MemorySize, typename MemoryType>
class Computer {
    using value_t = MemoryType;
    using memory_index_t = typename std::make_unsigned<value_t>::type;
    using memory_t = std::array<value_t, MemorySize>;
    using memoryMap_t = std::array<Id_t, MemorySize>;
    // Sprawdzenie czy rządana wielkość pamięci nie przekracza limitu typu
    static_assert(MemorySize <= std::numeric_limits<memory_index_t>::max());

    // Struktura przechowująca aktualny stan komputera.
    struct CompState {
        memory_t mem{ 0 };
        memoryMap_t memMap{ 0 };
        int ZF = 0;
        int SF = 0;
        Id_t jump = 0;

        // Ustawia flagę ZF (Zero Flag) komputera w zależności od value.
        constexpr void set_zero_flag(value_t value) {
            ZF = value == 0;
        }

        // Ustawia flagę SF (Sign Flag) komputera w zależności od value.
        constexpr void set_sign_flag(value_t value) {
            SF = value < 0;
        }
    };

    // Sprawdza czy value jest poza zakresem pamięci.
    constexpr static bool out_of_range_check(const memory_index_t value) {
        return value < 0 || value >= MemorySize;
    }

    // Pusty szablon programu w języku TMPAsm.
    template<typename P>
    struct TMPAsmProgram;

    // Szablon program w języku TMPAsm przyjmujący instrukcje w tym języku i
    // pozwalający na wykonanie podanych instrukcji.
    template<typename... Instructions>
    struct TMPAsmProgram<Program<Instructions...>> {
        // Wykonuje podane instrukcje i zwraca stan pamięci po
        // wykonaniu programu.
        constexpr static auto evaluate() {
            static_assert(((std::is_base_of<Instruction, Instructions>::value)
                && ... && true),
                "Error: a program should contain instructions only!");

            CompState state{};
            Initializer<Instructions...>::initialize(state, 0);
            do {
                Evaluator<Instructions...>::evaluate(state);
            } while (state.jump != 0);

            return state.mem;
        }
    };

    // Szablon struktury wydobywającej wartość podanej struktury.
    template<typename Src>
    struct ValueGetter {
        // Funkcja zwracająca wartość danej struktury: w tym przypadku
        // struktura nie jest poprawną p-wartością.
        constexpr static value_t get_val(const CompState&) {
            throw std::exception();
        }
    };

    // Szablon struktury wydobywającej wartość literału numerycznego.
    template<auto Val>
    struct ValueGetter<Num<Val>> {
        constexpr static value_t get_val(const CompState&) {
            return (value_t)Val;
        }
    };

    // Szablon struktury wydobywającej wartość z pamięci.
    template<typename Inner>
    struct ValueGetter<Mem<Inner>> {
        constexpr static value_t get_val(const CompState& state) {
            memory_index_t mem_place =
                (memory_index_t)ValueGetter<Inner>::get_val(state);

            if (out_of_range_check(mem_place)) {
                throw std::exception();
            }

            return state.mem.at(mem_place);
        }
    };

    // Szablon struktury wydobywającej wartość efektywnego adresu
    // zmiennej IdVal.
    template<Id_t IdVal>
    struct ValueGetter<Lea<IdVal>> {
        constexpr static value_t get_val(const CompState& state) {
            for (memory_index_t i = 0; i < state.memMap.size(); ++i) {
                if (state.memMap[i] == 0) {
                    // Aktualna i następne komórki pamięci nie są zainicjowane.
                    throw std::exception();
                }
                else if (state.memMap[i] == IdVal) {
                    // Znaleziono wartość efektywnego adresu zmiennej.
                    return i;
                }
            }

            // Nie znaleziono wartości efektywnego adresu zmiennej.
            throw std::exception();
        }
    };

    // Pusty szablon struktury inicjalizującej pamięć.
    template<typename... Instructions>
    struct Initializer {
        constexpr static void initialize(CompState&, memory_index_t) {};
    };

    // Szablon struktury inicjalizującej pamięć pomijający instrukcje,
    // które nie są deklaracjami zmiennych.
    template<typename I, typename... Rest>
    struct Initializer<I, Rest...> {
        constexpr static void initialize(CompState& state,
            memory_index_t last_mem_index) {
            Initializer<Rest...>::initialize(state, last_mem_index);
        }
    };

    // Szablon struktury inicjalizującej pamięć wyrzucający błąd,
    // przy błędnej instrukcji D
    template<Id_t IdVal, typename T, typename... Rest>
    struct Initializer<D<IdVal, T>, Rest...> {
        constexpr static void initialize(CompState&,
            memory_index_t) {
            throw std::exception();
        }
    };

    // Szablon struktury inicjalizującej pamięć zmienną o identyfikatorze
    // IdVal i wartości numerycznej InitVal.
    template<Id_t IdVal, auto InitVal, typename... Rest>
    struct Initializer<D<IdVal, Num<InitVal>>, Rest...> {
        constexpr static void initialize(CompState& state,
            memory_index_t last_mem_index) {
            // Przekroczenie dostępnej pamięci komputera spowoduje
            // rzucenie wyjątku przez funkcję std::array::at()
            state.mem.at(last_mem_index) = InitVal;
            state.memMap.at(last_mem_index) = IdVal;

            Initializer<Rest...>::initialize(state, last_mem_index + 1);
        }
    };

    // Szablon struktury wydobywającej wartość w danym miejscu w pamięci.
    template<typename Dsc>
    struct MemAddrGetter {
        // W tym przypadku struktura nie jest poprawną l-wartością.
        constexpr static value_t get_addr(const CompState&) {
            throw std::exception();
        }
    };

    // Szablon struktury wydobywającej wartość w danym miejscu w pamięci.
    template<typename Inner>
    struct MemAddrGetter<Mem<Inner>> {
        constexpr static memory_index_t get_addr(const CompState& state) {
            memory_index_t mem_place = ValueGetter<Inner>::get_val(state);

            if (out_of_range_check(mem_place)) {
                throw std::exception();
            }

            return mem_place;
        }
    };

    // Struktura wywołana na koniec wykonania z pustym programem.
    template<typename... Instructions>
    struct Evaluator {
        constexpr static void evaluate(CompState& state) {
            // Powiadomienie o nieobsłużonym skoku.
            if (state.jump != 0) {
                throw std::exception();
            }
        }
    };

    // Struktura odpowiadająca za wywołanie błędu w przypadku
    // nieobsługiwanej instrukcji.
    template<typename T, typename... Rest>
    struct Evaluator<T, Rest...> {
        constexpr static void evaluate(CompState&) {
            throw std::exception();
        }
    };

    // Struktura odpowiadająca za pominięcie instrukcji deklaracji zmiennych.
    template<Id_t IdVal, typename Src, typename... Rest>
    struct Evaluator<D<IdVal, Src>, Rest...> {
        constexpr static void evaluate(CompState& state) {
            Evaluator<Rest...>::evaluate(state);
        }
    };

    // Struktura odpowiadająca za obsłużenie instrukcji etykiety.
    template<Id_t IdVal, typename... Rest>
    struct Evaluator<Label<IdVal>, Rest...> {
        constexpr static void evaluate(CompState& state) {
            if (state.jump == IdVal) { // Etykieta znaleziona.
                state.jump = 0;
            }
            Evaluator<Rest...>::evaluate(state);
        }
    };

    // Struktura odpowiadająca za obsłużenie instrukcji skoku.
    template<Id_t IdVal, JumpType Type, typename... Rest>
    struct Evaluator<Jump<IdVal, Type>, Rest...> {
        constexpr static void evaluate(CompState& state) {
            if (state.jump == 0 &&
                ((Type == JumpType::JZ && state.ZF != 0) ||
                    (Type == JumpType::JS && state.SF != 0) ||
                    Type == JumpType::JMP)) {
                state.jump = IdVal;
            }
            else {
                Evaluator<Rest...>::evaluate(state);
            }
        }
    };

    // Struktura odpowiadająca za obsłużenie dwu-argumentowych instrukcji
    // przypisania.
    template<typename Dst, typename Src, typename... Rest, OpType Type>
    struct Evaluator<BinaryAssignment<Dst, Src, Type>, Rest...> {
        constexpr static void evaluate(CompState& state) {
            if (state.jump == 0) {
                const memory_index_t mem_place = MemAddrGetter<Dst>::get_addr(state);
                const value_t value = ValueGetter<Src>::get_val(state);
                switch (Type) {
                case OpType::MOV:
                    state.mem.at(mem_place) = value;
                    break;
                case OpType::ADD:
                    state.mem.at(mem_place) += value;
                    break;
                case OpType::SUB:
                    state.mem.at(mem_place) -= value;
                    break;
                case OpType::AND:
                    state.mem.at(mem_place) &= value;
                    break;
                case OpType::OR:
                    state.mem.at(mem_place) |= value;
                    break;
                }

                if (Type != OpType::MOV) {
                    state.set_zero_flag(state.mem.at(mem_place));
                    state.set_sign_flag(state.mem.at(mem_place));
                }
            }
            Evaluator<Rest...>::evaluate(state);
        }
    };

    // Struktura odpowiadająca za obsłużenie instrukcji logicznej "Not".
    template<typename Dst, typename... Rest>
    struct Evaluator<Not<Dst>, Rest...> {
        constexpr static void evaluate(CompState& state) {
            if (state.jump == 0) {
                const memory_index_t mem_place = MemAddrGetter<Dst>::get_addr(state);
                state.mem.at(mem_place) = ~state.mem.at(mem_place);

                state.set_zero_flag(state.mem.at(mem_place));
                state.set_sign_flag(state.mem.at(mem_place));
            }
            Evaluator<Rest...>::evaluate(state);
        }
    };

    // Struktura odpowiadająca za obsłużenie instrukcji logicznej "Cmp".
    template<typename Arg1, typename Arg2, typename... Rest>
    struct Evaluator<Cmp<Arg1, Arg2>, Rest...> {
        constexpr static void evaluate(CompState& state) {
            if (state.jump == 0) {
                const value_t value1 = ValueGetter<Arg1>::get_val(state);
                const value_t value2 = ValueGetter<Arg2>::get_val(state);
                const value_t result = value1 - value2;

                state.set_zero_flag(result);
                state.set_sign_flag(result);
            }
            Evaluator<Rest...>::evaluate(state);
        }
    };

public:
    // Funkcja wykonująca zadany program i zwracająca zawartość pamięci
    // po wykonaniu instrukcji w programie.
    template<typename Prog>
    constexpr static auto boot() {
        return TMPAsmProgram<Prog>::evaluate();
    }
};

#endif // COMPUTER_H
