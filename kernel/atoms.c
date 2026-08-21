/* kernel/atoms.c — Complex's imaginary unit and conjugation atom, spelled in bytes below the seam.
   SPDX-License-Identifier: MIT OR Apache-2.0 */
#include "slate/psda.h"

static const unsigned char A_I[]    = { 'i', 0 };
static const unsigned char A_CONJ[] = { 'K', 0 };   /* conjugate */

slate_reading cse_complex_i       = A_I;
slate_reading cse_complex_conj_op = A_CONJ;
