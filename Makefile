APPNAME = main
CXX = C:\mingw64\mingw64\bin\g++ -g
CXXFLAGS = -Werror -std=c++11 -DGLEW_STATIC
HDRDIR = -I./includes
OBJDIR = obj
SRCDIR = src
SOURCES := $(shell find $(SRCDIR) -name '*.cpp')
OBJECTS := $(patsubst $(SRCDIR)/%,$(OBJDIR)/%,$(SOURCES:%.cpp=%.o))
LDFLAGS_STATIC = -Wl,-Bstatic -lglfw3 -lglew32s -lgdi32 -lSOIL -lopengl32 -lfreetype -lassimp
LDFLAGS_DYNAMIC = -Wl,-Bdynamic -lpng -lharfbuzz -lz -lbz2 -lmman
OBJS = main.o shader.o texture.o resource_manager.o game.o rectangle.o text.o cube_map.o texture_3d.o cube.o entity.o physics.o level.o model.o mesh.o menu.o

all: $(APPNAME)

main: $(OBJECTS)
	$(CXX) $(CXXFLAGS) $(OBJECTS) $(LDFLAGS_STATIC) $(LDFLAGS_DYNAMIC) -o $(APPNAME)

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	$(CXX) $(CXXFLAGS) $(HDRDIR) -c $^ -o $@

clean:
	rm -f *~ *.o *.exe $(APPNAME)