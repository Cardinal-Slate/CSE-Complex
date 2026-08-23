/* tests/complex.c — Gaussian ℚ[i], type-first. No field is wired; Complex stacks on Arith directly, and
   a value carries the ℂ tag. The op refuses a foreign carrier.
   SPDX-License-Identifier: MIT OR Apache-2.0 */
#include <stdio.h>
#include "cse/complex.h"
#include "cse/arith.h"
#include "cse/types.h"
#include "prime.h"        /* Arith's plain-integer boundary, to read components */
#include "slate/psda.h"

static int fails = 0;
#define CHECK(c, m) do { if (!(c)) { printf("  FAIL %s\n", (m)); fails++; } } while (0)

static slate_psda arena[1024];
static slate_psda *pool;
static void pool_init(void) { int i; for (i = 0; i < 1023; i++) arena[i].next = &arena[i + 1]; arena[1023].next = 0; pool = &arena[0]; }

static slate_psda *C(long re, long im) {
  return cse_complex(&pool, cse_arith_val(&pool, re, 1), cse_arith_val(&pool, im, 1));
}
static int isc(slate_psda *z, long re, long im) {
  long n, d;
  cse_arith_read(cse_complex_re(z), &n, &d); if (n != re || d != 1) return 0;
  cse_arith_read(cse_complex_im(z), &n, &d); return n == im && d == 1;
}

int main(void) {
  pool_init();
  slate_psda *a = C(1, 2), *b = C(3, 4);

  CHECK(isc(cse_complex_add(&pool, a, b), 4, 6), "(1+2i) + (3+4i) = 4+6i");
  CHECK(isc(cse_complex_mul(&pool, a, b), -5, 10), "(1+2i)(3+4i) = −5+10i");
  CHECK(isc(cse_complex_conj(&pool, a), 1, -2), "conj(1+2i) = 1−2i");
  CHECK(isc(cse_complex_mul(&pool, a, cse_complex_conj(&pool, a)), 5, 0), "(1+2i)(1−2i) = 5");
  CHECK(isc(cse_complex_mul(&pool, C(0,1), C(0,1)), -1, 0), "i·i = −1");

  /* type-first: a value has the ℂ tag, and mul restricts its carrier to ℂ */
  CHECK(cse_type_has(a, cse_complex_type()) != 0, "a value has type ℂ");
  CHECK(cse_complex_mul(&pool, cse_arith_val(&pool, 2, 1), b) == 0, "mul refuses a ℚ where ℂ is required");

  printf(fails ? "complex: FAIL\n" : "complex: ok\n");
  return fails ? 1 : 0;
}
