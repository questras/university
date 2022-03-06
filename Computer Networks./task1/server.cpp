#include <sys/socket.h>
#include <netinet/in.h>
#include <string>
#include <poll.h>
#include <unistd.h>
#include <cstring>
#include <set>
#include <map>
#include <iostream>

#include "server.h"
#include "debug.h"
#include "constants.h"
#include "messages.h"
#include "events.h"

using std::map;
using std::set;
using std::string;
using std::cout;


Server::Server(port_t port, tur_speed_t turning_speed, rnd_speed_t rounds_per_sec,
               width_t board_width, height_t board_height, seed_t seed)
        : port(port), one_turn_time_ms(1000 / rounds_per_sec), game_started(false),
          game_manager(board_width, board_height, turning_speed, &random_generator),
          timers_manager(this->poll_sockets), messages_manager(this->poll_sockets),
          random_generator(seed) {}

void Server::loop() {
    this->init();

    int ret;
    while (true) {
        for (auto &poll_socket : poll_sockets) {
            poll_socket.revents = 0;
        }

        info("Start poll.");
        int timeout = -1;
        ret = poll(poll_sockets, sockets_num, timeout);

        if (ret == -1) {
            error("Error in poll.");
        }
        else {
            if (messages_manager.new_message()) {
                ClientMessage message = messages_manager.get_next_message();
                if (message.is_valid()) {
                    handle_message(message);
                }
            }
            for (const auto &player_id : timers_manager.get_timeout_players()) {
                disconnect(player_id);
            }
            if (time_to_start_game()) {
                start_game();
            }
            if (timers_manager.time_for_turn()) {
                next_turn();
            }
            if (game_started && game_manager.is_current_game_finished) {
                end_game();
            }
        }
    }

    if (close(poll_sockets[0].fd) == -1) {
        error("Close socket error.");
    }
}

void Server::init() {
    int sockfd;
    struct sockaddr_in6 server_address;

    sockfd = socket(AF_INET6, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        error("Message socket error.");
    }

    int mode = 0;
    setsockopt(sockfd, IPPROTO_IPV6, IPV6_V6ONLY, (void *) &mode, sizeof(mode));

    memset(&server_address, 0, sizeof(server_address));
    server_address.sin6_family = AF_INET6;
    server_address.sin6_port = htons(this->port);
    server_address.sin6_addr = in6addr_any;

    if (bind(sockfd, (struct sockaddr *) &server_address, sizeof(server_address)) < 0) {
        error("bind() error.");
    }

    // Initialize poll_sockets.
    for (auto &poll_socket : poll_sockets) {
        poll_socket.fd = -1;
        poll_socket.events = POLLIN;
        poll_socket.revents = 0;
    }
    poll_sockets[0].fd = sockfd;
}

void Server::disconnect(const player_id_t &player_id) {
    this->timers_manager.remove_timer(player_id);

    string name = id_to_player_info.at(player_id).name;

    if (!(name.empty())) {
        // Active player.
        name_to_id.erase(name);
        lobby.erase(name);
        ready.erase(name);
    }
    else {
        // Spectator.
        spectators.erase(player_id);
    }
    id_to_player_info.erase(player_id);
}

void Server::connect_player(const player_id_t &player_id, uint64_t session_id,
                            const string &name, uint8_t turn_direction) {
    this->timers_manager.add_timer(player_id);

    PlayerInfo player_info(name, session_id, turn_direction);
    id_to_player_info.emplace(player_id, player_info);
    if (name.empty()) {
        // Spectator.
        spectators.insert(player_id);
    }
    else {
        // Active player.
        name_to_id.emplace(name, player_id);
        if (turn_direction != TURN_DIRECTION_STRAIGHT) {
            ready.insert(name);
        }
        else {
            lobby.insert(name);
        }
    }
}

bool Server::time_to_start_game() {
    return !game_started && lobby.empty() && ready.size() > 1;
}

vector<player_id_t> Server::get_current_listeners() {
    // Firstly, all spectators are listeners.
    vector<player_id_t> listeners(spectators.begin(), spectators.end());

    // Secondly, all ready players are listeners.
    for (const auto &name : ready) {
        listeners.push_back(name_to_id[name]);
    }

    // Thirdly, all players in lobby are listeners.
    for (const auto &name : lobby) {
        listeners.push_back(name_to_id[name]);
    }

    return listeners;
}

void Server::start_game() {
    this->game_started = true;
    uint32_t game_id = this->random_generator.next();
    this->messages_manager.update_game_id(game_id);

    width_t maxx = game_manager.get_board_width();
    height_t maxy = game_manager.get_board_height();
    vector<string> ready_players(this->ready.begin(), this->ready.end());
    vector<player_id_t> listeners = this->get_current_listeners();
    vector<uint8_t> last_directions;
    for (const auto &player : ready_players) {
        last_directions.push_back(id_to_player_info.at(name_to_id[player]).last_turn_direction);
    }

    messages_manager.send_new_game_event(listeners, ready_players, maxx, maxy);
    vector<GameEventInfo> game_events = game_manager.initialize_new_game(game_id, ready_players, last_directions);
    messages_manager.send_new_events(listeners, game_events);
    timers_manager.start_turn_timer(this->one_turn_time_ms);
}

void Server::next_turn() {
    vector<GameEventInfo> game_events = game_manager.play_turn();
    messages_manager.send_new_events(this->get_current_listeners(), game_events);
}

void Server::end_game() {
    this->game_started = false;
    messages_manager.reset();
    timers_manager.remove_turn_timer();

    // Players from last game are now in lobby.
    this->lobby.insert(this->ready.begin(), this->ready.end());
    this->ready.clear();
}

void Server::handle_message(const ClientMessage &message) {
    player_id_t player_id = message.player_id;

    if (this->id_to_player_info.count(player_id) > 0) {
        // Already connected player.
        PlayerInfo player_info = this->id_to_player_info.at(player_id);

        if (message.session_id > player_info.session_id) {
            // New session id that is greater than before, reconnect user.
            this->disconnect(player_id);
            uint8_t turn_direction = message.turn_direction;
            if (this->game_started) {
                // Force user to enter lobby.
                turn_direction = TURN_DIRECTION_STRAIGHT;
            }
            this->connect_player(player_id, message.session_id,
                                 message.player_name, turn_direction);
        }
        else if (message.session_id == player_info.session_id) {
            if (message.player_name != player_info.name) {
                // Connected player but sent different name, ignore.
                return;
            }

            if (this->game_started && this->ready.count(message.player_name) > 0) {
                // Game started and it is currently playing player.
                this->game_manager.update_direction(message.player_name, message.turn_direction);
            }
            else if (!this->game_started &&
                     lobby.count(message.player_name) > 0 &&
                     message.turn_direction != TURN_DIRECTION_STRAIGHT) {
                // Player is ready for playing and game has not yet started.
                lobby.erase(message.player_name);
                ready.insert(message.player_name);
            }
        }
        else {
            // Connected player but lower session id than before, ignore.
            return;
        }
    }
    else {
        // New player.
        if (this->name_to_id.count(message.player_name) > 0) {
            // Player with such name already exists, ignore.
            return;
        }
        if (this->id_to_player_info.size() >= MAX_NUMBER_OF_PLAYERS) {
            // Number of players exceeded, do not connect.
            return;
        }

        this->connect_player(player_id, message.session_id,
                             message.player_name, message.turn_direction);
    }

    id_to_player_info.at(player_id).last_turn_direction = message.turn_direction;
    timers_manager.refresh_timer(player_id);
    // Send desired events to player.
    messages_manager.send_past_events(player_id, message.next_expected_event_no);
}
