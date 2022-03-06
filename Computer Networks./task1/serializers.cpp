#include "serializers.h"


unsigned char *serialize_uint32(unsigned char *buf, uint32_t numb) {
    uint32_t be_numb = htonl(numb);
    memcpy(buf, &be_numb, sizeof(be_numb));

    return buf + sizeof(be_numb);
}

unsigned char *deserialize_uint32(unsigned char *buf, uint32_t &result) {
    memcpy(&result, buf, sizeof(result));
    result = ntohl(result);

    return buf + sizeof(result);
}

unsigned char *deserialize_uint64(unsigned char *buf, uint64_t &result) {
    memcpy(&result, buf, sizeof(result));
    result = be64toh(result);

    return buf + sizeof(result);
}

unsigned char *serialize_uint8(unsigned char *buf, uint8_t numb) {
    buf[0] = numb;

    return buf + 1;
}

unsigned char *deserialize_uint8(unsigned char *buf, uint8_t &numb) {
    numb = buf[0];

    return buf + 1;
}

unsigned char *serialize_string(unsigned char *buf, const std::string &str) {
    size_t size = str.size() + 1; // + 1 because of '\0' at the end.
    memcpy(buf, str.c_str(), size);

    return buf + size;
}
