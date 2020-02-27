CXX = g++
DEBUG = -g
CC_FLAGS = $(DEBUG) -W -Wall  
CC_FLAGS += -I./
CC_FLAGS += -I./mysql
CC_FLAGS += -I./mysql/include
#CC_FLAGS += -I./mysql/include/mysqlx
CC_FLAGS += -I./mysql/include/mysql
#CC_FLAGS += -I./mysql/include/jdbc
CC_FLAGS += -std=c++11

#LIB_PATH = -L./mysql/lib64
#LIBS += -Wl,-rpath=./mysql/lib -lrt -lmysqlcppconn
LIB_PATH = -L./mysql/lib
LIBS += -Wl,-rpath=./mysql/lib -lrt -lmysqlclient

OBJS = main.o

VERSION = 1.2.0
EXEC = server

$(EXEC): $(OBJS)
	$(CXX) $(CC_FLAGS) $(OBJS) -o $@  $(LIB_PATH) $(LIBS)
main.o: main.cpp
	$(CXX) $(CC_FLAGS) -c -o $@ main.cpp
	
all: $(EXEC) $(TEST)
	
.PHONY:clean srs
clean:
	rm -rf $(OBJS) $(EXEC)  logs/*.log


