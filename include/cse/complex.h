/// cse/complex.h — the Complex domain: ℂ. Analysis + the imaginary unit i and conjugation.
/// SPDX-License-Identifier: MIT OR Apache-2.0
///
/// Complex stacks on Analysis (ℝ[i]). It adds one element, i, with i²=−1, and one operation,
/// conjugation, which is a field automorphism. The field/ring/transcendental laws are inherited; these
/// are what "complex" adds. Algebraic closure (every polynomial has a root) is the domain's headline
/// property but not an equational law — the operational core below is i²=−1 and the conjugation laws.
/// Operations build structure; `≡` is the spine's order at level; reduction is a provider's job.
#pragma once
#include "slate/psda.h"
#include "cse/analysis.h"   /* Complex = Analysis + i */

/// The imaginary unit (a constant atom) and the conjugation operation atom.
extern slate_reading cse_complex_i;        /* i */
extern slate_reading cse_complex_conj_op;  /* conjugate */

/// The operations — each builds a composition drawn from `pool`; none computes.
slate_psda *cse_complex_i_val(slate_psda **pool);                 /* i        */
slate_psda *cse_complex_conj (slate_psda **pool, slate_psda *a);  /* conj(a)  */

/// Equivalence — the spine's order at level (reduction supplied by a provider).
slate_psda *cse_complex_eq(slate_psda *x, slate_psda *y);

/// ─── the invariants Complex adds ────────────────────────────────────────────────────────────────────
///   i_sq        i · i ≡ −1
///   conj_conj   conj(conj(a)) ≡ a
///   conj_add    conj(a+b) ≡ conj(a) + conj(b)
///   conj_mul    conj(a·b) ≡ conj(a) · conj(b)
///   conj_i      conj(i) ≡ −i
slate_psda *cse_complex_i_sq     (slate_psda **s);
slate_psda *cse_complex_conj_conj(slate_psda **s, slate_psda *a);
slate_psda *cse_complex_conj_add (slate_psda **s, slate_psda *a, slate_psda *b);
slate_psda *cse_complex_conj_mul (slate_psda **s, slate_psda *a, slate_psda *b);
slate_psda *cse_complex_conj_i   (slate_psda **s);
