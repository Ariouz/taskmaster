#################### VARIABLES
NAME = taskmaster
CXX = g++
CXXFLAGS = -Wall -Wextra -Werror -std=c++17 -g
LDFLAGS = -lreadline

SRC_DIR = src
OBJ_DIR = obj

INCLUDES = $(addprefix -I,$(shell find include -type d))

SRCS = $(wildcard $(SRC_DIR)/**/*.cpp)
OBJS = $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SRCS))

#################### RULES

all: $(NAME)

$(NAME): $(OBJS)
	$(CXX) $(CXXFLAGS) $(INCLUDES) $^ -o $@ $(LDFLAGS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp | $(OBJ_DIR)
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

clean:
	rm -rf $(OBJ_DIR)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
