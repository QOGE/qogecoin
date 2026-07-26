Qogecoin — P2QPK Consensus Implementation (QOGE/qogecoin fork)
================================================================

<h1 align="center">
<img src="https://i.imgur.com/tPkmb9m.png" alt="Qogecoin" width="300"/>
</h1>

**This is a development fork implementing SIP-QOGE-PQC-02: post-quantum P2QPK (Pay-to-Quantum-Public-Key) consensus for Qogecoin Core.**

Upstream: https://github.com/qogecoin/qogecoin  
Wallet (symbiont-wallet): https://github.com/QOGE/symbiont-wallet

---

## What this fork adds

This fork implements the node-side of the SIP-QOGE-PQC-02 soft fork, which introduces a new P2QPK output type secured by SLH-DSA-SHA2-128f (FIPS 205) signatures. P2QPK outputs use witness version 2 and Bech32m addresses (`bq1z…`).

**Changes vs upstream (`stable` branch, commits `8550582`–`3262636a0`):**

| Commit | Description |
|--------|-------------|
| `8550582` | Phase B: integrate liboqs via pkg-config (Option B, dev-only) |
| `2a4c85a` | Phase D step 1: `SignatureHashP2QPK` — SIP-02a §3 sighash construction |
| `468f367` | Phase D step 2: `Init()` OP_2 trigger test for P2QPK precompute |
| `abb93a0` | Phase D step 3: `VerifyWitnessProgram` witver==2 branch + `SCRIPT_VERIFY_P2QPK` |
| `816cd06` | Phase D step 4: wire `OQS_SIG_slh_dsa_pure_sha2_128f_verify` into the spend path |
| `d005de1` | `CRegTestParams` + `CSigNetParams` stub |
| `ef91d00` | Regtest mining fix — yescrypt PoWHash + `fPowNoRetargeting` before DGW |
| `56a2aed` | Phase E: activate `DEPLOYMENT_P2QPK` in regtest — full SLH-DSA verification confirmed |
| `89812b7c` | Phase F prep: `DEPLOYMENT_P2QPK` in `CTestNetParams` (ALWAYS_ACTIVE, bit 3); `bech32_hrp = "bqt"`; `DeploymentInfo()` in `rpc/blockchain.cpp` wired for all chains |
| `09638b35` | Consensus safety: `BIP9Deployment` safe defaults (`bit{28}`, `nStartTime{NEVER_ACTIVE}`, `nTimeout{NEVER_ACTIVE}`); explicit `NEVER_ACTIVE` for `DEPLOYMENT_P2QPK` in `CMainParams` + `CSigNetParams` (per independent review) |
| `88c400c59` | Option A liboqs: `depends/packages/liboqs.mk` (0.15.0, static, `BUILD_TESTING=OFF`, `CMAKE_SYSTEM_PROCESSOR` fix); `configure.ac` Option A/B fallback; `packages.mk` updated |
| `135c2fc0b` | Fix static link: `BUILD_TESTING=OFF` in liboqs.mk; `$(LIBOQS_LIBS)` added to `qogecoin_tx`, `qogecoin_wallet`, `qogecoin_util` LDADD — verified: 5/5 P2QPK tests pass |
| `c00f6112d` | Fix `CTestNetParams`: `nRuleChangeActivationThreshold` 8064→1512 (75% of `nMinerConfirmationWindow=2016`); threshold previously exceeded window, making BIP9 lock-in structurally impossible on testnet |
| `3262636a0` | P2QPK mempool standardness: policy exception in `src/policy/policy.cpp` + `policy.h` — P2QPK spends now relay through standard mempools on mainnet |
| `061e88ea6` | Audit fix (comment-only): maintenance guardrail on `m_bip341_taproot_ready` gate in `SignatureHashP2QPK`; fix stale "liboqs stub" comment at witver==2 verify call |
| `7bade2229` | Audit 3 fixes: `static_assert(SLHDSA_PK_LEN == sizeof(uint256))` in `interpreter.cpp`; stale "liboqs stub / Phase D step 4" comments corrected in `interpreter.h` (×2) |
| `88888dc51` | Audit 2 fix: `PolicyScriptChecks` dynamically gates `SCRIPT_VERIFY_P2QPK` via `DeploymentActiveAfter` — resolves mempool invalid-sig acceptance, log spam, and `testmempoolaccept` false positive post-activation |
| — | **Real-parameter mainnet activation simulation (air-gapped, no commit):** Full BIP9 cycle confirmed at real mainnet params (`nMinerConfirmationWindow=2016`, `nRuleChangeActivationThreshold=1512`, genuine `nStartTime`): `DEFINED→STARTED` at 2016, `STARTED→LOCKED_IN` at 4032, `LOCKED_IN→ACTIVE` at 6048. Post-ACTIVE: real P2QPK spend confirmed; tampered spend rejected by `SCRIPT_VERIFY_P2QPK`. Closes last technical unknown before mainnet activation. |

**Phase E status: COMPLETE.** `DEPLOYMENT_P2QPK` added to `DeploymentPos` enum, `deploymentinfo.cpp`, and `CRegTestParams.vDeployments` (`ALWAYS_ACTIVE`). `DeploymentActiveAt(DEPLOYMENT_P2QPK)` gates `SCRIPT_VERIFY_P2QPK` in `GetBlockScriptFlags`. Validated on regtest: tampered-sig spend rejected (`SCRIPT_ERR_WITNESS_PROGRAM_MISMATCH` from `OQS_SIG_slh_dsa_pure_sha2_128f_verify`), real SLH-DSA spend accepted and confirmed on-chain.

**Phase F status: COMPLETE.** `DEPLOYMENT_P2QPK` in `CTestNetParams` (`ALWAYS_ACTIVE`, bit 3, `89812b7c`); `bech32_hrp = "bqt"`; `DeploymentInfo()` wired for all chains — `p2qpk: active: true` confirmed on testnet and regtest. `address.Network` + `bqt` HRP in Symbiont Wallet ([`83bbc73`](https://github.com/QOGE/symbiont-wallet/commit/83bbc73)). Option A liboqs depends build verified (`88c400c59`, `135c2fc0b`): `liboqs.a` (21 MB) installed; configure reports "Option A — static lib"; `script_p2qpk_tests` 5/5 pass. `nRuleChangeActivationThreshold` fixed to 1512/2016 (`c00f6112d`). Independent BIP9 parameter review: PASS. **Public testnet live at `167.86.81.222:42070`; P2QPK tx `357d4d0c...` confirmed in block 104.**

**Consensus safety fix (`09638b35`, per independent review).** `BIP9Deployment` struct fields `bit`, `nStartTime`, `nTimeout` lacked default member initializers, leaving the `DEPLOYMENT_P2QPK` slot in `CMainParams` and `CSigNetParams` with indeterminate values — a potential consensus-safety risk if `DeploymentActiveAt` or the versionbits state machine read them. Fixed: added `{28}`, `{NEVER_ACTIVE}`, `{NEVER_ACTIVE}` defaults to the struct, and explicitly configured `DEPLOYMENT_P2QPK` as `NEVER_ACTIVE` in both params classes. `NEVER_ACTIVE` deployments are correctly hidden from `getdeploymentinfo` (`DeploymentEnabled` returns false) — this is expected behavior, not a regression.

## SLH-DSA constants

| Property | Value |
|----------|-------|
| Algorithm | SLH-DSA-SHA2-128f (FIPS 205) |
| Public key | 32 bytes |
| Signature | 17,088 bytes |
| Witness version | 2 |
| Address prefix | `bq` (Bech32m, mainnet + regtest) / `bqt` (testnet) |

## Status

- **Pre-activation:** witver==2 outputs with 32-byte program are anyone-can-spend (correct per SIP-02 §3.4).
- **Post-activation:** `SCRIPT_VERIFY_P2QPK` flag triggers full SLH-DSA verification via liboqs.
- **Do not send funds of value** to P2QPK addresses on mainnet before soft fork activation.
- **P2QPK mempool standardness: COMPLETE** (`3262636a0`) — policy exception in `src/policy/policy.cpp` + `policy.h`; P2QPK spends relay through standard mempools on mainnet.
- liboqs integration: **Option A** (`depends/packages/liboqs.mk`, static, verified `135c2fc0b`) is the consensus build path. Option B (host pkg-config) was dev/Phase D-E only.
- **Symbiont Wallet test suite:** 72/72 passing (address 17, signer 11, keystore 17, wallet 27).
- **M1.3 — deterministic keygen: RESOLVED** (`98b1332`, `5342f1b` in symbiont-wallet). Keys are now deterministically derived from the master seed via HKDF + a liboqs RNG-hook mechanism. Forward-looking only — addresses generated before this fix remain DB-only recoverable; addresses generated after are fully seed-recoverable.
- **Testnet liboqs version alignment — COMPLETE.** The public testnet node at `167.86.81.222` was rebuilt against the pinned Option A liboqs 0.15.0 (previously running an unpinned `0.16.0-rc1` build), closing the reproducibility gap flagged in Audit 3.

## Audit status

**Audit 1 (sighash construction) — COMPLETE** (1–2 July 2026). Auditors: Claude Opus 4.8, ChatGPT 5.5, OpenAI Codex — independent, fresh context. Scope: `SignatureHashP2QPK` + SIP-QOGE-PQC-02a normative construction.

- Test vector `8a17f83ed68457d5469f4bbcfc68ddaeaa70739522c1b6fb76685ba7b2008c38` independently recomputed to exact match by all three models.
- Core security properties (cross-input reuse, cross-transaction replay, domain separation, length-extension): **unanimous PASS**.
- Q1 malleability framing disagreement (Codex FAIL narrow): acknowledged — inherited SegWit property, fund-safe, unfixable, wallet-avoided; documented in SIP-02a §8.
- Code fixes applied: sighash gate maintenance guardrail + stale "liboqs stub" comment corrected (`061e88ea6`, `b08e02108`).
- **No finding is a bottleneck for mainnet activation.**

Triage artifact: [`docs/sips/Audit_1_Sighash_Construction_Triage.md`](https://github.com/QOGE/symbiont-wallet/blob/main/docs/sips/Audit_1_Sighash_Construction_Triage.md)

**Audit 2 (witness verification) — COMPLETE** (5 July 2026). Auditors: Codex, Claude Opus 4.8, ChatGPT 5.5, Grok — independent, fresh context. Scope: P2QPK mempool policy path (`PolicyScriptChecks`, `STANDARD_SCRIPT_VERIFY_FLAGS`).

- **Bug confirmed:** `SCRIPT_VERIFY_P2QPK` was absent from `constexpr STANDARD_SCRIPT_VERIFY_FLAGS`; `PolicyScriptChecks` (`src/validation.cpp`) used this static set and never enforced SLH-DSA verification at the mempool policy layer post-activation.
- 3/4 auditors found the bug (Codex, Opus 4.8, ChatGPT 5.5). Grok PASS — correctly analyzed `GetBlockScriptFlags`/`ConnectBlock` path (which is correct) without separately examining `STANDARD_SCRIPT_VERIFY_FLAGS`.
- Fix disagreement: Opus proposed adding `SCRIPT_VERIFY_P2QPK` to the `constexpr` directly (wrong — would enforce SLH-DSA before activation, breaking pre-activation anyone-can-spend per SIP-02 §3.4); ChatGPT proposed dynamic `DeploymentActiveAfter` gate (correct). Resolved by direct code inspection.
- **Fix applied (`88888dc51`):** dynamic `DeploymentActiveAfter` gate in `PolicyScriptChecks` — same pattern as `AreInputsStandard` (`3262636a0`).
- `testmempoolaccept` false positive (`allowed:true` for invalid-sig P2QPK tx) discovered during verification — not by any auditor — confirmed fixed by the same commit. Verified live: `allowed:false`, `reject-reason: non-mandatory-script-verify-flag (Witness program hash mismatch)`.
- All three consequences of the single root cause resolved: mempool acceptance of invalid sigs, "BUG! PLEASE REPORT THIS!" log spam, `test_accept` false positive.
- **No finding is a bottleneck for mainnet activation** — block-connection path was always correct; however fix must be in place before mainnet activation.

Triage artifact: [`docs/sips/Audit_2_Witness_Verification_Triage.md`](https://github.com/QOGE/symbiont-wallet/blob/main/docs/sips/Audit_2_Witness_Verification_Triage.md)

**Audit 3 (liboqs integration) — COMPLETE** (6 July 2026). Six independent passes: OpenAI Codex (remote + local), Grok Build (local), Claude Opus 4.8 (remote, hash-verified liboqs tarball), ChatGPT 5.5 (remote, source-only), Claude Code (local, dispute resolution). Algorithm identifiers, size constants (32/64/17088), and static-linking design unanimously confirmed correct. Build-path dispute (Opus claimed Option B was committed; Codex/Grok Build empirically confirmed Option A via `ldd`/`readelf`) — resolved in favor of the empirical passes. M1.3 remediation path clarified: liboqs 0.15.0 has no seeded SIG keygen API; `OQS_randombytes_custom_algorithm()` hook identified as the path (subsequently implemented — see M1.3 above). **No finding is a bottleneck for mainnet activation.**

Triage artifact: [`docs/sips/Audit_3_liboqs_Integration_Triage_Summary.md`](https://github.com/QOGE/symbiont-wallet/blob/main/docs/sips/Audit_3_liboqs_Integration_Triage_Summary.md)

**Audit 4 (single-use address lifecycle) — COMPLETE** (7–9 July 2026, two-pass + three-pass convergence). Auditors: Grok Build, Claude Sonnet 4.6, Codex CLI. Original design redesigned: automatic key destruction on confirmation replaced with decoupled flagging (`OnConfirmation`, automatic, reversible) vs. destruction (`PurgeSpentKey`, optional, manual, irreversible, never automatic). Change-output routing enforcement added to `SignP2QPKInput`/`SignTransaction` (exact scriptPubKey binding check). **Overall verdict: PASS — ready for mainnet.**

Triage artifacts: [`docs/sips/Audit_4_single_use_lifecycle_triage_summary.md`](https://github.com/QOGE/symbiont-wallet/blob/main/docs/sips/Audit_4_single_use_lifecycle_triage_summary.md), [`docs/sips/Audit_4b_single_use_lifecycle_second_pass.md`](https://github.com/QOGE/symbiont-wallet/blob/main/docs/sips/Audit_4b_single_use_lifecycle_second_pass.md)

**Audit 5 (wallet lifecycle, unstructured) — COMPLETE** (6 July 2026). Codex CLI, self-directed read-only review. One false positive ruled out (address-reservation "bug" is intentional peek semantics). Two real findings fixed: retirement atomicity, and a `SignP2QPKInput` cross-check gap (`FromAddr` vs. spent UTXO script, `4f80192`).

Triage artifact: [`docs/sips/Audit_5_Wallet_Lifecycle_Triage_Summary.md`](https://github.com/QOGE/symbiont-wallet/blob/main/docs/sips/Audit_5_Wallet_Lifecycle_Triage_Summary.md)

## Real-Parameter Mainnet Activation Simulation

A full BIP9 activation cycle was run end-to-end on an isolated, air-gapped two-node local simulation using the REAL intended mainnet parameters — `nMinerConfirmationWindow=2016`, `nRuleChangeActivationThreshold=1512` (75%), a genuine future `nStartTime` — not the compressed test values used in an earlier mechanism-only simulation.

**Result: full activation cycle confirmed at exactly the predicted heights:**

| Phase | Height |
|---|---|
| `DEFINED → STARTED` | 2016 |
| `STARTED → LOCKED_IN` | 4032 |
| `LOCKED_IN → ACTIVE` | 6048 |

Post-`ACTIVE`, a real P2QPK spend (SLH-DSA-SHA2-128f) was constructed via `SignP2QPKInput`, broadcast, and confirmed via natural mining on both nodes independently. A second, deliberately tampered spend (three signature bytes flipped) was correctly rejected by `SCRIPT_VERIFY_P2QPK` (`non-mandatory-script-verify-flag (Witness program hash mismatch)`), confirming enforcement is genuinely live under real parameters, not just under the earlier mechanism-only test.

**This closes the last major open unknown before a real mainnet activation decision** — the BIP9 mechanism, the real window/threshold values, and P2QPK enforcement have all now been validated together, end-to-end, under realistic conditions. What remains is a governance decision on the real `nStartTime` and formal SIP ratification, not further technical validation.

## Governance

Activation parameters (BIP9 bit, start/timeout heights) are a SAOGEN governance decision. See [docs/sips/](https://github.com/QOGE/symbiont-wallet/tree/main/docs/sips) for the full SIP-QOGE-PQC-02 specification.

---

Whitepaper: [qogecoin_whitepaper.pdf](https://github.com/QOGE/qogecoin/blob/stable/doc/qogecoin_whitepaper.pdf)  
Website: https://qoge.org  
Discord: https://discord.gg/T8uYSDmtde  
Blockexplorer: https://explorer.qoge.org

Further information about Qogecoin is available in the [doc folder](/doc).

License
-------

Qogecoin is released under the terms of the MIT license. See [COPYING](COPYING) for more
information or see https://opensource.org/licenses/MIT.
