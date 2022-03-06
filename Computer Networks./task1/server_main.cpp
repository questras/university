#include <unistd.h>

#include "constants.h"
#include "server.h"
#include "debug.h"
#include "validators.h"


int main(int argc, char **argv) {
    long long port = DEFAULT_SERVER_PORT,
            turning_speed = DEFAULT_TURNING_SPEED,
            rounds_per_sec = DEFAULT_ROUNDS_PER_SEC,
            width = DEFAULT_BOARD_WIDTH,
            height = DEFAULT_BOARD_HEIGHT,
            seed = (seed_t) (time(NULL));

    int opt;
    while ((opt = getopt(argc, argv, "p:s:t:v:w:h:")) != EOF) {
        switch (opt) {
            case 'p':
                port = strtoll(optarg, nullptr, 10);
                break;
            case 's':
                seed = strtoll(optarg, nullptr, 10);
                break;
            case 't':
                turning_speed = strtoll(optarg, nullptr, 10);
                break;
            case 'v':
                rounds_per_sec = strtoll(optarg, nullptr, 10);
                break;
            case 'w':
                width = strtoll(optarg, nullptr, 10);
                break;
            case 'h':
                height = strtoll(optarg, nullptr, 10);
                break;
            case '?':
                error("Undefined option.");
                break;
            default:
                error("'default' case in getopt.");
                break;
        }
    }
    if (optind < argc) {
        error("Bad argument found.");
    }

    info("Server argument 'port' is equal to:");
    info(std::to_string(port));
    info("Server argument 'seed' is equal to:");
    info(std::to_string(seed));
    info("Server argument 'turning_speed' is equal to:");
    info(std::to_string(turning_speed));
    info("Server argument 'rounds_per_sec' is equal to:");
    info(std::to_string(rounds_per_sec));
    info("Server argument 'width' is equal to:");
    info(std::to_string(width));
    info("Server argument 'height' is equal to:");
    info(std::to_string(height));

    validate_server_args(port, seed, width, height, turning_speed, rounds_per_sec);

    Server server((port_t) port, (tur_speed_t) turning_speed, (rnd_speed_t) rounds_per_sec,
                  (width_t) width, (height_t) height, (seed_t) seed);
    server.loop();

    return 0;
}