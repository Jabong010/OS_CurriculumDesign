# 定义编译器和编译选项
CXX = g++
CXXFLAGS = -w -std=c++11

# 定义所有的源文件和目标文件
SRCS = shell.cpp
OBJS = $(SRCS:.cpp=.o)
BINS = $(SRCS:.cpp=)

# 默认目标，编译所有的可执行文件
all: $(BINS)

# 编译每个可执行文件
$(BINS): % : %.o
	$(CXX) $(CXXFLAGS) $< -o $@

# 编译每个目标文件
$(OBJS): %.o : %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# clean指令，清除所有生成的可执行文件和目标文件
clean:
	rm -f $(BINS) $(OBJS)
