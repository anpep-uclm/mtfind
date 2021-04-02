CXXFLAGS += -Iinclude -pthread -std=c++17

all: mtfind

mtfind: src/Thread.cpp src/SearchThread.cpp src/mtfind.cpp
	${CXX} ${CXXFLAGS} $^ -o $@

clean:
	rm -f mtfind *.d

.PHONY: all clean
