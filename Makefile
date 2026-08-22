# Makefile — CSE-Complex: a pair over a component field, generic over cse_field. The LIBRARY depends only
# on spine + CSE-DSA + the CSE-Field *interface* — never on a concrete field. The TEST wires in CSE-Arith
# as the field provider; that is the only place Arith is named.
#
# Build the deps first: make -C ../CardinalSlate lib && make -C ../CSE-DSA lib && make -C ../CSE-Arith lib

SPINE ?= ../CardinalSlate
DSA   ?= ../CSE-DSA
FIELD ?= ../CSE-Field
ARITH ?= ../CSE-Arith
CC    ?= clang
CFLAGS := -std=c11 -Iinclude -I$(FIELD)/include -I$(DSA)/include -I$(SPINE)/include -O2 -Wall -Wextra
TFLAGS := $(CFLAGS) -I$(ARITH)/include -I$(ARITH)/kernel

OUT     := build
HDRS    := include/cse/complex.h
SRCS    := $(wildcard src/*.c)
OBJS    := $(patsubst src/%.c,$(OUT)/%.o,$(SRCS))

.PHONY: all check clean lib
all: check lib

$(OUT):
	@mkdir -p $(OUT)

# slate-only gate — note the library source names no field; cse_field is an interface type
$(OUT)/types.stamp: $(HDRS) $(SRCS) | $(OUT)
	@bad=$$(grep -rnE '\b(int|long|short|size_t|unsigned|char|bool|float|double)\b|void[[:space:]]*\*|stdint' include src 2>/dev/null || true); \
	  if [ -n "$$bad" ]; then printf "  %-10s C TYPE FOUND\n" "complex:"; printf '%s\n' "$$bad" | sed 's/^/    /'; exit 1; \
	  else printf "  %-10s only slate · generic over a field\n" "complex:"; fi; touch $@

$(OUT)/standalone.stamp: $(HDRS) | $(OUT)
	@for h in $(HDRS); do rel=$${h#include/}; printf '#include "%s"\nint main(void){return 0;}\n' "$$rel" > $(OUT)/one.c; \
	  $(CC) $(CFLAGS) -fsyntax-only $(OUT)/one.c || exit 1; done; touch $@

$(OUT)/%.o: src/%.c $(HDRS) | $(OUT)
	@$(CC) $(CFLAGS) -c $< -o $@

lib: $(OUT)/libcse-complex.a
$(OUT)/libcse-complex.a: $(OBJS) | $(OUT)
	@ar rcs $@ $(OBJS)

# the test WIRES a provider: Complex (generic) + Arith (a field) — this is the only Arith reference
$(OUT)/test_complex: tests/complex.c $(OBJS) | $(OUT)
	@$(CC) $(TFLAGS) tests/complex.c $(OBJS) $(ARITH)/build/libcse-arith.a $(DSA)/build/libcse-dsa.a -o $@

check: $(OUT)/types.stamp $(OUT)/standalone.stamp $(OUT)/test_complex
	@echo "== cse-complex =="; out=$$($(OUT)/test_complex 2>&1); st=$$?; \
	  if [ $$st -ne 0 ] || printf '%s' "$$out" | grep -q FAIL; then printf '%s\n' "$$out" | sed 's/^/  /'; exit 1; \
	  else printf '%s\n' "$$out" | sed 's/^/  /'; echo "== ALL PASS =="; fi

clean:
	@rm -rf $(OUT)
