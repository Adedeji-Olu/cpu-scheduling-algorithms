CXX = c++
CXXFLAGS = -std=c++17 -Wall -Wextra -Wpedantic -O2
DEBUGFLAGS = -g -O0 -DDEBUG
INCLUDES = -Iinclude -I/usr/local/include
LDFLAGS = -L/usr/local/lib -lgtest -lgtest_main -pthread

SRC_DIR = src
BUILD_DIR = build
BIN_DIR = bin
OBJ_DIR = $(BUILD_DIR)/obj

SOURCES = $(wildcard $(SRC_DIR)/*.cpp)
MAIN_SRC = $(SRC_DIR)/main.cpp
LIB_SOURCES = $(filter-out $(MAIN_SRC),$(SOURCES))
OBJECTS = $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SOURCES))
LIB_OBJECTS = $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(LIB_SOURCES))

EXEC = $(BIN_DIR)/scheduler
UNIT_TEST = $(BIN_DIR)/unit_tests
SYSTEM_TEST = $(BIN_DIR)/system_tests

.PHONY: all build debug test unit-test system-test clean install

all: build test

build: $(EXEC)

debug: CXXFLAGS = -std=c++17 -Wall -Wextra $(DEBUGFLAGS)
debug: clean $(EXEC)

$(EXEC): $(OBJECTS) | $(BIN_DIR)
	$(CXX) $(CXXFLAGS) $^ -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

test: unit-test system-test

unit-test: $(UNIT_TEST)
	./$(UNIT_TEST)

system-test: $(SYSTEM_TEST)
	./$(SYSTEM_TEST)

$(UNIT_TEST): tests/unit/test_scheduler.cpp $(LIB_OBJECTS) | $(BIN_DIR)
	$(CXX) $(CXXFLAGS) $(INCLUDES) $< $(LIB_OBJECTS) $(LDFLAGS) -o $@

$(SYSTEM_TEST): tests/system/system_test.cpp $(LIB_OBJECTS) | $(BIN_DIR)
	$(CXX) $(CXXFLAGS) $(INCLUDES) $< $(LIB_OBJECTS) $(LDFLAGS) -o $@

clean:
	rm -rf $(BUILD_DIR) $(BIN_DIR)

install: build
	install -d /usr/local/bin
	install -m 755 $(EXEC) /usr/local/bin/cpu-scheduler

$(BIN_DIR):
	mkdir -p $(BIN_DIR)

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)
