./build/bin/ccc: ./ccc/src/ccc.cpp
	mkdir -p ./build/bin
	$(COMPILER) ./ccc/src/ccc.cpp -o ./build/bin/ccc -I./ccc/inc $(COMPILE_FLAGS)

clean:
	rm -rf ./build/bin/ccc