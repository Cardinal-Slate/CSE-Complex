/* src/complex.c — Complex over an opaque component field. It composes F's operations on a CSE-DSA cell;
   it does not know, name, or link the concrete field. Slate-only.
   SPDX-License-Identifier: MIT OR Apache-2.0 */
#include "cse/complex.h"
#include "cse/dsa/cell.h"

slate_psda *cse_complex(slate_psda **pool, slate_psda *re, slate_psda *im) {
  return cse_cell(cse_cell_take(pool), re, im, 0);   /* re = payload, im = rest */
}
slate_psda *cse_complex_re(slate_psda *z) { return cse_cell_payload(z); }
slate_psda *cse_complex_im(slate_psda *z) { return cse_cell_rest(z); }

slate_psda *cse_complex_add(const cse_field *F, slate_psda **p, slate_psda *x, slate_psda *y) {
  return cse_complex(p,
    F->add(p, cse_complex_re(x), cse_complex_re(y)),
    F->add(p, cse_complex_im(x), cse_complex_im(y)));
}
slate_psda *cse_complex_mul(const cse_field *F, slate_psda **p, slate_psda *x, slate_psda *y) {
  slate_psda *a = cse_complex_re(x), *b = cse_complex_im(x);
  slate_psda *c = cse_complex_re(y), *d = cse_complex_im(y);
  slate_psda *re = F->sub(p, F->mul(p, a, c), F->mul(p, b, d));   /* ac − bd */
  slate_psda *im = F->add(p, F->mul(p, a, d), F->mul(p, b, c));   /* ad + bc */
  return cse_complex(p, re, im);
}
slate_psda *cse_complex_conj(const cse_field *F, slate_psda **p, slate_psda *z) {
  return cse_complex(p, cse_complex_re(z), F->neg(p, cse_complex_im(z)));
}
