build:
	mkdir -p build
	cd build && cmake ..
	cd build && cmake --build .

run: build
	./build/Proyecto_1

clean:
	rm -rf build

.PHONY: build run clean