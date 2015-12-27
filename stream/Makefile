TARGETS := PipeLatPower.out

SRCS_PLP := main_PipeLatPower.cpp PipeModel.cpp PipeLatPower.cpp Task.cpp Bucket.cpp
SRCS_PLP += ../timer/timer.cpp

OBJS_PLP := $(patsubst %.cpp,%.o,$(SRCS_PLP))

CXXFLAGS := -g -Wall
CXXFLAGS += -g -Wall -I../timer
LINKFLAGS :=

PipeLatPower.out: $(OBJS_PLP)
	$(CXX) -o $@ $^ $(LINKFLAGS)

%.o: %.cpp
	$(CXX) -o $@ -c $< $(CXXFLAGS)

clean:
	rm -rf *.o $(TARGETS)
