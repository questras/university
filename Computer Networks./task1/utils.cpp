#include "utils.h"


RandomGenerator::RandomGenerator(seed_t seed) : current_number(seed) {}

uint32_t RandomGenerator::next() {
    auto result = uint32_t(this->current_number);
    this->current_number = (this->current_number * 279410273) % 4294967291;

    return result;
}

uint32_t calculate_crc32(const unsigned char *data, size_t data_size) {
    const unsigned char *p = data;
    uint32_t crc;

    crc = ~0U;
    while (data_size--) {
        crc = crc32_tab[(crc ^ *p++) & 0xFF] ^ (crc >> 8);
    }

    return crc ^ ~0U;
}
