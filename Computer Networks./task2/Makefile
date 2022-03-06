FLAGS = -Wall -Wextra -O2 -std=c++17
END_FLAGS = -lstdc++fs

all: serwer

serwer: message.o log.o parsing.o correlated_server.o server.o
	g++ $(FLAGS) -o serwer $^ $(END_FLAGS)

message.o: message.cpp message.h
	g++ $(FLAGS) -c $< $(END_FLAGS)

log.o: log.cpp log.h
	g++ $(FLAGS) -c $< $(END_FLAGS)

parsing.o: parsing.cpp parsing.h log.h constants.h
	g++ $(FLAGS) -c $< $(END_FLAGS)

correlated_server.o: correlated_server.cpp correlated_server.h
	g++ $(FLAGS) -c $< $(END_FLAGS)

server.o: server.cpp parsing.h log.h message.h correlated_server.h
	g++ $(FLAGS) -c $< $(END_FLAGS)

clean:
	@rm -f *.o serwer
