#ifndef CURVE_FEVER_VALIDATORS_H
#define CURVE_FEVER_VALIDATORS_H

#include <string>

using std::string;
using validated_t = long long;


bool validate_turn_direction(uint8_t turn_direction);

bool validate_player_name(string name);

bool validate_port(validated_t port);

bool validate_seed(validated_t seed);

bool validate_width(validated_t width);

bool validate_height(validated_t height);

bool validate_turning_speed(validated_t speed);

bool validate_rounds_per_sec(validated_t rounds_per_sec);

void validate_server_args(validated_t port, validated_t seed, validated_t width,
                          validated_t height, validated_t turning_speed,
                          validated_t rounds_per_sec);

#endif //CURVE_FEVER_VALIDATORS_H
