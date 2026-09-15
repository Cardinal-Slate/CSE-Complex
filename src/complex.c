/* src/complex.c — Complex ℚ[i], type-first over opaque typing. A value is a ℚ pair (re, im) tagged with the
   Complex type via cse_typed: its content is a cell(re, im), its type is an identity handle. The component
   arithmetic is Arith's (cse_arith_*) — it stacks concretely on the layer below. Every op checks its
   carriers have type ℂ by identity (cse_type_has), never reading their content. Slate-only.
   SPDX-License-Identifier: MIT OR Apache-2.0 */
#include "cse/complex.h"
#include "cse/arith.h"
#include "cse/types.h"
#include "cse/dsa/cell.h"

/* the Complex type: an object with identity (its fields are never inspected) */
static slate_psda CTYPE;
slate_psda *cse_complex_type(void) { return &CTYPE; }

slate_psda *cse_complex(slate_psda **pool, slate_psda *re, slate_psda *im) {
  slate_psda *content = cse_cell(cse_cell_take(pool), re, im, 0);   /* re = payload, im = rest */
  return cse_typed(cse_cell_take(pool), content, &CTYPE);
}
slate_psda *cse_complex_re(slate_psda *z) { return cse_cell_payload(cse_content(z)); }
slate_psda *cse_complex_im(slate_psda *z) { return cse_cell_rest(cse_content(z)); }

slate_psda *cse_complex_add(slate_psda **p, slate_psda *x, slate_psda *y) {
  if (!cse_type_has(x, &CTYPE) || !cse_type_has(y, &CTYPE)) return 0;
  return cse_complex(p,
    cse_arith_add(p, cse_complex_re(x), cse_complex_re(y)),
    cse_arith_add(p, cse_complex_im(x), cse_complex_im(y)));
}
slate_psda *cse_complex_mul(slate_psda **p, slate_psda *x, slate_psda *y) {
  if (!cse_type_has(x, &CTYPE) || !cse_type_has(y, &CTYPE)) return 0;
  slate_psda *a = cse_complex_re(x), *b = cse_complex_im(x);
  slate_psda *c = cse_complex_re(y), *d = cse_complex_im(y);
  slate_psda *re = cse_arith_sub(p, cse_arith_mul(p, a, c), cse_arith_mul(p, b, d));   /* ac − bd */
  slate_psda *im = cse_arith_add(p, cse_arith_mul(p, a, d), cse_arith_mul(p, b, c));   /* ad + bc */
  return cse_complex(p, re, im);
}
slate_psda *cse_complex_conj(slate_psda **p, slate_psda *z) {
  if (!cse_type_has(z, &CTYPE)) return 0;
  return cse_complex(p, cse_complex_re(z), cse_arith_neg(p, cse_complex_im(z)));
}
