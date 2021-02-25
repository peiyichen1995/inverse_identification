CXX = g++
CXXFLAGS = -Wall -g -fPIC $(INCLUDE)
LDFLAGS =
INCLUDE = -Iinclude -I$(HOME)/.local/include -Ieigen

src = $(wildcard src/*.cpp)
obj = $(src:.cpp=.o)

objective: $(obj)
	$(CXX) $(CXXFLAGS) $(INCLUDE) -o $@ $^ $(LDFLAGS)

.PHONY: clean

clean:
	rm -f src/*.o
