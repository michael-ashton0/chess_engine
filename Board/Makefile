CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -O2 -Iinclude/

TARGET = 1ply

SRCS = \
	src/main.cpp \
	src/board/board.cpp \
	src/board/move.cpp \
	src/pieces/pawn.cpp \
	src/pieces/precomputed/knight.cpp \
	src/pieces/precomputed/king.cpp \
	src/pieces/sliders/rook.cpp \
	src/pieces/sliders/bishop.cpp \
	src/pieces/sliders/queen.cpp \
	src/perft.cpp \
	src/nps.cpp \
	src/uci.cpp \
	src/player/search.cpp \
	src/player/evaluation.cpp \
	src/player/selfPlay.cpp \

OBJS = $(SRCS:.cpp=.o)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $(TARGET)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(OBJS) $(TARGET)