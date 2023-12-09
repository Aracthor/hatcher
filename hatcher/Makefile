CXX=	g++
EMXX=	em++
ARXX=	ar rc
EMARXX=	emar rc
MKDIR=	mkdir
RM=	rm -f
RMDIR=	rm -fd
CLONE=	git clone

IMGUI_REPO=		imgui/
IMGUI_VERSION=		v1.88
IMGUI_REPO_URL=		https://github.com/ocornut/imgui.git

JS_DIR=			js/
SRCS_DIR=		srcs/hatcher/
TEST_DIR=		unit_testing/
INC_DIR=		srcs/
OBJS_DIR=		objs/
BIN_DIR=		bin/
NATIVE_DIR=		native/
WEBASM_DIR=		webasm/
RELEASE_DIR=		release/
DEBUG_DIR=		debug/

IMGUI_SUBDIRS=		backends/	\

SRCS_SUBDIRS=		Graphics/	\

SRCS_SUBSUBDIRS=	Graphics/Core/	\

OBJS_NATIVE_DIR=		$(OBJS_DIR)$(NATIVE_DIR)
OBJS_NATIVE_RELEASE_DIR=	$(OBJS_NATIVE_DIR)$(RELEASE_DIR)
OBJS_NATIVE_RELEASE_SUBDIRS=	$(SRCS_SUBDIRS:%=$(OBJS_NATIVE_RELEASE_DIR)%)
OBJS_NATIVE_RELEASE_SUBSUBDIRS=	$(SRCS_SUBSUBDIRS:%=$(OBJS_NATIVE_RELEASE_DIR)%)
OBJS_NATIVE_DEBUG_DIR=		$(OBJS_NATIVE_DIR)$(DEBUG_DIR)
OBJS_NATIVE_DEBUG_SUBDIRS=	$(SRCS_SUBDIRS:%=$(OBJS_NATIVE_DEBUG_DIR)%)
OBJS_NATIVE_DEBUG_SUBSUBDIRS=	$(SRCS_SUBSUBDIRS:%=$(OBJS_NATIVE_DEBUG_DIR)%)
OBJS_NATIVE_IMGUI_DIR=		$(OBJS_NATIVE_DIR)$(IMGUI_REPO)
OBJS_NATIVE_IMGUI_SUBDIRS=	$(IMGUI_SUBDIRS:%=$(OBJS_NATIVE_IMGUI_DIR)%)
OBJS_WEBASM_DIR=		$(OBJS_DIR)$(WEBASM_DIR)
OBJS_WEBASM_RELEASE_DIR=	$(OBJS_WEBASM_DIR)$(RELEASE_DIR)
OBJS_WEBASM_RELEASE_SUBDIRS=	$(SRCS_SUBDIRS:%=$(OBJS_WEBASM_RELEASE_DIR)%)
OBJS_WEBASM_RELEASE_SUBSUBDIRS=	$(SRCS_SUBSUBDIRS:%=$(OBJS_WEBASM_RELEASE_DIR)%)
OBJS_WEBASM_DEBUG_DIR=		$(OBJS_WEBASM_DIR)$(DEBUG_DIR)
OBJS_WEBASM_DEBUG_SUBDIRS=	$(SRCS_SUBDIRS:%=$(OBJS_WEBASM_DEBUG_DIR)%)
OBJS_WEBASM_DEBUG_SUBSUBDIRS=	$(SRCS_SUBSUBDIRS:%=$(OBJS_WEBASM_DEBUG_DIR)%)
OBJS_WEBASM_IMGUI_DIR=		$(OBJS_WEBASM_DIR)$(IMGUI_REPO)
OBJS_WEBASM_IMGUI_SUBDIRS=	$(IMGUI_SUBDIRS:%=$(OBJS_WEBASM_IMGUI_DIR)%)
OBJS_TEST_DIR=			$(OBJS_DIR)$(TEST_DIR)
WEBASM_BIN_DIR=			$(BIN_DIR)


# Order matters here : subfolders must be on top of their root folder, because it is the order used to delete them.
OBJS_DIRS=		$(OBJS_TEST_DIR)			\
			$(OBJS_NATIVE_RELEASE_SUBSUBDIRS)	\
			$(OBJS_NATIVE_RELEASE_SUBDIRS)		\
			$(OBJS_NATIVE_RELEASE_DIR)		\
			$(OBJS_NATIVE_DEBUG_SUBSUBDIRS)		\
			$(OBJS_NATIVE_DEBUG_SUBDIRS)		\
			$(OBJS_NATIVE_DEBUG_DIR)		\
			$(OBJS_NATIVE_IMGUI_SUBDIRS)		\
			$(OBJS_NATIVE_IMGUI_DIR)		\
			$(OBJS_NATIVE_DIR)			\
			$(OBJS_WEBASM_RELEASE_SUBSUBDIRS)	\
			$(OBJS_WEBASM_RELEASE_SUBDIRS)		\
			$(OBJS_WEBASM_RELEASE_DIR)		\
			$(OBJS_WEBASM_DEBUG_SUBSUBDIRS)		\
			$(OBJS_WEBASM_DEBUG_SUBDIRS)		\
			$(OBJS_WEBASM_DEBUG_DIR)		\
			$(OBJS_WEBASM_IMGUI_SUBDIRS)		\
			$(OBJS_WEBASM_IMGUI_DIR)		\
			$(OBJS_WEBASM_DIR)			\
			$(OBJS_DIR)				\

CXX_COMMON_FLAGS=	-Wall -Werror		\
			-std=c++17		\
			-I $(INC_DIR)		\
			-I $(IMGUI_REPO)	\

CXX_RELEASE_FLAGS=	$(CXX_COMMON_FLAGS)	\
			-O3

CXX_DEBUG_FLAGS=	$(CXX_COMMON_FLAGS)	\
			-g3			\
			-DNDEBUG		\

CXX_NATIVE_FLAGS=

EMXX_FLAGS=		-s USE_SDL=2 		\

CXX_IMGUI_NATIVE_FLAGS=	`sdl2-config --cflags`	\

LD_TEST_BIN_FLAGS=	-l$(PROJECT_NAME)_release -L $(BIN_DIR)


IMGUI_SRCS_FILES=	imgui.cpp			\
			imgui_demo.cpp			\
			imgui_draw.cpp			\
			imgui_tables.cpp		\
			imgui_widgets.cpp		\
			backends/imgui_impl_sdl.cpp	\
			backends/imgui_impl_opengl3.cpp	\

IMGUI_SRCS=	$(addprefix $(IMGUI_REPO),$(IMGUI_SRCS_FILES))

LIBIMGUI_NAME=	libimgui

IMGUI_OBJS_NATIVE=	$(IMGUI_SRCS:$(IMGUI_REPO)%.cpp=$(OBJS_NATIVE_IMGUI_DIR)%.o)
IMGUI_OBJS_WEBASM=	$(IMGUI_SRCS:$(IMGUI_REPO)%.cpp=$(OBJS_WEBASM_IMGUI_DIR)%.o)


SRCS_FILES=	CommandManager.cpp			\
		ComponentLoader.cpp			\
		ComponentManager.cpp			\
		ComponentSaver.cpp			\
		EntityDescriptor.cpp			\
		EntityDescriptorCatalog.cpp		\
		EntityManager.cpp			\
		EntityIDRegistry.cpp	    		\
		FileSystem.cpp 				\
		ISaveLoader.cpp				\
		GameApplication.cpp 			\
		string_view.cpp				\
		World.cpp				\
							\
		Graphics/Core/GLContext.cpp		\
		Graphics/Core/ImGuiIntegration.cpp	\
		Graphics/Core/ShaderProgram.cpp		\
		Graphics/Core/VertexArrayObject.cpp	\
		Graphics/Core/VertexBufferObject.cpp	\
		Graphics/Core/Window.cpp 		\
		Graphics/Clock.cpp			\
		Graphics/EventUpdater.cpp		\
		Graphics/FrameRenderer.cpp		\
		Graphics/Material.cpp 			\
		Graphics/MaterialFactory.cpp		\
		Graphics/Mesh.cpp 			\
		Graphics/MeshLoader.cpp			\
		Graphics/Rendering.cpp 			\
		Graphics/Texture.cpp 			\
							\
		Maths/RandomGenerator.cpp		\
		
SRCS=   	$(addprefix $(SRCS_DIR),$(SRCS_FILES))

TEST_SRCS_FILES=SaveLoader.cpp				\
		unique_ptr.cpp 				\

TEST_SRCS=	$(addprefix $(TEST_DIR),$(TEST_SRCS_FILES))

PROJECT_NAME=	hatcher
LIB_NAME=	lib$(PROJECT_NAME)

OBJS_NATIVE_RELEASE=	$(SRCS:$(SRCS_DIR)%.cpp=$(OBJS_NATIVE_RELEASE_DIR)%.o)
OBJS_NATIVE_DEBUG=	$(SRCS:$(SRCS_DIR)%.cpp=$(OBJS_NATIVE_DEBUG_DIR)%.o)
OBJS_WEBASM_RELEASE=	$(SRCS:$(SRCS_DIR)%.cpp=$(OBJS_WEBASM_RELEASE_DIR)%.o)
OBJS_WEBASM_DEBUG=	$(SRCS:$(SRCS_DIR)%.cpp=$(OBJS_WEBASM_DEBUG_DIR)%.o)
OBJS_TEST=		$(TEST_SRCS:$(TEST_DIR)%.cpp=$(OBJS_TEST_DIR)%.o)
OBJS=			$(OBJS_NATIVE_RELEASE)	\
			$(OBJS_NATIVE_DEBUG)	\
			$(OBJS_WEBASM_RELEASE)	\
			$(OBJS_WEBASM_DEBUG)	\
			$(IMGUI_OBJS_NATIVE)	\
			$(IMGUI_OBJS_WEBASM)	\
			$(OBJS_TEST)		\

DEPS=			$(OBJS:.o=.dep)


BIN_IMGUI_NATIVE=	$(BIN_DIR)$(LIBIMGUI_NAME)_native.a
BIN_IMGUI_WEBASM=	$(BIN_DIR)$(LIBIMGUI_NAME)_webasm.a

BIN_NATIVE_RELEASE=	$(BIN_DIR)$(LIB_NAME)_release.a

BIN_NATIVE_DEBUG=	$(BIN_DIR)$(LIB_NAME)_debug.a

BIN_WEBASM_RELEASE=	$(BIN_DIR)$(LIB_NAME)_webasm_release.a

BIN_WEBASM_DEBUG=	$(BIN_DIR)$(LIB_NAME)_webasm_debug.a

BIN_TEST=		$(BIN_DIR)unit_testing

BINS=			$(BIN_NATIVE_RELEASE)	\
			$(BIN_NATIVE_DEBUG)	\
			$(BIN_WEBASM_RELEASE)	\
			$(BIN_WEBASM_DEBUG)	\
			$(BIN_IMGUI_NATIVE)	\
			$(BIN_IMGUI_WEBASM)	\
			$(BIN_TEST)		\


.SECONDARY:

.SECONDEXPANSION:

include $(shell test -d $(OBJS_DIR) && find $(OBJS_DIR) -name "*.dep")

%/:
	$(MKDIR) -p $@


$(OBJS_NATIVE_RELEASE_DIR)%.o:	$(SRCS_DIR)%.cpp | $$(@D)/ $(IMGUI_REPO)
	$(CXX) $(CXX_NATIVE_FLAGS) $(CXX_RELEASE_FLAGS) -MMD -MF $(@:.o=.dep) -c $< -o $@

$(OBJS_NATIVE_DEBUG_DIR)%.o:	$(SRCS_DIR)%.cpp | $$(@D)/ $(IMGUI_REPO)
	$(CXX) $(CXX_NATIVE_FLAGS) $(CXX_DEBUG_FLAGS) -MMD -MF $(@:.o=.dep) -c $< -o $@

$(OBJS_NATIVE_IMGUI_DIR)%.o:	$(IMGUI_REPO)%.cpp | $$(@D)/
	$(CXX) $(CXX_NATIVE_FLAGS) $(CXX_RELEASE_FLAGS) $(CXX_IMGUI_NATIVE_FLAGS) -c $< -o $@

$(OBJS_WEBASM_RELEASE_DIR)%.o:	$(SRCS_DIR)%.cpp | $$(@D)/ $(IMGUI_REPO)
	$(EMXX) $(EMXX_FLAGS) $(CXX_RELEASE_FLAGS) -MMD -MF $(@:.o=.dep) -c $< -o $@

$(OBJS_WEBASM_DEBUG_DIR)%.o:	$(SRCS_DIR)%.cpp | $$(@D)/ $(IMGUI_REPO)
	$(EMXX) $(EMXX_FLAGS) $(CXX_DEBUG_FLAGS) -MMD -MF $(@:.o=.dep) -c $< -o $@

$(OBJS_WEBASM_IMGUI_DIR)%.o:	$(IMGUI_REPO)%.cpp | $(OBJS_WEBASM_IMGUI_DIR) $(OBJS_WEBASM_IMGUI_SUBDIRS)
	$(EMXX) $(EMXX_FLAGS) $(CXX_RELEASE_FLAGS) -c $< -o $@


$(IMGUI_REPO):
	$(CLONE) $(IMGUI_REPO_URL) --depth 1 --branch $(IMGUI_VERSION)

$(BIN_IMGUI_NATIVE):	| $(IMGUI_OBJS_NATIVE) $(BIN_DIR) $(IMGUI_REPO)
	$(ARXX) $(BIN_IMGUI_NATIVE) $(IMGUI_OBJS_NATIVE)

$(BIN_IMGUI_WEBASM):	| $(IMGUI_OBJS_WEBASM) $(BIN_DIR) $(IMGUI_REPO)
	$(EMARXX) $(BIN_IMGUI_WEBASM) $(IMGUI_OBJS_WEBASM)


$(BIN_NATIVE_RELEASE):	$(OBJS_NATIVE_RELEASE) $(BIN_IMGUI_NATIVE) | $(BIN_DIR)
	$(ARXX) $(BIN_NATIVE_RELEASE) $(OBJS_NATIVE_RELEASE)

$(BIN_NATIVE_DEBUG):	$(OBJS_NATIVE_DEBUG) $(BIN_IMGUI_NATIVE) | $(BIN_DIR)
	$(ARXX) $(BIN_NATIVE_DEBUG) $(OBJS_NATIVE_DEBUG)

$(BIN_WEBASM_RELEASE):	$(OBJS_WEBASM_RELEASE) $(BIN_IMGUI_WEBASM) | $(WEBASM_BIN_DIR)
	$(EMARXX) $(BIN_WEBASM_RELEASE) $(OBJS_WEBASM_RELEASE)

$(BIN_WEBASM_DEBUG):	$(OBJS_WEBASM_DEBUG) $(BIN_IMGUI_WEBASM) | $(WEBASM_BIN_DIR)
	$(EMARXX) $(BIN_WEBASM_DEBUG) $(OBJS_WEBASM_DEBUG)


$(OBJS_TEST_DIR)%.o:	$(TEST_DIR)%.cpp	| $$(@D)/
	$(CXX) $(CXX_NATIVE_FLAGS) $(CXX_RELEASE_FLAGS) -MMD -MF $(@:.o=.dep) -c $< -o $@

$(BIN_TEST):	$(BIN_NATIVE_RELEASE) $(OBJS_TEST) | $$(@D)/
	$(CXX) $(OBJS_TEST) -o $(BIN_TEST) $(LD_TEST_BIN_FLAGS)


all:	$(BINS)

clean:
	$(RM) $(OBJS) $(DEPS)
	$(RMDIR) $(OBJS_DIRS)

fclean: clean
	$(RM) $(BINS)
	$(RMDIR) $(BIN_DIR)

re:	fclean native_release

test:	$(BIN_TEST)
	./$(BIN_TEST)

# aliases command
native_release:	$(BIN_NATIVE_RELEASE)
native_debug:	$(BIN_NATIVE_DEBUG)
webasm_release:	$(BIN_WEBASM_RELEASE)
webasm_debug:	$(BIN_WEBASM_DEBUG)

.DEFAULT_GOAL=	native_release
