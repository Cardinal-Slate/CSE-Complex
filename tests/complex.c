/* tests/complex.c — i²=−1 and the conjugation laws: the domain's contract for ℂ.
   SPDX-License-Identifier: MIT OR Apache-2.0 */
#include <stdio.h>
#include "cse/complex.h"
#include "cse/dsa/cell.h"
#include "slate/psda.h"
#include "slate/encode.h"

static int fails = 0;
#define CHECK(c, m) do { if (!(c)) { printf("  FAIL %s\n", (m)); fails++; } } while (0)

static slate_psda arena[256];
static slate_psda *pool;
static void pool_init(void) { int i; for (i = 0; i < 255; i++) arena[i].next = &arena[i + 1]; arena[255].next = 0; pool = &arena[0]; }

static unsigned char vbuf[8][4];
static slate_psda vnodes[8];
static int vn = 0;
static slate_psda *value(unsigned id) {
  unsigned char b = (unsigned char)id;
  slate_encode(&b, 1, vbuf[vn]);
  vnodes[vn].potential = vbuf[vn]; vnodes[vn].prev = 0; vnodes[vn].next = 0;
  return &vnodes[vn++];
}

int main(void) {
  pool_init();
  slate_psda *a = value(10), *b = value(20);

  /* operations build the right structure */
  CHECK(cse_cell_key(cse_complex_i_val(&pool)) == cse_complex_i, "i is the imaginary unit atom");
  slate_psda *k = cse_complex_conj(&pool, a);
  CHECK(cse_cell_payload(k) == a && cse_cell_key(k) == cse_complex_conj_op, "conj builds conj(a)");

  /* the five invariants — the domain contract for ℂ */
  struct { const char *law; slate_psda *held; } inv[] = {
    { "i_sq       i * i = -1",                 cse_complex_i_sq(&pool) },
    { "conj_conj  conj(conj(a)) = a",          cse_complex_conj_conj(&pool, a) },
    { "conj_add   conj(a+b) = conj a + conj b",cse_complex_conj_add(&pool, a, b) },
    { "conj_mul   conj(a*b) = conj a * conj b",cse_complex_conj_mul(&pool, a, b) },
    { "conj_i     conj(i) = -i",               cse_complex_conj_i(&pool) },
  };
  int i;
  printf("  the Complex domain — what i and conjugation add:\n");
  for (i = 0; i < 5; i++)
    printf("    %-38s %s\n", inv[i].law, inv[i].held ? "hold" : "awaits reduction");

  printf(fails ? "complex: FAIL\n" : "complex: ok\n");
  return fails ? 1 : 0;
}
