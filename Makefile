all:
	mkdir -p build
	g++ ./sand/main.cpp -o ./build/main.cpp

.PHONY: run clean
run:
	./build/main.cpp

clean:
	rm -rf build
