# Why Migration to the P2QPK-HNDL defence Soft Fork by 2030 Is Not Optional
## A Technical and Strategic Assessment of Qogecoin's Post-Quantum Position

**Document status:** SIP-C v2.0 | SAOGEN SAO | SAOGEN AGS 

**Date:** 14 October 2026

---

## 1. The Original Qogecoin Repository: An Honest Assessment

Link: https://github.com/qogecoin/qogecoin

Qogecoin was founded with an explicit post-quantum security goal, stated in
its whitepaper. 
However, a direct inspection of the `qogecoin/qogecoin` repository
(branch `stable`, current as of October 2026) reveals that no consensus-layer
implementation of post-quantum cryptography was ever built.

The evidence is specific and technical, not a matter of interpretation:

** <i class="fa fa-thumbs-down" aria-hidden="true"></i>. No regtest implementation exists.**
`CRegTestParams` — the chain parameters class for Qogecoin's isolated local
development and testing network — was never added to `chainparams.cpp`. The
function that selects chain parameters (`CreateChainParams`) has its error deliberately 
throw commented out to hide two missing chain classes. 
This is strong evidence of incomplete implementation than simply forgetting to add regtest.
It means the standard development environment for Bitcoin-derived consensus work was never
functional. This is not a small gap — it is the fingerprint of a
codebase that was cloned and configured but never seriously developed at the consensus
layer.

**  The mining loop uses the wrong hash function.**
`GenerateBlock` in `rpc/mining.cpp` calls `block.GetHash()` (SHA256d) to
find a valid proof-of-work nonce, while `AcceptBlock` validates with
`block.GetPoWHash()` (yescrypt, Qogecoin's actual PoW algorithm). These are
completely different functions. The mining loop produces a nonce that
satisfies SHA256d but fails yescrypt validation on every block. This serious bug would
be discovered in the first minute of any serious regtest session.

** 1c. DarkGravityWave ignores `fPowNoRetargeting`.**
Qogecoin replaced Bitcoin's difficulty adjustment algorithm with
DarkGravityWave (DGW). However, `GetNextWorkRequired` calls DGW
unconditionally, ignoring the `fPowNoRetargeting` flag that regtest sets to
disable difficulty adjustment during testing. The Bitcoin-derived code that
checks this flag is an unreachable dead code. This is a second bug that would
have been discovered immediately on any regtest mining attempt.

This is the honest starting point from which the P2QPK-HNDL defence soft fork was
developed by SAOGEN. 

The larger implication for SAOGEN's AGS is significant: 
Qogecoin P2QPK-HNDL defence development was not simply adding quantum-resistant signatures to an already prepared foundation. 
It required repairing the whole Qogecoin's infrastructure needed to develop and implement consensus changes safely.

---

## 2. The Threat Model: Why Classical Addresses Cannot Be Left in Place post 2030

Qogecoin, like Bitcoin and all Bitcoin-derived chains, uses secp256k1 elliptic
curve cryptography for address generation and transaction signing. Every QOGE
address is derived from a secp256k1 public key.

The security of secp256k1 depends on the computational hardness of the elliptic
curve discrete logarithm problem (ECDLP). A sufficiently powerful quantum
computer running Shor's algorithm solves ECDLP in polynomial time — meaning it
recovers the private key from the public key deterministically. This is not a
probabilistic attack. Given the public key, it produces the private key.

**When is the public key exposed?**

For any address that has been spent from: the public key was broadcast to the
network at spend time and is permanently recorded on-chain. It cannot be
removed. Any CRQC that scans the blockchain can target these keys immediately.

For any unspent address: the public key is hidden behind a hash (HASH160 or
HASH256 depending on address type). However, the moment the owner attempts to
spend — broadcasting a transaction that includes the public key in the witness
— the key is exposed in the mempool for the duration of the block interval.
On Qogecoin's 1-minute block time, this is a ~60-second window. A CRQC
operating faster than this window could recover the private key and broadcast
a competing transaction before the legitimate one confirms.

**The harvest-now-decrypt-later (HNDL) attack:**
A quantum adversary does not need to wait until a CRQC exists to begin
collecting data. Nation-state and well-resourced actors are archiving
blockchain data today with the explicit intent of decrypting it once quantum
computing reaches sufficient scale. Every secp256k1 public key ever broadcast
on a blockchain is already harvested. The threat is not hypothetical or future
— the collection phase is happening now. The decryption phase follows whenever
the hardware becomes available.

---

## 3. Why "Optional" P2QPK-HNDL  Migration Fails

An optional PQC migration — where P2QPK addresses are available but classical
secp256k1 addresses remain permanently valid — does not solve the problem. It
creates a two-tier chain where:

**Tier 1 (migrated):** P2QPK outputs protected by SLH-DSA (FIPS 205), with
public keys hidden at rest behind HASH256. A CRQC cannot attack what it cannot
see. Additional HNDL defence integrated in the Symbiont Wallet makes the PQC attacks infinitely harder.  

**Tier 2 (unmigrated):** Classical secp256k1 outputs, permanently vulnerable.
Every Qogecoin address in this tier with an exposed public key is a target.

The existence of Tier 2 undermines the entire system, for several reasons:

**Known exploit targets exist permanently.** A quantum adversary does not need
to attack the whole chain — it targets the specific classical outputs it has
already collected public keys for. As long as those outputs are valid and
spendable, the incentive to develop a CRQC to attack them remains.

**Economic confidence is undermined.** A chain where a known class of outputs
can be stolen by a sufficiently advanced attacker cannot credibly claim
post-quantum security. The uncertainty alone suppresses adoption and value.

**The founding promise is not fulfilled.** Qogecoin's whitepaper stated PQC as
a founding goal — not a feature option, but the chain's reason for existing.
A chain that offers PQC as opt-in while leaving classical addresses permanently
valid has not delivered on that promise.

**Low migration pressure without coordination produces worse outcomes.** If
migration is optional but widely understood to be necessary, individual holders
will migrate at different times and rates, leaving a long tail of vulnerable
outputs that are practically difficult to clean up. Mandatory migration with a
defined 1st January 2030 timeline creates a clean cutover with network-wide coordination.

---


## 5. The P2QPK-HNDL Migration Path 2026-2030

The P2QPK-HNDL soft fork is designed to be backward-compatible during the
transition period, using Bitcoin's standard soft fork mechanism.

**After a defined cutover period 2026-2030:** Classical Qogecoin secp256k1 addresses will be
deprecated. Unmigrated funds are subject to a governance-defined process
(timelock, community fund, or other mechanism to be determined via SIP-C
governance) that prevents them from remaining as permanent quantum-vulnerable
targets.


---

## 6. Summary

| Question | Answer |
|----------|--------|
| Did the original Qogecoin repo implement PQC? | No. The whitepaper stated it as a goal; no consensus-layer implementation existed. |
| Is the secp256k1 threat real? | Yes. Shor's algorithm deterministically recovers private keys from public keys. Collection of public keys is happening now. |
| Can optional migration solve this? | No. Permanently valid classical addresses remain exploitable targets indefinitely. |
| What has been built? | A complete P2QPK-HNDL soft fork: wallet, consensus rules, sighash, activated on mainnet. |
| Is the migration backward-compatible? | Yes, during the 2026-2030 transition. The soft fork mechanism allows staged adoption. |
| Is migration ultimately optional? | No. The founding promise of Qogecoin and the threat model both require it. |

---

*Governed under SIP-C v2.0 | SAOGEN SAO 

*Developed by | SAOGEN AGS 

*P2QPK-HNDL AGS Repository: https://github.com/QOGE/qogecoin*
