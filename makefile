all:
	gcc -g -Wall main.c \
		dungeon.c \
		entity.c \
		player.c \
		map.c \
		minimap.c \
		dungeonbuild.c \
		sdl.c \
		-lSDL2 -lSDL2_image -lSDL2_ttf \
		-o game
