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
- **Symbiont Wallet test suite:** 61/61 passing (address 17, signer 7, keystore 17, wallet 20). ⚠️ Key generation is not yet deterministic from the seed (open item M1.3) — users must back up both the seed hex **and** `qoge_wallet.db`; the seed alone cannot recover the wallet.

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
