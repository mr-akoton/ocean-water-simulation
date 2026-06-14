# ============================================================================ #
# VARIABLES

NAME		:= water-simulation

SRC_DIR		:= ./src
INC_DIR		:= ./inc
LIB_DIR		:= ./lib
EXT_DIR		:= ./ext

BUILD_TYPE	?= debug

# ============================================================================ #
# COMPILER SETTING

CC			:= gcc
CFLAGS		:= -I $(INC_DIR) -I $(LIB_DIR)

CXX_VERSION	:= -std=c++23
CXX = g++
ifeq ($(BUILD_TYPE), release)
	CXXFLAGS := -Wall -Wextra -O2 -DNDEBUG $(CXX_VERSION)
else
	CXXFLAGS := -Wall -Wextra -Wpedantic -O0 -g $(CXX_VERSION)
endif
CXXFLAGS	+= -I $(INC_DIR) -I $(LIB_DIR)
LDFLAGS		:= -L $(EXT_DIR) -lglfw -lGL -lX11 -lpthread -lXrandr -lXi -ldl

RM			:= rm -rf

# ============================================================================ #
# SOURCES

# ---------------------------------- C Files --------------------------------- #

SRC_C		:= $(EXT_DIR)/glad/glad.c
OBJ_C		:= $(SRC_C:.c=.o)

# --------------------------------- C++ Files -------------------------------- #

EXT_FILE	:= imgui/imgui_draw.cpp \
			   imgui/imgui_impl_glfw.cpp \
			   imgui/imgui_impl_opengl3.cpp \
			   imgui/imgui_tables.cpp \
			   imgui/imgui_widgets.cpp \
			   imgui/imgui.cpp \
			   stb/stb_image.cpp

SRC_FILE	:= components/Camera.cpp \
				components/CubeMap.cpp \
			   components/Water.cpp \
			   components/Interface.cpp \
			   core/objects/EBO.cpp \
			   core/objects/VAO.cpp \
			   core/objects/VBO.cpp \
			   core/Engine.cpp \
			   core/Shader.cpp \
			   core/Texture.cpp \
			   core/Window.cpp \
			   main.cpp

SRC_CXX		:= $(addprefix $(EXT_DIR)/, $(EXT_FILE)) \
			   $(addprefix $(SRC_DIR)/, $(SRC_FILE))
OBJ_CXX		:= $(SRC_CXX:.cpp=.o)

# ------------------------------- Final Object ------------------------------- #

OBJ			:= $(OBJ_C) $(OBJ_CXX)

# ============================================================================ #
# RULES

all: $(NAME)

$(NAME): $(OBJ_C) $(OBJ_CXX)
	$(CXX) $(CXXFLAGS) $(OBJ) $(LDFLAGS) -o $@

run: all
	@./$(NAME)

clean:
	$(RM) $(OBJ)

fclean: clean
	$(RM) $(NAME)

re: fclean all

# ============================================================================ #
# PHONY

.PHONY: all clean fclean re
