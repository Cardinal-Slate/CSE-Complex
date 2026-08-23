/// cse/complex.h — the Complex domain: ℚ[i], type-first. A value is a psda carrying the Complex type
/// (tag 'C'): payload = re, rest = im, each a ℚ value. It stacks *concretely* on Arith — the component
/// arithmetic is cse_arith_* — and names its type: you call cse_complex_mul, so the operands must be ℂ
/// carriers, checked (cse_type_has). No field is supplied; the method is the type, and the type restricts
/// the carrier. Stacks on the spine + CSE-DSA + CSE-Types + CSE-Arith.
/// SPDX-License-Identifier: MIT OR Apache-2.0
#pragma once
#include "slate/psda.h"

/// the Complex type ℂ — the tag a value must order level with to be accepted.
slate_psda *cse_complex_type(void);

/// build re + im·i (each a ℚ value), and read the parts.
slate_psda *cse_complex(slate_psda **pool, slate_psda *re, slate_psda *im);
slate_psda *cse_complex_re(slate_psda *z);
slate_psda *cse_complex_im(slate_psda *z);

/// (a+bi) + (c+di) = (a+c) + (b+d)i. Operands must be ℂ, or nothing.
slate_psda *cse_complex_add(slate_psda **pool, slate_psda *x, slate_psda *y);
/// (a+bi)(c+di) = (ac−bd) + (ad+bc)i
slate_psda *cse_complex_mul(slate_psda **pool, slate_psda *x, slate_psda *y);
/// conjugate: a − bi
slate_psda *cse_complex_conj(slate_psda **pool, slate_psda *z);
