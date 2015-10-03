TARGETS := stream.out

SRCS := framework.cpp dinic.cpp disjoint.cpp priorityQ.cpp

CXXFLAGS := -g -Wall
LINKFLAGS :=

stream.out: $(OBJS)
	$(CXX) -o $@ $^ $(LINKFLAGS)

%.cpp: %.o
	$(CXX) -o $@ -c $< $(CXXFLAGS)

clean:
	rm -rf $(TARGETS) *.o
