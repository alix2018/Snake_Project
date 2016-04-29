
SOURCE_FILES = list.c \
			   struc.c \
               affichage.c \
               bonus.c \
               ia.c \
               partie.c \
               collisions.c \
               score.c \
               multi.c \
               serveur.c \
               client.c

SRC_DIR = src
OBJ_DIR = build
BUILD_DIR = bin
DOXYGEN = doxygen

PROG_SOURCE_FILES = $(SRC_DIR)/main.c $(addprefix $(SRC_DIR)/, $(SOURCE_FILES))
TESTS_SOURCE_FILES = tests/tests.c $(addprefix $(SRC_DIR)/, $(SOURCE_FILES))
TESTS_OBJ_FILES = $(addprefix $(OBJ_DIR)/, $(TESTS_SOURCE_FILES:.c=.o))
PROG_OBJ_FILES = $(addprefix $(OBJ_DIR)/, $(PROG_SOURCE_FILES:.c=.o))

DIST_DIRS = src \
             tests \
             data
        
DIST_FILES = configure \
             Makefile.in \
             Doxyfile

VERSION = jalon3
TARNAME = alpha-snake-$(VERSION)

CC = gcc
CFLAGS = -g -O2
LDFLAGS = -lm

CLUTTER_CFLAGS = -pthread -I/usr/include/clutter-1.0 -I/usr/include/pango-1.0 -I/usr/include/cogl -I/usr/include/cairo -I/usr/include/atk-1.0 -I/usr/include/json-glib-1.0 -I/usr/include/gtk-3.0 -I/usr/include/harfbuzz -I/usr/include/freetype2 -I/usr/include/glib-2.0 -I/usr/lib/x86_64-linux-gnu/glib-2.0/include -I/usr/include/gdk-pixbuf-2.0 -I/usr/include/libdrm -I/usr/include/libpng12 -I/usr/include/pixman-1 -I/usr/include/gio-unix-2.0/  
CLUTTER_LDFLAGS = -pthread -lclutter-1.0 -latk-1.0 -lcogl-pango -ljson-glib-1.0 -lgdk-3 -lXi -lcogl -lgmodule-2.0 -lwayland-egl -lgbm -ldrm -lwayland-server -lEGL -lX11 -lXext -lXdamage -lXcomposite -lXrandr -lwayland-client -lgio-2.0 -lpangocairo-1.0 -lgdk_pixbuf-2.0 -lcairo-gobject -lpango-1.0 -lcairo -lgobject-2.0 -lglib-2.0 -lXfixes  

all: alpha-snake

$(OBJ_DIR)/%.o: %.c
	@test -d $(@D) || mkdir -p $(@D)
	@echo CC $<
	@$(CC) -c $< -o $@ $(CFLAGS) $(CLUTTER_CFLAGS)

alpha-snake: $(PROG_OBJ_FILES)
	@echo CCLD $^
	@test -d $(BUILD_DIR) || mkdir -p $(BUILD_DIR)
	@$(CC) $^ -o $(BUILD_DIR)/$@ $(CLUTTER_LDFLAGS) $(LDFLAGS)
	

tests: $(TESTS_OBJ_FILES)
	@test -d $(BUILD_DIR) || mkdir -p $(BUILD_DIR)
	@echo CCLD $^
	@$(CC) $^ -o $(BUILD_DIR)/$@ $(CLUTTER_LDFLAGS) $(LDFLAGS)

clean:
	-rm -rv $(OBJ_DIR)

maintainer-clean: clean
	-rm -rv Makefile configure config.status autom4te.cache aclocal.m4 config.log
	-rm -rv projet-ipi-jalon1
	-rm -v projet-ipi-jalon1.tar.bz2
	-rm -rv doc/

doc :
	@test -d doc || mkdir -p doc
	$(DOXYGEN) Doxyfile
	
dist-bzip:
	mkdir -p $(TARNAME)
	@cp -rv $(DIST_DIRS) $(TARNAME)
	@cp -v $(DIST_FILES) $(TARNAME)
	@cp -v README_prof.md $(TARNAME)/README.md
	tar -cvjSf $(TARNAME).tar.bz2 $(TARNAME)
	@rm -rv $(TARNAME)

