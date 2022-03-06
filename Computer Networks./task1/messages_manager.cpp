#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>

#include "messages_manager.h"
#include "debug.h"


MessagesManager::MessagesManager(struct pollfd *poll_sockets_ptr)
        : poll_sockets_ptr(poll_sockets_ptr), game_id(-1) {}

bool MessagesManager::new_message() {
    return this->poll_sockets_ptr[this->message_socket_index].revents & POLLIN;
}

ClientMessage MessagesManager::get_next_message() {
    struct sockaddr_storage client_address;
    unsigned char buf[100];
    int flags = 0;
    socklen_t client_len = (socklen_t) sizeof(client_address);

    ssize_t msg_len = recvfrom(this->poll_sockets_ptr[this->message_socket_index].fd,
                               buf, sizeof(buf), flags,
                               (struct sockaddr *) &client_address, &client_len);

    if (msg_len < 0) {
        warning("Negative length of read message.");
        return ClientMessage();
    }
    else if (msg_len > MAX_CLIENT_DATAGRAM_SIZE) {
        warning("Client datagram size too big.");
        return ClientMessage();
    }
    else if (msg_len < MIN_CLIENT_DATAGRAM_SIZE) {
        warning("Client datagram size too small.");
        return ClientMessage();
    }

    info("Received data from client.");
    char host[NI_MAXHOST], port[NI_MAXSERV];
    getnameinfo((struct sockaddr *) &client_address, client_len,
                host, sizeof(host), port, sizeof(port), NI_NUMERICHOST | NI_NUMERICSERV);
    string client_host(host);
    port_t client_port = stoul(string(port));

    ClientMessage message;
    ClientMessage::deserialize(buf, msg_len, client_host, client_port, message);

    return message;
}

void MessagesManager::send_past_events(player_id_t player_id, uint32_t starting_event_no) {
    vector<Event> to_send;
    size_t data_size = 0;
    unsigned char buffer[MAX_SERVER_DATAGRAM_SIZE];

    for (uint32_t i = starting_event_no; i < this->events_history.size(); i++) {
        if (data_size + this->events_history[i].size() <= MAX_EVENTS_SIZE) {
            data_size += this->events_history[i].size();
            to_send.push_back(this->events_history[i]);
        }
        else {
            ServerMessage server_message(this->game_id, to_send);
            server_message.serialize(buffer);
            this->send_message(player_id, buffer, data_size + sizeof(server_message.game_id));

            to_send.clear();
            to_send.push_back(this->events_history[i]);
            data_size = this->events_history[i].size();
        }
    }

    if (!to_send.empty()) {
        ServerMessage server_message(this->game_id, to_send);
        server_message.serialize(buffer);
        this->send_message(player_id, buffer, data_size + sizeof(server_message.game_id));
    }
}

bool MessagesManager::send_message(player_id_t player_id, unsigned char *buffer, size_t data_size) {
    struct sockaddr_in6 client_address;
    client_address.sin6_family = AF_INET6;
    inet_pton(AF_INET6, player_id.first.c_str(), &(client_address.sin6_addr));
    client_address.sin6_port = htons(player_id.second);
    socklen_t client_len = (socklen_t) sizeof(client_address);
    int flags = 0;

    ssize_t send_len = sendto(this->poll_sockets_ptr[this->message_socket_index].fd, buffer,
                              data_size, flags, (struct sockaddr *) &client_address,
                              client_len);

    if (send_len < 0) {
        warning("An error occurred when sending message.");
        return false;
    }
    if ((size_t) send_len != data_size) {
        warning("Length of sendto doesn't equal data size.");
        return false;
    }

    return true;
}

void MessagesManager::send_new_events(const vector<player_id_t> &players_ids,
                                      const vector<GameEventInfo> &game_events) {
    uint32_t first_new_event_no = this->events_history.size();
    for (GameEventInfo game_event : game_events) {
        uint32_t next_event_no = this->events_history.size();
        Event e(0, 0, 0, nullptr);

        if (game_event.event_type == EVENT_TYPE_PIXEL) {
            e = EventFactory::CreatePixelEvent(next_event_no, game_event.player_number,
                                               game_event.x, game_event.y);
        }
        else if (game_event.event_type == EVENT_TYPE_PLAYER_ELIMINATED) {
            e = EventFactory::CreatePlayerEliminatedEvent(next_event_no, game_event.player_number);
        }
        else if (game_event.event_type == EVENT_TYPE_GAME_OVER) {
            e = EventFactory::CreateGameOverEvent(next_event_no);
        }
        else {
            // Invalid event type.
            continue;
        }

        this->events_history.push_back(e);
    }

    // Send new events to given players.
    for (const auto &player_id : players_ids) {
        send_past_events(player_id, first_new_event_no);
    }
}

void MessagesManager::send_new_game_event(vector<player_id_t> players_ids,
                                          vector<string> players,
                                          uint32_t maxx,
                                          uint32_t maxy) {

    uint32_t new_game_event_no = this->events_history.size();
    Event e = EventFactory::CreateNewGameEvent(new_game_event_no, maxx, maxy, players);
    this->events_history.push_back(e);

    for (const auto &player_id : players_ids) {
        // This will only send new game event because there are no
        // events added after new game event.
        send_past_events(player_id, new_game_event_no);
    }
}

void MessagesManager::reset() {
    this->events_history.clear();
    this->game_id = -1;
}

void MessagesManager::update_game_id(uint32_t new_game_id) {
    this->game_id = new_game_id;
}
