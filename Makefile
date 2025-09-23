#################### VARIABLES
NAME = taskmaster
CXX = g++
CXXFLAGS = -Wall -Wextra -Werror -std=c++17 -g $(if $(BONUS),-DBONUS)
LDFLAGS = -lreadline -lyaml-cpp

SRC_DIR = src
OBJ_DIR = obj

INCLUDES = $(addprefix -I,$(shell find include -type d))

SRCS = $(wildcard $(SRC_DIR)/**/*.cpp)
OBJS = $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SRCS))

YAML_INSTALLED := $(shell dpkg -s libyaml-cpp-dev 2>/dev/null | grep -c "Status: install ok installed")

#################### RULES

all: install-yaml-cpp $(NAME)

$(NAME): $(OBJS)
	$(CXX) $(CXXFLAGS) $(INCLUDES) $^ -o $@ $(LDFLAGS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp | $(OBJ_DIR)
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

bonus:
	make $(NAME) BONUS=1

clean:
	rm -rf $(OBJ_DIR)

fclean: clean
	rm -f $(NAME)

re: fclean all

install-yaml-cpp:
ifeq ($(YAML_INSTALLED),0)
	@echo "yaml-cpp is not installed, installation via apt..."
	sudo apt update && sudo apt install -y libyaml-cpp-dev
else
	@echo "yaml-cpp is already installed"
endif

.PHONY: all clean fclean re install-yaml-cpp
