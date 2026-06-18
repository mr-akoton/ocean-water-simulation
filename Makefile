# ============================================================================ #
# VARIABLES

NAME		:= water-simulation

SRC_DIR		:= ./src
INC_DIR		:= ./inc
LIB_DIR		:= ./lib
EXT_DIR		:= ./ext
OBJ_DIR		:= ./obj

BLD_DIR		:= ./build
SHD_DIR		:= ./shader
RES_DIR		:= ./resources

SETTINGS	:= ./settings.json

BUILD_TYPE	?= debug

# ============================================================================ #
# COMPILER SETTING

CC			:= gcc
CFLAGS		:= -I $(INC_DIR) -I $(LIB_DIR)

CXX_VERSION	:= -std=c++17
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
OBJ_C   	:= $(addprefix $(OBJ_DIR)/, $(patsubst ./%,%,$(SRC_C:.c=.o)))

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
			   components/Environment.cpp \
			   components/Interface.cpp \
			   components/Water.cpp \
			   core/objects/EBO.cpp \
			   core/objects/FBO.cpp \
			   core/objects/RBO.cpp \
			   core/objects/VAO.cpp \
			   core/objects/VBO.cpp \
			   core/Engine.cpp \
			   core/Shader.cpp \
			   core/Texture.cpp \
			   core/Window.cpp \
			   effects/Fog.cpp \
			   settings/SettingsData.cpp \
			   settings/SettingsManager.cpp \
			   main.cpp

SRC_CXX		:= $(addprefix $(EXT_DIR)/, $(EXT_FILE)) \
			   $(addprefix $(SRC_DIR)/, $(SRC_FILE))
OBJ_CXX 	:= $(addprefix $(OBJ_DIR)/, $(patsubst ./%,%,$(SRC_CXX:.cpp=.o)))

# ------------------------------- Final Object ------------------------------- #

OBJ			:= $(OBJ_C) $(OBJ_CXX)

# ============================================================================ #
# RULES

$(OBJ_DIR)/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR)/%.o: %.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

all: $(NAME)

$(NAME): $(OBJ_C) $(OBJ_CXX)
	$(CXX) $(CXXFLAGS) $(OBJ) $(LDFLAGS) -o $@

build:
	@BUILD_TYPE=release make re
	@mkdir -p $(BLD_DIR)
	@mv $(NAME) $(BLD_DIR)
	@cp -r $(SETTINGS) $(SHD_DIR) $(RES_DIR) $(BLD_DIR)/.

archive:
	zip -r $(NAME).zip $(BLD_DIR)
	tar -czvf $(NAME).tar.gz $(BLD_DIR)

run: all
	@./$(NAME)

clean:
	$(RM) $(OBJ_DIR)

fclean: clean
	$(RM) $(NAME)

re: fclean all

# ============================================================================ #
# PHONY

.PHONY: all clean fclean re
