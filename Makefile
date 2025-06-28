CXX := g++
CXXFLAGS := -std=c++17 -Wall -Wextra -O3
LDFLAGS :=  -lglfw -lvulkan -ldl -lpthread -lX11 -lXxf86vm -lXrandr -lXi
TARGET := FineNgine
SRC_DIR := src

SRCS := $(SRC_DIR)/main.cpp $(SRC_DIR)/render.cpp
OBJS := $(SRCS:.cpp=.o)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) $^ -o $@ $(LDFLAGS)

$(SRC_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(TARGET) $(OBJS)

.PHONY: clean
