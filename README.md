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

**Changes vs upstream (`stable` branch, commits `8550582`–`ef91d00`):**

| Commit | Description |
|--------|-------------|
| `8550582` | Phase B: integrate liboqs via pkg-config (Option B, dev-only) |
| `2a4c85a` | Phase D step 1: `SignatureHashP2QPK` — SIP-02a §3 sighash construction |
| `468f367` | Phase D step 2: `Init()` OP_2 trigger test for P2QPK precompute |
| `abb93a0` | Phase D step 3: `VerifyWitnessProgram` witver==2 branch + `SCRIPT_VERIFY_P2QPK` |
| `816cd06` | Phase D step 4: wire `OQS_SIG_slh_dsa_pure_sha2_128f_verify` into the spend path |
| `d005de1` | `CRegTestParams` + `CSigNetParams` stub |
| `ef91d00` | Regtest mining fix — yescrypt PoWHash + `fPowNoRetargeting` before DGW |

**Phase E status (regtest validation):** Blocks mined, P2QPK UTXO confirmed on-chain, 17,088-byte SLH-DSA spend transaction mined (`witness_unknown` pre-activation path). Remaining: add `DEPLOYMENT_P2QPK` to `CRegTestParams.vDeployments` with `ALWAYS_ACTIVE`, set `SCRIPT_VERIFY_P2QPK` in regtest policy flags, re-confirm spend under full verification.

## SLH-DSA constants

| Property | Value |
|----------|-------|
| Algorithm | SLH-DSA-SHA2-128f (FIPS 205) |
| Public key | 32 bytes |
| Signature | 17,088 bytes |
| Witness version | 2 |
| Address prefix | `bq` (Bech32m) |

## Status

- **Pre-activation:** witver==2 outputs with 32-byte program are anyone-can-spend (correct per SIP-02 §3.4).
- **Post-activation:** `SCRIPT_VERIFY_P2QPK` flag triggers full SLH-DSA verification via liboqs.
- **Do not send funds of value** to P2QPK addresses on mainnet before soft fork activation.
- liboqs integration uses **Option B** (host pkg-config at `/usr/local`) — dev/Phase D-E only. Option A (`depends/packages/liboqs.mk`) is required before any consensus-build merge.

## Governance

Activation parameters (BIP9 bit, start/timeout heights) are a SAOGEN governance decision. See [docs/sips/](docs/sips/) for the full SIP-QOGE-PQC-02 specification.

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
