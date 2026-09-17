CXX = g++
CXXFLAGS = -std=c++17 -Wall -Iinclude
SRC = $(wildcard src/*.cpp)

app: $(SRC)
	$(CXX) $(CXXFLAGS) $(SRC) -o app

clean:
	-del /Q app.exe 2>nul
