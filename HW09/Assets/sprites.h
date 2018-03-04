/*
 * Exported with nin10kit v1.7
 * Invocation command was nin10kit --mode=sprites --bpp=8 --for_bitmap sprites Hulk.png Hela.png Thor.png Loki.png 
 * Time-stamp: Sunday 11/12/2017, 12:10:26
 * 
 * Image Information
 * -----------------
 * Hulk.png 32@64
 * Hela.png 32@64
 * Thor.png 32@64
 * Loki.png 64@64
 * 
 * All bug reports / feature requests are to be filed here https://github.com/TricksterGuy/nin10kit/issues
 */

#ifndef SPRITES_H
#define SPRITES_H

#define SPRITES_PALETTE_TYPE (1 << 13)
#define SPRITES_DIMENSION_TYPE (1 << 6)

extern const unsigned short sprites_palette[255];
#define SPRITES_PALETTE_SIZE 510
#define SPRITES_PALETTE_LENGTH 255

extern const unsigned short sprites[5120];
#define SPRITES_SIZE 10240
#define SPRITES_LENGTH 5120

#define HULK_PALETTE_ID (0 << 12)
#define HULK_SPRITE_SHAPE (2 << 14)
#define HULK_SPRITE_SIZE (3 << 14)
#define HULK_ID 512

#define HELA_PALETTE_ID (0 << 12)
#define HELA_SPRITE_SHAPE (2 << 14)
#define HELA_SPRITE_SIZE (3 << 14)
#define HELA_ID 576

#define THOR_PALETTE_ID (0 << 12)
#define THOR_SPRITE_SHAPE (2 << 14)
#define THOR_SPRITE_SIZE (3 << 14)
#define THOR_ID 640

#define LOKI_PALETTE_ID (0 << 12)
#define LOKI_SPRITE_SHAPE (0 << 14)
#define LOKI_SPRITE_SIZE (3 << 14)
#define LOKI_ID 704

#endif

