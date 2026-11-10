
# For god's sake do not compile ANYTHING inside Git-bash, fucks up PATH, STDLIB and appearantly C Runtime and DLL comp.

# 64-bit MinGW
CXX = x86_64-w64-mingw32-g++
TARGET = CoBaLT.exe

# Release DLL
RELEASE_DIR = release
DLL_TARGET  = CoBaLT.dll
IMPLIB      = libCoBaLT.dll.a
DLL_LDFLAGS = -static -shared -Wl,--export-all-symbols -Wl,--out-implib,$(RELEASE_DIR)/$(IMPLIB)

# Raylib path
RAYLIB_PATH = C:/raylib/raylib/src

# Compiler flags
CXXFLAGS = -std=c++23 -Wall -Wextra -O3 -I$(RAYLIB_PATH)

SRC = src
# Common source files
CPP_SRCS_COMMON := $(filter-out $(SRC)/CBLT_Dialog_Win32.cpp $(SRC)/CBLT_Dialog_Linux.cpp $(SRC)/CBLT_Dialog_MacOS.cpp, $(wildcard $(SRC)/*.cpp))

CPP_SRCS_PLATFORM := $(SRC)/CBLT_Dialog_Win32.cpp
LDFLAGS := -L$(RAYLIB_PATH) -lraylib -lopengl32 -lgdi32 -lwinmm -lole32 -lshell32 -luuid -static-libgcc -static-libstdc++

CPP_SRCS := $(CPP_SRCS_COMMON) $(CPP_SRCS_PLATFORM)
OBJDIR := obj
CPP_OBJS := $(notdir $(CPP_SRCS:%.cpp=%.o))
CPP_OBJS := $(addprefix $(OBJDIR)/,$(CPP_OBJS))
OBJS := $(CPP_OBJS)

# Default target
all: $(TARGET)

# Build executable
$(TARGET): $(OBJS)
	@if not exist $(OBJDIR) mkdir $(OBJDIR)
	$(CXX) $(OBJS) -o $(TARGET) $(LDFLAGS)

# Compile object files
$(OBJDIR)/%.o: $(SRC)/%.cpp
	@if not exist $(OBJDIR) mkdir $(OBJDIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Build DLL
$(RELEASE_DIR)/$(DLL_TARGET): $(OBJS)
	@if not exist $(RELEASE_DIR) mkdir $(RELEASE_DIR)
	$(CXX) $(OBJS) $(DLL_LDFLAGS) -o $@ $(LDFLAGS)

# Clean
clean:
	@if exist $(OBJDIR) rmdir /s /q $(OBJDIR)
	@if exist $(TARGET) del /q $(TARGET)
	@if exist $(RELEASE_DIR) rmdir /s /q $(RELEASE_DIR)

# Release target
rel: $(RELEASE_DIR)/$(DLL_TARGET)
