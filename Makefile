# Makefile — CSE-Complex: the complex numbers (ℂ), Analysis + the imaginary unit i and conjugation.
#
# Build the deps first:
#   make -C ../CardinalSlate lib && make -C ../CSE-DSA lib && make -C ../CSE-Arith lib \
#     && make -C ../CSE-Field lib && make -C ../CSE-Analysis lib

SPINE    ?= ../CardinalSlate
DSA      ?= ../CSE-DSA
ARITH    ?= ../CSE-Arith
FIELD    ?= ../CSE-Field
ANALYSIS ?= ../CSE-Analysis
CC       ?= clang
CFLAGS := -std=c11 -Iinclude -I$(ANALYSIS)/include -I$(FIELD)/include -I$(ARITH)/include -I$(DSA)/include -I$(SPINE)/include -O2 -Wall -Wextra

OUT     := build
HDRS    := include/cse/complex.h
SRCS    := $(wildcard src/*.c)
OBJS    := $(patsubst src/%.c,$(OUT)/%.o,$(SRCS))
KOBJ    := $(OUT)/k_atoms.o
DEPLIBS := $(ANALYSIS)/build/libcse-analysis.a $(FIELD)/build/libcse-field.a $(ARITH)/build/libcse-arith.a $(DSA)/build/libcse-dsa.a

.PHONY: all check clean lib
all: check lib

$(OUT):
	@mkdir -p $(OUT)

# the slate-only gate covers include/ and src/ — kernel/ is the byte atoms, below the seam
$(OUT)/types.stamp: $(HDRS) $(SRCS) | $(OUT)
	@bad=$$(grep -rnE '\b(int|long|short|size_t|unsigned|char|bool|float|double)\b|void[[:space:]]*\*|stdint' include src 2>/dev/null || true); \
	  if [ -n "$$bad" ]; then printf "  %-10s C TYPE FOUND\n" "complex:"; printf '%s\n' "$$bad" | sed 's/^/    /'; exit 1; \
	  else printf "  %-10s only slate (kernel/ is below the seam)\n" "complex:"; fi; touch $@

$(OUT)/standalone.stamp: $(HDRS) | $(OUT)
	@for h in $(HDRS); do \
	  rel=$${h#include/}; \
	  printf '#include "%s"\nint main(void){return 0;}\n' "$$rel" > $(OUT)/one.c; \
	  $(CC) $(CFLAGS) -fsyntax-only $(OUT)/one.c || exit 1; \
	done; touch $@

$(OUT)/%.o: src/%.c $(HDRS) | $(OUT)
	@$(CC) $(CFLAGS) -c $< -o $@
$(OUT)/k_atoms.o: kernel/atoms.c $(HDRS) | $(OUT)
	@$(CC) $(CFLAGS) -c $< -o $@

lib: $(OUT)/libcse-complex.a
$(OUT)/libcse-complex.a: $(OBJS) $(KOBJ) | $(OUT)
	@ar rcs $@ $(OBJS) $(KOBJ)

$(OUT)/test_complex: tests/complex.c $(OBJS) $(KOBJ) | $(OUT)
	@$(CC) $(CFLAGS) tests/complex.c $(OBJS) $(KOBJ) $(DEPLIBS) -o $@

check: $(OUT)/types.stamp $(OUT)/standalone.stamp $(OUT)/test_complex
	@echo "== cse-complex =="; out=$$($(OUT)/test_complex 2>&1); st=$$?; \
	  if [ $$st -ne 0 ] || printf '%s' "$$out" | grep -q FAIL; then printf '%s\n' "$$out" | sed 's/^/  /'; exit 1; \
	  else printf '%s\n' "$$out" | sed 's/^/  /'; echo "== ALL PASS =="; fi

clean:
	@rm -rf $(OUT)
