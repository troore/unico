TARGETS := stream.out

SRCS := main.cpp Framework.cpp

OBJS := $(patsubst %.cpp,%.o,$(SRCS))

CXXFLAGS := -g -Wall
LINKFLAGS :=

stream.out: $(OBJS)
	$(CXX) -o $@ $^ $(LINKFLAGS)

%.o: %.cpp
	$(CXX) -o $@ -c $< $(CXXFLAGS)

clean:
	rm -rf $(TARGETS) *.o
