all:
	gcc -g main.c \
		fsg_dungeon.c \
		fsg_entity.c \
		fsg_player.c \
		fsg_map.c \
		-Wall \
		cdlv/menu/cdlv_sdl.c \
		-lSDL2 -lSDL2_image -lSDL2_ttf \
		-L cdlv/build/ -lcdlv \
		-o game
