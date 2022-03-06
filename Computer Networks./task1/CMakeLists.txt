cmake_minimum_required(VERSION 3.19)
project(curve_fever)

set(CMAKE_CXX_STANDARD 17)

add_executable(screen-worms-server server_main.cpp constants.h utils.cpp utils.h server.cpp server.h worm.cpp worm.h board.cpp board.h debug.cpp debug.h messages.cpp messages.h player.h events.cpp events.h serializers.cpp serializers.h game_event_info.h messages_manager.cpp messages_manager.h game_manager.cpp game_manager.h timers_manager.cpp timers_manager.h validators.cpp validators.h)
