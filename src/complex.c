/* src/complex.c — Complex ℚ[i], type-first. A value carries the tag 'C' (kernel/tag.c); its parts are ℚ
   values and its arithmetic is Arith's (cse_arith_*) — it stacks concretely on the layer below, no field
   passed in. Every op checks its carriers have type ℂ (cse_type_has); a foreign carrier is refused.
   Slate-only. SPDX-License-Identifier: MIT OR Apache-2.0 */
#include "cse/complex.h"
#include "cse/arith.h"
#include "cse/types.h"
#include "cse/dsa/cell.h"

slate_psda *cse_complex(slate_psda **pool, slate_psda *re, slate_psda *im) {
  return cse_cell(cse_cell_take(pool), re, im, cse_complex_type()->potential);   /* tag 'C', re/im = payload/rest */
}
slate_psda *cse_complex_re(slate_psda *z) { return cse_cell_payload(z); }
slate_psda *cse_complex_im(slate_psda *z) { return cse_cell_rest(z); }

slate_psda *cse_complex_add(slate_psda **p, slate_psda *x, slate_psda *y) {
  if (!cse_type_has(x, cse_complex_type()) || !cse_type_has(y, cse_complex_type())) return 0;
  return cse_complex(p,
    cse_arith_add(p, cse_complex_re(x), cse_complex_re(y)),
    cse_arith_add(p, cse_complex_im(x), cse_complex_im(y)));
}
slate_psda *cse_complex_mul(slate_psda **p, slate_psda *x, slate_psda *y) {
  if (!cse_type_has(x, cse_complex_type()) || !cse_type_has(y, cse_complex_type())) return 0;
  slate_psda *a = cse_complex_re(x), *b = cse_complex_im(x);
  slate_psda *c = cse_complex_re(y), *d = cse_complex_im(y);
  slate_psda *re = cse_arith_sub(p, cse_arith_mul(p, a, c), cse_arith_mul(p, b, d));   /* ac − bd */
  slate_psda *im = cse_arith_add(p, cse_arith_mul(p, a, d), cse_arith_mul(p, b, c));   /* ad + bc */
  return cse_complex(p, re, im);
}
slate_psda *cse_complex_conj(slate_psda **p, slate_psda *z) {
  if (!cse_type_has(z, cse_complex_type())) return 0;
  return cse_complex(p, cse_complex_re(z), cse_arith_neg(p, cse_complex_im(z)));
}
