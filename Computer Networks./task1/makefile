FLAGS = -Wall -Wextra -O2 -std=c++17

all: screen-worms-server

screen-worms-server: board.o debug.o events.o game_manager.o messages.o messages_manager.o serializers.o \
					 server.o server_main.o timers_manager.o utils.o validators.o worm.o
	g++ $(FLAGS) -o screen-worms-server $^

board.o: board.cpp board.h constants.h
	g++ $(FLAGS) -c $<

debug.o: debug.cpp debug.h
	g++ $(FLAGS) -c $<

events.o: events.cpp events.h constants.h serializers.h utils.h
	g++ $(FLAGS) -c $<

game_manager.o: game_manager.cpp game_manager.h utils.h board.h worm.h game_manager.h constants.h
	g++ $(FLAGS) -c $<

messages.o: messages.cpp messages.h events.h constants.h serializers.h validators.h
	g++ $(FLAGS) -c $<

messages_manager.o: messages_manager.cpp messages_manager.h game_event_info.h constants.h events.h messages.h debug.h
	g++ $(FLAGS) -c $<

serializers.o: serializers.cpp serializers.h
	g++ $(FLAGS) -c $<

server.o: server.cpp server.h player.h constants.h worm.h board.h game_manager.h \
		  timers_manager.h messages.h utils.h debug.h events.h
	g++ $(FLAGS) -c $<

server_main.o: server_main.cpp constants.h server.h debug.h validators.h
	g++ $(FLAGS) -c $<

timers_manager.o: timers_manager.cpp timers_manager.h constants.h debug.h
	g++ $(FLAGS) -c $<

utils.o: utils.cpp utils.h constants.h
	g++ $(FLAGS) -c $<

validators.o: validators.cpp validators.h constants.h debug.h
	g++ $(FLAGS) -c $<

worm.o: worm.cpp worm.h constants.h
	g++ $(FLAGS) -c $<

clean:
	@rm -f *.o screen-worms-server