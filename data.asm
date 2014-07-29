.include "hdr.asm"

.section ".rodata1" superfree

patkawaii:
.incbin "kawaii.pic"
patkawaii_end:

mapkawaii:
.incbin "kawaii.map"
mapkawaii_end:

palkawaii:
.incbin "kawaii.pal"
palkawaii_end:

pattilekawaii:
.incbin "tilekawaii.pic"
pattilekawaii_end:

maptilekawaii:
.incbin "tilekawaii.map"
maptilekawaii_end:

paltilekawaii:
.incbin "tilekawaii.pal"
paltilekawaii_end:

.ends

.section ".rodata2" superfree

gfxpsrite:
.incbin "sprites.pic"
gfxpsrite_end:

palsprite:
.incbin "sprites.pal"
palsprite_end:

soundbrr:
.incbin "hihi.brr"
soundbrrend:

snesfont:
.incbin "snesfont.pic"

.ends