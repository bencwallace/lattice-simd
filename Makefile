CXX = g++
CXXFLAGS = -Wall -Wextra -Werror -std=c++14 -pedantic -O3 -mavx512vl #-fsanitize=alignment
LDLIBS = -lbenchmark

EXECS := bench2 bench4
INCLUDES := s_trans.h v_trans.h

all: $(EXECS) $(EXECS:=.s)

$(EXECS): %: %.o $(INCLUDES)
	$(CXX) $(CXXFLAGS) $< -o $@ $(LDLIBS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

%.s: %.cpp
	$(CXX) $(CXXFLAGS) -S $< -o $@

clean:
	rm -f $(EXECS) *.o *.s
