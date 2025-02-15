SHELL := /bin/sh
.ONESHELL:
MAKEFLAGS += --warn-undefined-variables
MAKEFLAGS += --no-builtin-rules

all: BASEZ80.COM BASEZ80.EXE

FILES := main.asm exec.asm eval.asm math.asm asmb.asm data.asm dist.asm hook.asm cmos.asm debug.c osinit.c vdu.c timer.c nanoprintf.c

.PHONY: BASEZ80.COM
BASEZ80.COM:
	@mkdir -p ./bin
	@ez80-cc -DCOM_APP --lib v99x8-standard --output-dir  ./bin --optimise size BASEZ80.COM $(FILES)

.PHONY: BASEZ80.EXE
BASEZ80.EXE:
	@mkdir -p ./bin
	@ez80-cc -DEXE_APP --mem-model 2MB --lib v99x8-standard --output-dir  ./bin --optimise size BASEZ80.EXE $(FILES)

.PHONY: format
format: SHELL:=/bin/bash
format:
	@find \( -name "*.c" -o -name "*.h" \) -exec echo "formating {}" \; -exec ez80-clang-format -i {} \;
