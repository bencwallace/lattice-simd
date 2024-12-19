CXX = g++
CXXFLAGS = -Wall -Wextra -Werror -std=c++11 -pedantic -O3 -mavx512vl #-fsanitize=alignment
LDLIBS = -lbenchmark

EXECS := main

all: $(EXECS) $(EXECS:=.s)

$(EXECS): %: %.o s_trans4.h v_trans4.h
	$(CXX) $(CXXFLAGS) $< -o $@ $(LDLIBS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

%.s: %.cpp
	$(CXX) $(CXXFLAGS) -S $< -o $@

clean:
	rm -f $(EXECS) *.o *.s
