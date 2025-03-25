SHELL := /bin/sh
.ONESHELL:
MAKEFLAGS += --warn-undefined-variables
MAKEFLAGS += --no-builtin-rules

all: BBCeZ80.EXE

FILES := main.asm exec.asm eval.asm math.asm asmb.asm data.asm dist.asm hook.asm cmos.asm debug.c osinit.c vdu.c timer.c nanoprintf.c graphics.asm
FILES += vdu/variables.c vdu/vdu_22.c vdu/logic_to_physical_coords.c vdu/vdu_12.c vdu/vdu_17.c vdu/fonts.c vdu/sysfont.asm

.PHONY: BBCeZ80.EXE
BBCeZ80.EXE:
	@mkdir -p ./bin
	@ez80-cc -DEXE_APP --mem-model 2MB --lib v99x8-hdmi --output-dir  ./bin --optimise size BBCeZ80.EXE $(FILES)

.PHONY: format
format: SHELL:=/bin/bash
format:
	@find \( -name "*.c" -o -name "*.h" \) -exec echo "formating {}" \; -exec ez80-clang-format -i {} \;
