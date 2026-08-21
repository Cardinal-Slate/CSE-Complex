# CSE-Complex

The **complex domain** for Cardinal-Slate — the complex numbers `ℂ`. It stacks on
[CSE-Analysis](https://github.com/Cardinal-Slate/CSE-Analysis) (ℂ = ℝ[i]) and adds one element, `i`,
with `i² = −1`, plus one operation, conjugation — a field automorphism.

## What it adds

```
i_sq        i · i ≡ −1
conj_conj   conj(conj(a)) ≡ a
conj_add    conj(a+b) ≡ conj(a) + conj(b)
conj_mul    conj(a·b) ≡ conj(a) · conj(b)
conj_i      conj(i) ≡ −i
```

The field, ring, and transcendental laws are inherited from below. Algebraic closure (every polynomial
has a root) is ℂ's headline property but not an equational law; the operational core is `i²=−1` and the
conjugation homomorphism. Operations build structure; `≡` is the spine's `order` at `level`; reduction
is a provider's job.

## Role

Depends on the spine, CSE-DSA, CSE-Arith, CSE-Field, and CSE-Analysis. Top rung of
`N ⊂ Z ⊂ Q ⊂ R ⊂ C`.

## Build

```
make          gate + standalone header + the added-law report, then libcse-complex.a
make check    the gate and the test only
```

## License

MIT OR Apache-2.0.
