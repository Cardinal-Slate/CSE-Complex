/// cse/complex.h — the Complex domain: a value is a pair (re, im) over a component field. Generic over
/// cse_field — this repo never names its component. Supply Arith's field and it is Gaussian ℚ[i]; supply
/// a reals field and it is ℂ. Stacks on the spine + CSE-DSA + the field *interface* (not a field).
/// SPDX-License-Identifier: MIT OR Apache-2.0
#pragma once
#include "slate/psda.h"
#include "cse/field.h"

/// build re + im·i, and read the parts.
slate_psda *cse_complex(slate_psda **pool, slate_psda *re, slate_psda *im);
slate_psda *cse_complex_re(slate_psda *z);
slate_psda *cse_complex_im(slate_psda *z);

/// (a+bi) + (c+di) = (a+c) + (b+d)i   — over the given field F.
slate_psda *cse_complex_add(const cse_field *F, slate_psda **pool, slate_psda *x, slate_psda *y);
/// (a+bi)(c+di) = (ac−bd) + (ad+bc)i
slate_psda *cse_complex_mul(const cse_field *F, slate_psda **pool, slate_psda *x, slate_psda *y);
/// conjugate: a − bi
slate_psda *cse_complex_conj(const cse_field *F, slate_psda **pool, slate_psda *z);
