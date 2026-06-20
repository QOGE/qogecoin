// Copyright (c) 2026 The Bitcoin and Qogecoin Core Authors
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.
//
// Unit tests for SIP-QOGE-PQC-02a SignatureHashP2QPK.
// Phase D, step 1: sighash function in isolation (no VerifyWitnessProgram, no liboqs call).

#include <script/interpreter.h>
#include <primitives/transaction.h>
#include <script/script.h>
#include <streams.h>
#include <test/data/bip341_wallet_vectors.json.h>
#include <test/util/setup_common.h>
#include <uint256.h>
#include <util/strencodings.h>

#include <boost/test/unit_test.hpp>
#include <univalue.h>

BOOST_FIXTURE_TEST_SUITE(script_p2qpk_tests, BasicTestingSetup)

// Reproduces SIP-QOGE-PQC-02a §6 Open Item 2 test vector using the real C++ implementation.
// Input data: BIP341 wallet vectors keyPathSpending[0] (same tx and UTXOs used to cross-validate
// TapSighash; see SIP-QOGE-PQC-02a §6/Open Item 2 and QOGE_P2QPK_PQC_Independent_Review.md).
// Expected output independently recomputed by GPT-5.5 Thinking (20 June 2026) and confirmed.
BOOST_AUTO_TEST_CASE(p2qpksighash_test_vector)
{
    UniValue tests;
    tests.read((const char*)json_tests::bip341_wallet_vectors, sizeof(json_tests::bip341_wallet_vectors));

    const auto& vec = tests["keyPathSpending"].getValues()[0];

    // Decode the unsigned transaction.
    CMutableTransaction tx;
    SpanReader{SER_NETWORK, PROTOCOL_VERSION, ParseHex(vec["given"]["rawUnsignedTx"].get_str())} >> tx;

    // Decode the UTXOs being spent (one per input, in order).
    std::vector<CTxOut> utxos;
    for (const auto& utxo : vec["given"]["utxosSpent"].getValues()) {
        auto script_bytes = ParseHex(utxo["scriptPubKey"].get_str());
        CScript script{script_bytes.begin(), script_bytes.end()};
        CAmount amount{utxo["amountSats"].get_int()};
        utxos.emplace_back(amount, script);
    }

    // Precompute with force=true so all five hash fields are populated regardless of
    // the UTXOs' witness version. (The BIP341 vectors use Taproot/witver-1 scriptPubKeys;
    // in production with real P2QPK/witver-2 inputs the Init() OP_2 extension ensures
    // m_spent_amounts_single_hash and m_spent_scripts_single_hash are populated without force.)
    PrecomputedTransactionData txdata;
    txdata.Init(tx, std::vector<CTxOut>{utxos}, /*force=*/true);
    BOOST_REQUIRE(txdata.m_bip341_taproot_ready);
    BOOST_REQUIRE(txdata.m_spent_outputs_ready);

    // Verify the five intermediary hashes against BIP341 reference values.
    // This confirms the precomputed fields match the independent cross-validation data
    // from SIP-QOGE-PQC-02a §6/Open Item 2 before passing them to SignatureHashP2QPK.
    const auto& imd = vec["intermediary"];
    BOOST_CHECK_EQUAL(HexStr(txdata.m_prevouts_single_hash),
        imd["hashPrevouts"].get_str());
    BOOST_CHECK_EQUAL(HexStr(txdata.m_spent_amounts_single_hash),
        imd["hashAmounts"].get_str());
    BOOST_CHECK_EQUAL(HexStr(txdata.m_spent_scripts_single_hash),
        imd["hashScriptPubkeys"].get_str());
    BOOST_CHECK_EQUAL(HexStr(txdata.m_sequences_single_hash),
        imd["hashSequences"].get_str());
    BOOST_CHECK_EQUAL(HexStr(txdata.m_outputs_single_hash),
        imd["hashOutputs"].get_str());

    // Compute P2QPKSighash for input 0 and verify against the SIP-QOGE-PQC-02a test vector.
    //
    // Preimage (174 bytes):
    //   TaggedHash("P2QPKSighash";
    //     0x00           epoch
    //     0x01           SIGHASH_ALL (fixed v1)
    //     02000000       nVersion (LE32)
    //     0065cd1d       nLockTime (LE32 = 500,000,000)
    //     e3b33bb4...    m_prevouts_single_hash
    //     58a6964a...    m_spent_amounts_single_hash
    //     23ad0f61...    m_spent_scripts_single_hash
    //     18959c72...    m_sequences_single_hash
    //     a2e6dab7...    m_outputs_single_hash
    //     00000000       in_pos=0 (LE32)
    //   )
    uint256 sighash;
    BOOST_REQUIRE(SignatureHashP2QPK(sighash, tx, /*in_pos=*/0, txdata, MissingDataBehavior::FAIL));

    BOOST_CHECK_EQUAL(HexStr(sighash),
        "8a17f83ed68457d5469f4bbcfc68ddaeaa70739522c1b6fb76685ba7b2008c38");
}

// Safeguard D (SIP-QOGE-PQC-02a §7-D): verify that Init() without force=true detects a
// witver==2 (OP_2) spent scriptPubKey and sets m_bip341_taproot_ready, populating
// m_spent_amounts_single_hash and m_spent_scripts_single_hash through the normal code path.
// This tests the OP_1||OP_2 trigger in Init() itself — the thing that ensures precomputed
// data is available when SignatureHashP2QPK is later called from VerifyWitnessProgram.
BOOST_AUTO_TEST_CASE(init_op2_trigger_sets_taproot_ready_flag)
{
    // Build a minimal transaction with one witness-bearing input (witness must be non-null
    // for Init() to enter the scriptPubKey detection branch).
    CMutableTransaction tx;
    tx.nVersion = 2;
    tx.nLockTime = 0;
    tx.vin.resize(1);
    tx.vin[0].prevout.hash = uint256::ONE;
    tx.vin[0].prevout.n = 0;
    tx.vin[0].nSequence = 0xffffffff;
    // Non-empty witness stack — any byte will do; Init() only checks IsNull().
    tx.vin[0].scriptWitness.stack.push_back({0x00});
    tx.vout.resize(1);
    tx.vout[0].nValue = 1000;
    tx.vout[0].scriptPubKey = CScript() << OP_0 << std::vector<unsigned char>(20, 0x00);

    // Construct a P2QPK scriptPubKey: OP_2 <32-byte program>.
    // This is the witness-v2 output being spent by the input above.
    std::vector<unsigned char> program(32, 0xab); // arbitrary 32-byte witness program
    CScript p2qpk_script = CScript() << OP_2 << program;
    BOOST_REQUIRE_EQUAL(p2qpk_script.size(), 34U); // sanity: OP_2 + pushdata(32)

    std::vector<CTxOut> spent_outputs;
    spent_outputs.emplace_back(CAmount{50000}, p2qpk_script);

    // Init() without force=true — must detect OP_2 and set m_bip341_taproot_ready.
    PrecomputedTransactionData txdata;
    txdata.Init(tx, std::vector<CTxOut>{spent_outputs});

    // The OP_2 trigger is the whole point of this test.
    BOOST_CHECK(txdata.m_bip341_taproot_ready);
    BOOST_CHECK(txdata.m_spent_outputs_ready);

    // Both spent-amount and spent-script hashes must be populated (not default-zero),
    // since SignatureHashP2QPK reads them unconditionally. A zero hash here would mean
    // the trigger fired but the computation was skipped — that would be a consensus bug.
    BOOST_CHECK(txdata.m_spent_amounts_single_hash != uint256());
    BOOST_CHECK(txdata.m_spent_scripts_single_hash != uint256());

    // m_bip143_segwit_ready must NOT be set: a pure P2QPK spend should not trigger
    // the BIP143 path (that would be a false-positive that wastes work, or worse,
    // could indicate the else-branch is still firing for OP_2 inputs).
    BOOST_CHECK(!txdata.m_bip143_segwit_ready);
}

// Verify that the missing-data guard returns false (not crash) when precomputed data absent.
BOOST_AUTO_TEST_CASE(p2qpksighash_missing_data_returns_false)
{
    CMutableTransaction tx;
    tx.vin.resize(1);
    tx.vout.resize(1);

    PrecomputedTransactionData txdata; // not initialized — m_bip341_taproot_ready == false

    uint256 sighash;
    BOOST_CHECK(!SignatureHashP2QPK(sighash, tx, 0, txdata, MissingDataBehavior::FAIL));
}

BOOST_AUTO_TEST_SUITE_END()
