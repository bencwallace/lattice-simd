CXX = g++
CXXFLAGS = -Wall -Wextra -Werror -std=c++14 -pedantic -O3 -mavx512vl #-fsanitize=alignment
LDLIBS = -lbenchmark -lgtest

EXECS := bench2 test2

all: Makefile $(EXECS) $(EXECS:=.s)

$(EXECS): %: %.o
	$(CXX) $(CXXFLAGS) $< -o $@ $(LDLIBS)

%.o: %.cpp %.h Makefile
	$(CXX) $(CXXFLAGS) -c $< -o $@

bench2.o: bench2.cpp Makefile
	$(CXX) $(CXXFLAGS) -c $< -o $@

test2.o: test2.cpp Makefile
	$(CXX) $(CXXFLAGS) -c $< -o $@

%.s: %.cpp %.h
	$(CXX) $(CXXFLAGS) -S $< -o $@

bench2.s: bench2.cpp Makefile
	$(CXX) $(CXXFLAGS) -S $< -o $@

test2.s: test2.cpp Makefile
	$(CXX) $(CXXFLAGS) -S $< -o $@

clean:
	rm -f $(EXECS) *.o *.s
