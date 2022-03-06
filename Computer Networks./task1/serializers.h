#ifndef CURVE_FEVER_SERIALIZERS_H
#define CURVE_FEVER_SERIALIZERS_H

#include <cstdint>
#include <netinet/in.h>
#include <cstring>
#include <string>


unsigned char *serialize_uint32(unsigned char *buf, uint32_t numb);

unsigned char *deserialize_uint32(unsigned char *buf, uint32_t &result);

unsigned char *deserialize_uint64(unsigned char *buf, uint64_t &result);

unsigned char *serialize_uint8(unsigned char *buf, uint8_t numb);

unsigned char *deserialize_uint8(unsigned char *buf, uint8_t &numb);

unsigned char *serialize_string(unsigned char *buf, const std::string& str);

#endif //CURVE_FEVER_SERIALIZERS_H
