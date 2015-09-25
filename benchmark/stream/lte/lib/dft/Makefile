
include ../../../common/makefile.config

#SRC = dft.cpp test.cpp ../util.cpp
SRC = test.cpp ../util.cpp
SRC += $(LIB_DIR)/check/check.cpp $(LIB_DIR)/timer/meas.cpp

SRC_OMP = $(subst dft, dft_omp, $(SRC))

OBJS = $(patsubst %.cpp, %.o, $(SRC))
OBJS_OMP = $(patsubst %.cpp, %.o, $(SRC_OMP))

INC_DIRS = ./ ../ $(LIB_DIR)

CXXFLAGS += $(INC_DIRS:%=-I%)

#LINKFLAGS += -L$(LIB_DIR)/dft/c -ldft

TARGETS = dft.out dft_omp.out

all: $(TARGETS) 

dft: $(OBJS) libdft.a
	$(CXX) -o $@ $(OBJS) $(LINKFLAGS) -L$(LIB_DIR)/dft/c -ldft

libdft.a: dft.o
	ar rcs $@ $^

dft_omp: $(OBJS_OMP) libdftomp.a
	$(CXX) -o $@ $(OBJS_OMP) $(LINKFLAGS) -fopenmp -L$(LIB_DIR)/dft/c -ldftomp

libdftomp.a: dft_omp.o
	ar rcs $@ $^

dft_omp.o: dft_omp.cpp
	$(CXX) -o $@ -c $< $(CXXFLAGS) -fopenmp

%.o: %.cpp
	$(CXX) -o $@ -c $< $(CXXFLAGS)


clean:
	rm -rf $(OBJS) $(OBJS_OMP) $(TARGETS) *.o *.a *.so
