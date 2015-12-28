TARGETS := stream.out

SRCS := main.cpp PipeModel.cpp PipeAreaPower.cpp Task.cpp Bucket.cpp
SRCS += ../timer/timer.cpp

OBJS := $(patsubst %.cpp,%.o,$(SRCS))

CXXFLAGS := -g -Wall
CXXFLAGS += -g -Wall -I../timer
LINKFLAGS :=

stream.out: $(OBJS)
	$(CXX) -o $@ $^ $(LINKFLAGS)

%.o: %.cpp
	$(CXX) -o $@ -c $< $(CXXFLAGS)

clean:
	rm -rf *.o $(TARGETS)
