CXX = g++
CXXFLAGS = -std=c++20 -Wall -pedantic

BUILD_DIR = build
TEMPLATE_DIR = .template
OUT_EXE = UTaste

SRCS = $(wildcard examples/*.cpp utils/*.cpp server/*.cpp)
OBJS = $(patsubst %.cpp, $(BUILD_DIR)/%.o, $(SRCS))

ifeq ($(OS),Windows_NT)
	LDLIBS += -l Ws2_32
endif

all: $(BUILD_DIR)/examples $(BUILD_DIR)/utils $(BUILD_DIR)/server $(OUT_EXE)

$(OUT_EXE): $(OBJS)
	$(CXX) $(CXXFLAGS) $^ $(LDLIBS) -o $@

$(BUILD_DIR)/examples $(BUILD_DIR)/utils $(BUILD_DIR)/server:
	mkdir -p $@

$(BUILD_DIR)/%.o: %.cpp | $(BUILD_DIR)/examples $(BUILD_DIR)/utils $(BUILD_DIR)/server
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -rf $(BUILD_DIR) $(TEMPLATE_DIR) *.o *.out &> /dev/null
