#include "validators.h"
#include "constants.h"
#include "debug.h"


bool validate_turn_direction(uint8_t turn_direction) {
    return turn_direction == TURN_DIRECTION_STRAIGHT ||
           turn_direction == TURN_DIRECTION_RIGHT ||
           turn_direction == TURN_DIRECTION_LEFT;
}

bool validate_player_name(string name) {
    if (name.size() > PLAYER_NAME_MAX_LENGTH) {
        return false;
    }

    for (auto c : name) {
        uint8_t ascii = c;

        if (ascii < MIN_NAME_ASCII || ascii > MAX_NAME_ASCII) {
            return false;
        }
    }

    return true;
}

bool validate_port(validated_t port) {
    return port >= MIN_SERVER_PORT && port <= MAX_SERVER_PORT;
}

bool validate_seed(validated_t seed) {
    return seed >= MIN_SEED && seed <= MAX_SEED;
}

bool validate_width(validated_t width) {
    return width >= MIN_BOARD_WIDTH && width <= MAX_BOARD_WIDTH;
}

bool validate_height(validated_t height) {
    return height >= MIN_BOARD_HEIGHT && height <= MAX_BOARD_HEIGHT;
}

bool validate_turning_speed(validated_t speed) {
    return speed >= MIN_TURNING_SPEED && speed <= MAX_TURNING_SPEED;
}

bool validate_rounds_per_sec(validated_t rounds_per_sec) {
    return rounds_per_sec >= MIN_ROUNDS_PER_SEC && rounds_per_sec <= MAX_ROUNDS_PER_SEC;
}

void validate_server_args(validated_t port, validated_t seed, validated_t width,
                          validated_t height, validated_t turning_speed,
                          validated_t rounds_per_sec) {
    if (!validate_port(port)) {
        error("Invalid port.");
    }
    if (!validate_seed(seed)) {
        error("Invalid seed.");
    }
    if (!validate_turning_speed(turning_speed)) {
        error("Invalid turning_speed.");
    }
    if (!validate_rounds_per_sec(rounds_per_sec)) {
        error("Invalid rounds_per_sec.");
    }
    if (!validate_width(width)) {
        error("Invalid width.");
    }
    if (!validate_height(height)) {
        error("Invalid height.");
    }
}