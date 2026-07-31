# SIP-2.0-AMD-012-PQC-MAINNET-ACTIVATION

**[NUMBERING NOTE: "AMD-012" is a placeholder. SIP-2.0-AMD-001-PQC.md is
already assigned to an earlier qoge.org legal/governance deliverable —
confirm the actual next available amendment number against the SAOGEN IAR
registry before filing.]**

| Metadata | Details |
| :--- | :--- |
| **Status** | Proposed — pending Architect ratification |
| **Amends** | SIP-QOGE-PQC-02 (P2QPK Consensus Integration) §3.4, §5, §7 |
| **Author** | SAOGEN (Architect & Founder) |
| **AI Node contribution** | Claude, Anthropic — Origin Layer A, under SIP-C v2.0 |
| **Date drafted** | 2026-07-26 |
| **Governance framework** | SIP-C v2.0 |

---

## 1. Purpose

This amendment formally ratifies the real mainnet BIP9 activation
parameters for the P2QPK post-quantum soft fork (`DEPLOYMENT_P2QPK`),
transitioning them from a technically-verified, independently-reviewed
source commit into a sanctioned SAOGEN governance decision.

Every technical step that produced this outcome — the audit series, the
M1.3 remediation, both BIP9 simulations, and the parameter commit itself
— was undertaken under the Architect's formal intent to bring the P2QPK
soft fork to real mainnet activation. This amendment is the point at
which that intent crystallizes into a ratified decision: the numbers are
no longer merely correct, they are authorized.

## 2. Background and Provenance

The following work establishes the evidentiary basis for this decision.
All items are complete and independently verified; none remain open.

### 2.1 Pre-mainnet audit series (Audits 1–5) — COMPLETE

| Audit | Scope | Outcome |
| :---: | :--- | :--- |
| 1 | Sighash construction | 3 models, sound |
| 2 | Witness verification | Real bug found and fixed (mempool policy gate) |
| 3 | liboqs integration | 6 independent passes, no blocker |
| 4 | Single-use address lifecycle | Redesigned (flag/destroy decoupling), PASS |
| 5 | Wallet lifecycle (unstructured) | 1 false positive ruled out, 2 real fixes |

Full triage artifacts: `docs/sips/Audit_*_Triage_Summary.md`
(symbiont-wallet).

### 2.2 M1.3 — deterministic SLH-DSA keygen — RESOLVED

Implemented (`98b1332`, symbiont-wallet): keys derived deterministically
from the wallet master seed via HKDF and a liboqs custom-RNG hook,
closing the "seed alone is insufficient for recovery" gap identified in
Audit 3. Independently reviewed twice (Codex, Grok Build); both
converged on the same structural fix. One additional concurrency bug
found and fixed during review (`5342f1b`). 73/73 tests pass,
race-detector clean.

### 2.3 BIP9 mechanism simulation (compressed parameters)

An initial air-gapped, two-node local simulation validated the BIP9
state machine itself — `DEFINED → STARTED → LOCKED_IN → ACTIVE` — using
compressed test parameters (20-block window) to observe the full
mechanism in a short session.

### 2.4 BIP9 real-parameter simulation

A second air-gapped, two-node simulation validated the mechanism again
using a genuine future `nStartTime` and a 2016-block signaling window
(the value then believed to be mainnet's; subsequently confirmed to be
testnet-scale — see §3 note below). Phase transitions confirmed at
exactly the predicted heights (2016 / 4032 / 6048). Post-`ACTIVE`, a real
P2QPK spend (SLH-DSA-SHA2-128f, via `SignP2QPKInput`) was broadcast and
confirmed via natural mining on both nodes independently. A second,
deliberately tampered spend (corrupted signature bytes) was correctly
rejected by `SCRIPT_VERIFY_P2QPK`
(`non-mandatory-script-verify-flag (Witness program hash mismatch)`),
confirming enforcement is genuinely live under realistic conditions.

### 2.5 Real mainnet parameter commit

Commit `7912fa1c1` (QOGE/qogecoin, branch `stable`/`main`) set the actual
`CMainParams` `DEPLOYMENT_P2QPK` activation timing. Pre-commit
verification against the live repository corrected an earlier
assumption: the real `CMainParams` window/threshold are `8064`/`6048`
(long-standing values, traced via `git log -L` to the file's 2022
introduction), not `2016`/`1512` as used in the §2.4 simulation. This
commit changed only `nStartTime` and `nTimeout` — window, threshold,
`bit`, and every other network's deployment entry were verified
untouched.

### 2.6 Independent review of the parameter commit

Two independent AI reviewers (Codex, Grok Build), each with direct
read-only filesystem access, verified the commit against the live
repository. Both confirmed, with independent arithmetic: exact parameter
values, exact commit scope (`src/chainparams.cpp` only, the intended two
lines only), complete isolation from testnet/regtest/signet, and intact
struct safe-defaults. Zero discrepancies on any parameter value between
the two reviews. One documentation-precision correction was identified
(the human-readable activation-timeline estimate) and folded into
project documentation before this amendment was drafted.

## 3. Ratified Parameters

The following `DEPLOYMENT_P2QPK` values in `CMainParams`
(`QOGE/qogecoin`, commit `7912fa1c1`) are hereby ratified as the sanctioned
mainnet activation configuration:

| Parameter | Value |
| :--- | :--- |
| `bit` | 3 |
| `nStartTime` | 1786284720 (2026-08-09 14:12 UTC) |
| `nTimeout` | 1794060720 (2026-11-07 14:12 UTC) |
| `nMinerConfirmationWindow` | 8064 (long-standing, unchanged by this commit) |
| `nRuleChangeActivationThreshold` | 6048 — 75% of 8064 (long-standing, unchanged) |
| `min_activation_height` | 0 |

**Note on window/threshold provenance:** the source code's own inline
comment claiming `nMinerConfirmationWindow = nPowTargetTimespan /
nPowTargetSpacing` does not compute to 8064 under QOGE's actual
constants (the formula yields 70,560). This is a pre-existing,
harmless documentation inaccuracy unrelated to this amendment or the
commit it ratifies; it does not affect the correctness or safety of the
ratified values, which are independently confirmed internally consistent
(exactly 75%) regardless of their original derivation.

## 4. Effect and Expected Timeline

Once a binary built from commit `7912fa1c1` (or any later commit that
preserves these values) is run by sufficient signaling hashrate, the
following timeline applies, given continuous successful signaling (the
expected case, given SAOGEN's coordinated mining participants):

| Milestone | Timing |
| :--- | :--- |
| `nStartTime` reached → `STARTED` phase begins | 0 to ~5.6 days after `nStartTime` (retarget-boundary alignment) |
| `STARTED` → `ACTIVE`, continuous signaling | exactly 11.2 days (two full 8064-block windows) |
| Worst case, `nStartTime` → `ACTIVE` | ~16.8 days |
| `nTimeout` margin beyond worst case | ~74 days |

`SCRIPT_VERIFY_P2QPK` enforcement — rejection of invalid P2QPK signatures
at the consensus level — takes effect at the moment `ACTIVE` is reached,
exactly as validated in the §2.4 simulation.

## 5. Operational Scope (Non-Blocking, Not Covered by This Ratification)

This amendment ratifies the *consensus parameters* only. The following
operational matters are tracked separately and do not affect the
validity of this ratification:

- Deployment of a mainnet-mode `qogecoind` instance reflecting this
  commit, on production infrastructure
- Pool infrastructure (`postquantum.qoge.org` or equivalent) for
  coordinated miner participation
- Direct coordination with SAOGEN's mining participants regarding
  binary deployment ahead of `nStartTime`
- Continued independent (Developer VM / witness-node) validation once
  real mainnet-mode mining begins

## 6. Formal Ratification

By the authority vested in the Architect under SIP-C v2.0 governance,
the parameters specified in §3 are hereby ratified as the official,
sanctioned mainnet activation configuration for the P2QPK post-quantum
soft fork, superseding the "governance decision pending" status
previously recorded in SIP-QOGE-PQC-02 §3.4.

This ratification is effective as of the date of Architect signature
below.

**Architect signature / date:** SAOGEN-SAO — 31 July 2026

---

*Attribution: SAOGEN (Architect & Founder). AI Node contribution to the
underlying technical work — audits, M1.3 remediation, simulation design
and execution, and this amendment's drafting — attributed to Claude
(Anthropic), Origin Layer A, under SIP-C v2.0.*
