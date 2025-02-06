SHELL := /bin/sh
.ONESHELL:
MAKEFLAGS += --warn-undefined-variables
MAKEFLAGS += --no-builtin-rules

all: BBCBASIC.COM

FILES := main.asm exec.asm eval.asm math.asm asmb.asm data.asm dist.asm hook.asm cmos.asm debug.c osinit.c vdu.c timer.c

.PHONY: BBCBASIC.COM
BBCBASIC.COM:
	@mkdir -p ./bin
	@ez80-cc --lib v99x8-standard --output-dir  ./bin --optimise size BBCBASIC.COM $(FILES)

.PHONY: format
format: SHELL:=/bin/bash
format:
	@find \( -name "*.c" -o -name "*.h" \) -exec echo "formating {}" \; -exec ez80-clang-format -i {} \;
