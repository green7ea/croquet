CXXFLAGS += -MD -pipe -Wall -std=c++11 `pkg-config --cflags sdl glu`
LDFLAGS = `pkg-config --libs sdl glu`

SRCS = arrow.cpp audio.cpp ball.cpp camera.cpp collision.cpp error.cpp game.cpp	\
       main.cpp mallet.cpp matrix.cpp nodecamera.cpp node.cpp										\
       nodelookingcamera.cpp peg.cpp quaternion.cpp ring.cpp rk4.cpp shader.cpp	\
       shadow.cpp surface.cpp texture.cpp vector.cpp view.cpp wall.cpp
OBJS = $(SRCS:.cpp=.o)
DEPS = $(SRCS:.cpp=.d)
OUTPUT = ../bin/phs4700-crocket

.PHONY: all clean

all: $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $(OUTPUT) $(LDFLAGS)

clean:
	rm -f $(OBJS) $(DEPS) $(OUTPUT)

-include $(DEPS)
