/* kernel/tag.c — the Complex type identity: a psda whose potential is the tag 'C'. Below the seam,
   because a tag is raw reading bytes (like the spine's own order readings). Every ℂ value orders level
   with this node; that is what cse_type_has checks. SPDX-License-Identifier: MIT OR Apache-2.0 */
#include "cse/complex.h"
#include "slate/psda.h"

static const unsigned char TAG_C[] = { 'C', 0 };
static slate_psda CTYPE = { TAG_C, 0, 0 };
slate_psda *cse_complex_type(void) { return &CTYPE; }
