CXX = g++
CXXFLAGS = -Wall -Wextra -Werror -std=c++14 -pedantic -O3 -mavx512vl
LDLIBS = -lbenchmark -lgtest

EXECS := bench2d test2d

all: Makefile $(EXECS) $(EXECS:=.s)

$(EXECS): %: %.o
	$(CXX) $(CXXFLAGS) $< -o $@ $(LDLIBS)

%.o: %.cpp %.h Makefile
	$(CXX) $(CXXFLAGS) -c $< -o $@

bench2d.o: bench2d.cpp Makefile
	$(CXX) $(CXXFLAGS) -c $< -o $@

test2d.o: test2d.cpp Makefile
	$(CXX) $(CXXFLAGS) -c $< -o $@

%.s: %.cpp %.h
	$(CXX) $(CXXFLAGS) -S $< -o $@

bench2d.s: bench2d.cpp Makefile
	$(CXX) $(CXXFLAGS) -S $< -o $@

test2d.s: test2d.cpp Makefile
	$(CXX) $(CXXFLAGS) -S $< -o $@

clean:
	rm -f $(EXECS) *.o *.s
