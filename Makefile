all: calendar

SRC_FILES=$(shell find src -type f -name "*.cpp")
SRC_OBJECTS=$(SRC_FILES:src/%=obj/%.o)

calendar: $(SRC_OBJECTS)
	g++ -lcurl -lncurses -lm $^ -o $@

obj/%.o: src/%
	mkdir -p $(dir $@)
	g++ -c -Wall -g -std=c++14 -O1 $< -o $@

clean:
	rm -rf calendar obj/

