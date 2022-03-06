#ifndef CURVE_FEVER_UTILS_H
#define CURVE_FEVER_UTILS_H

#include <cstdint>

#include "constants.h"


class RandomGenerator {
    uint64_t current_number;
public:

    explicit RandomGenerator(seed_t seed);

    uint32_t next();
};

uint32_t calculate_crc32(const unsigned char *data, size_t data_size);

#endif //CURVE_FEVER_UTILS_H
