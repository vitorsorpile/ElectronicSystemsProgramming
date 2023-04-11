PROJ_NAME=Montanha_Russa

# .c files
C_SOURCE=$(wildcard src/*.cpp)

# .h files
H_SOURCE=$(wildcard src/*.h)

# Object files
OBJ=$(C_SOURCE:.cpp=.o)

# Compiler
CC=g++-11

# Flags for compiler
CC_FLAGS= -Wall      \
          -std=c++20 \
		  -lrt		 \
		  -pthread	 \
		  -g

#
# Compilation and linking
#
all: $(PROJ_NAME)

$(PROJ_NAME): $(OBJ)
	$(CC) -o $@ $^ $(CC_FLAGS)

%.o: %.cpp %.h
	$(CC) -o $@ -c $< $(CC_FLAGS)

main.o: main.cpp $(H_SOURCE)
	$(CC) -o $@ $< $(CC_FLAGS)


clean:
	rm -rf src/*.o $(PROJ_NAME) *~

remake:
	$(MAKE) clean
	$(MAKE)

run:
	$(MAKE)
	./$(PROJ_NAME)

valgrind:
	$(MAKE)
	valgrind -s --leak-check=full --show-leak-kinds=all ./$(PROJ_NAME)