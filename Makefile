# 变量定义
CXX = g++
CXXFLAGS = -Wall -g
LDFLAGS = -lncursesw

# 目录设置
SRC_DIR = src/include
INCLUDE_DIR = include
BIN_DIR = bin

# 可执行文件名
TARGET = $(BIN_DIR)/main 

# 查找所有的源文件和对象文件
SRCS = $(wildcard $(SRC_DIR)/*.cpp)
OBJS = $(SRCS:.cpp=.o)

# 默认目标
all: $(TARGET)

# 链接目标
$(TARGET): $(OBJS)
	@mkdir -p $(BIN_DIR)
	$(CXX) -o $@ $(OBJS) $(LDFLAGS)

# 编译目标
%.o: $(SRC_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -I$(INCLUDE_DIR) -c $< -o $@

# 清理生成的文件
clean:
	rm -f $(TARGET) $(OBJS)

# 声明伪目标
.PHONY: all clean