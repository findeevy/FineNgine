CXX := g++
CXXFLAGS := -std=c++17 -Wall -Wextra -O3
LDFLAGS :=  -lglfw -lvulkan -ldl -lpthread -lX11 -lXxf86vm -lXrandr -lXi
TARGET := FineNgine
SRC_DIR := src

SRCS := $(SRC_DIR)/main.cpp $(SRC_DIR)/render.cpp $(SRC_DIR)/utils.cpp
OBJS := $(SRCS:.cpp=.o)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) $^ -o $@ $(LDFLAGS)

$(SRC_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(TARGET) $(OBJS)

FORMAT_EXTENSIONS := *.cpp *.h
FORMAT_DIR := src
CLANG_FORMAT := clang-format
FORMAT_STYLE := file

format:
	find $(FORMAT_DIR) -type f \( -name "*.cpp" -o -name "*.h" \) -exec $(CLANG_FORMAT) -i -style=$(FORMAT_STYLE) {} +

.PHONY: format
