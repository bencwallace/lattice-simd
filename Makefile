CXX = g++
CXXFLAGS = -Wall -Wextra -Werror -std=c++14 -pedantic -O3 -mavx512vl #-fsanitize=alignment
LDLIBS = -lbenchmark
GTEST = -lgtest #-lgtest_main -pthread

EXECS := bench2 bench4 test2 test4
INCLUDES := s_trans.h v_trans.h

all: $(EXECS) $(EXECS:=.s)

$(EXECS): %: %.o
	$(CXX) $(CXXFLAGS) $< -o $@ $(LDLIBS) $(GTEST)

%.o: %.cpp $(INCLUDES)
	$(CXX) $(CXXFLAGS) -c $< -o $@

%.s: %.cpp
	$(CXX) $(CXXFLAGS) -S $< -o $@

clean:
	rm -f $(EXECS) *.o *.s
