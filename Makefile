CFLAGS = -std=c++17 -O3
LDFLAGS = -lglfw -lvulkan -ldl -lpthread -lX11 -lXxf86vm -lXrandr -lXi

FineNgine: src/main.cpp
	g++ $(CFLAGS) -o FineNgine src/main.cpp $(LDFLAGS)

.PHONY: test clean

test: FineNgine
	./FineNgine

clean:
	rm -f FineNgine
