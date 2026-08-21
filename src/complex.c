/* src/complex.c — the Complex domain: i²=−1 and conjugation as a field automorphism. Slate-only.
   SPDX-License-Identifier: MIT OR Apache-2.0

   i is a constant; conj builds a composition. The ring/field operations come from Arith. The five laws
   below are what complex adds to the reals. Equivalence is the spine's order at level; reduction is a
   provider's job. */
#include "cse/complex.h"
#include "cse/dsa/cell.h"

slate_psda *cse_complex_i_val(slate_psda **pool) {
  slate_psda *c = cse_cell_take(pool); c->potential = cse_complex_i; c->prev = 0; c->next = 0; return c;
}
slate_psda *cse_complex_conj(slate_psda **pool, slate_psda *a) {
  return cse_cell(cse_cell_take(pool), a, 0, cse_complex_conj_op);
}

slate_psda *cse_complex_eq(slate_psda *x, slate_psda *y) {
  return (slate_order(x, y).potential == slate_level()) ? x : 0;
}

/* i · i ≡ −1 */
slate_psda *cse_complex_i_sq(slate_psda **s) {
  slate_psda *i = cse_complex_i_val(s);
  return cse_complex_eq(cse_arith_mul(s, i, i), cse_arith_neg(s, cse_arith_one_val(s)));
}
/* conj(conj(a)) ≡ a */
slate_psda *cse_complex_conj_conj(slate_psda **s, slate_psda *a) {
  return cse_complex_eq(cse_complex_conj(s, cse_complex_conj(s, a)), a);
}
/* conj(a+b) ≡ conj(a) + conj(b) */
slate_psda *cse_complex_conj_add(slate_psda **s, slate_psda *a, slate_psda *b) {
  return cse_complex_eq(cse_complex_conj(s, cse_arith_add(s, a, b)),
                        cse_arith_add(s, cse_complex_conj(s, a), cse_complex_conj(s, b)));
}
/* conj(a·b) ≡ conj(a) · conj(b) */
slate_psda *cse_complex_conj_mul(slate_psda **s, slate_psda *a, slate_psda *b) {
  return cse_complex_eq(cse_complex_conj(s, cse_arith_mul(s, a, b)),
                        cse_arith_mul(s, cse_complex_conj(s, a), cse_complex_conj(s, b)));
}
/* conj(i) ≡ −i */
slate_psda *cse_complex_conj_i(slate_psda **s) {
  return cse_complex_eq(cse_complex_conj(s, cse_complex_i_val(s)),
                        cse_arith_neg(s, cse_complex_i_val(s)));
}
