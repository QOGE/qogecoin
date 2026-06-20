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
