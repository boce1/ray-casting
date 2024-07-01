CXX := g++

SRCS := main.cpp line.cpp
OBJS := $(SRCS:.cpp=.o)

all: link
	
link: $(OBJS)
	$(CXX) -o main $(OBJS) -L src/lib -lmingw32 -lSDL2main -lSDL2 

%.o : %.cpp
	$(CXX) -c -I src/include $< -o $@

clean:
	rm -f $(OBJS) main