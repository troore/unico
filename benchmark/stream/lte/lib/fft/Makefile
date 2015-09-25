
include ../../common/makefile.config

SRC = fft.cpp test.cpp util.cpp
SRC += $(LIB_DIR)/check/check.cpp $(LIB_DIR)/timer/timer.cpp

OBJS = $(patsubst %.cpp, %.o, $(SRC))

INC_DIRS = ./ ../ $(LIB_DIR)

CXXFLAGS += $(INC_DIRS:%=-I%)

#LINKFLAGS += -L$(LIB_DIR)/fft/c -lfft

TARGETS = fft.out

all: $(TARGETS) 

#fft.out: $(OBJS) libfft.a
fft.out: $(OBJS)
	$(CXX) -o $@ $(OBJS) $(LINKFLAGS)

libfft.a: fft.o util.o
	ar rcs $@ $^

%.o: %.cpp
	$(CXX) -o $@ -c $< $(CXXFLAGS)

clean:
	rm -rf *.o $(OBJS) $(TARGETS) *.a *.so
