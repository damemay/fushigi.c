all:
	gcc -g -Wall main.c \
		dungeon.c \
		entity.c \
		player.c \
		map.c \
		minimap.c \
		randomwalk.c \
		cdlv/menu/cdlv_sdl.c \
		-lSDL2 -lSDL2_image -lSDL2_ttf \
		-L cdlv/build/ -lcdlv \
		-o game
