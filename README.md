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

**Changes vs upstream (`stable` branch, commits `8550582`–`89812b7c`):**

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

**Phase E status: COMPLETE.** `DEPLOYMENT_P2QPK` added to `DeploymentPos` enum, `deploymentinfo.cpp`, and `CRegTestParams.vDeployments` (`ALWAYS_ACTIVE`). `DeploymentActiveAt(DEPLOYMENT_P2QPK)` gates `SCRIPT_VERIFY_P2QPK` in `GetBlockScriptFlags`. Validated on regtest: tampered-sig spend rejected (`SCRIPT_ERR_WITNESS_PROGRAM_MISMATCH` from `OQS_SIG_slh_dsa_pure_sha2_128f_verify`), real SLH-DSA spend accepted and confirmed on-chain.

**Phase F status: IN PROGRESS.** `DEPLOYMENT_P2QPK` added to `CTestNetParams` (`ALWAYS_ACTIVE`, bit 3); `bech32_hrp = "bqt"` (distinguishes testnet P2QPK addresses from mainnet `bq`); `DeploymentInfo()` in `rpc/blockchain.cpp:1275` wired for all chains — `p2qpk: active: true` confirmed on both testnet and regtest. `address.Network` type + `bqt` HRP support added to Symbiont Wallet address package ([`83bbc73`](https://github.com/QOGE/symbiont-wallet/commit/83bbc73)). **Pending:** Option A liboqs build (`depends/packages/liboqs.mk`), `nRuleChangeActivationThreshold` fix on testnet, independent BIP9 parameter review, public testnet node launch.

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
- liboqs integration uses **Option B** (host pkg-config at `/usr/local`) — dev/Phase D-E only. Option A (`depends/packages/liboqs.mk`) is required before any consensus-build merge.

## Governance

Activation parameters (BIP9 bit, start/timeout heights) are a SAOGEN governance decision. See [docs/sips/](https://github.com/QOGE/symbiont-wallet/tree/main/docs/sips) for the full SIP-QOGE-PQC-02 specification.

---

Whitepaper: https://www.qogecoin.org/_downloads/fd0f7c423eaa00b6deef76295c7a9822/qogecoin_whitepaper.pdf  
Website: https://qoge.org  
Discord: https://discord.gg/T8uYSDmtde  
Blockexplorer: https://explorer.qoge.org

Further information about Qogecoin is available in the [doc folder](/doc).

License
-------

Qogecoin is released under the terms of the MIT license. See [COPYING](COPYING) for more
information or see https://opensource.org/licenses/MIT.
