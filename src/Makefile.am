# Copyright (c) 2013-2016 The Bitcoin and Qogecoin Core Authors
# Distributed under the MIT software license, see the accompanying
# file COPYING or http://www.opensource.org/licenses/mit-license.php.

# Pattern rule to print variables, e.g. make print-top_srcdir
print-%: FORCE
	@echo '$*'='$($*)'

DIST_SUBDIRS = secp256k1

AM_LDFLAGS = $(LIBTOOL_LDFLAGS) $(HARDENED_LDFLAGS) $(GPROF_LDFLAGS) $(SANITIZER_LDFLAGS) $(LTO_LDFLAGS) $(CORE_LDFLAGS)
AM_CXXFLAGS = $(DEBUG_CXXFLAGS) $(HARDENED_CXXFLAGS) $(WARN_CXXFLAGS) $(NOWARN_CXXFLAGS) $(ERROR_CXXFLAGS) $(GPROF_CXXFLAGS) $(SANITIZER_CXXFLAGS) $(LTO_CXXFLAGS) $(CORE_CXXFLAGS)
AM_CPPFLAGS = $(DEBUG_CPPFLAGS) $(HARDENED_CPPFLAGS) $(CORE_CPPFLAGS)
AM_LIBTOOLFLAGS = --preserve-dup-deps
PTHREAD_FLAGS = $(PTHREAD_CFLAGS) $(PTHREAD_LIBS)
EXTRA_LIBRARIES =

lib_LTLIBRARIES =
noinst_LTLIBRARIES =

bin_PROGRAMS =
noinst_PROGRAMS =
TESTS =
BENCHMARKS =

QOGECOIN_INCLUDES=-I$(builddir) -I$(srcdir)/$(MINISKETCH_INCLUDE_DIR_INT) -I$(srcdir)/secp256k1/include -I$(srcdir)/$(UNIVALUE_INCLUDE_DIR_INT) $(BDB_CPPFLAGS) $(BOOST_CPPFLAGS) $(LEVELDB_CPPFLAGS)

LIBQOGECOIN_NODE=libqogecoin_node.a
LIBQOGECOIN_COMMON=libqogecoin_common.a
LIBQOGECOIN_CONSENSUS=libqogecoin_consensus.a
LIBQOGECOIN_CLI=libqogecoin_cli.a
LIBQOGECOIN_KERNEL=libqogecoin_kernel.a
LIBQOGECOIN_UTIL=libqogecoin_util.a
LIBQOGECOIN_CRYPTO_BASE=crypto/libqogecoin_crypto_base.la
LIBQOGECOINQT=qt/libqogecoinqt.a
LIBSECP256K1=secp256k1/libsecp256k1.la

if ENABLE_ZMQ
LIBQOGECOIN_ZMQ=libqogecoin_zmq.a
endif
if BUILD_QOGECOIN_LIBS
LIBQOGECOINCONSENSUS=libqogecoinconsensus.la
endif
if BUILD_QOGECOIN_KERNEL_LIB
LIBQOGECOINKERNEL=libqogecoinkernel.la
endif
if ENABLE_WALLET
LIBQOGECOIN_WALLET=libqogecoin_wallet.a
LIBQOGECOIN_WALLET_TOOL=libqogecoin_wallet_tool.a
endif

LIBQOGECOIN_CRYPTO = $(LIBQOGECOIN_CRYPTO_BASE)
if ENABLE_SSE41
LIBQOGECOIN_CRYPTO_SSE41 = crypto/libqogecoin_crypto_sse41.la
LIBQOGECOIN_CRYPTO += $(LIBQOGECOIN_CRYPTO_SSE41)
endif
if ENABLE_AVX2
LIBQOGECOIN_CRYPTO_AVX2 = crypto/libqogecoin_crypto_avx2.la
LIBQOGECOIN_CRYPTO += $(LIBQOGECOIN_CRYPTO_AVX2)
endif
if ENABLE_X86_SHANI
LIBQOGECOIN_CRYPTO_X86_SHANI = crypto/libqogecoin_crypto_x86_shani.la
LIBQOGECOIN_CRYPTO += $(LIBQOGECOIN_CRYPTO_X86_SHANI)
endif
if ENABLE_ARM_SHANI
LIBQOGECOIN_CRYPTO_ARM_SHANI = crypto/libqogecoin_crypto_arm_shani.la
LIBQOGECOIN_CRYPTO += $(LIBQOGECOIN_CRYPTO_ARM_SHANI)
endif
noinst_LTLIBRARIES += $(LIBQOGECOIN_CRYPTO)

$(LIBSECP256K1): $(wildcard secp256k1/src/*.h) $(wildcard secp256k1/src/*.c) $(wildcard secp256k1/include/*)
	$(AM_V_at)$(MAKE) $(AM_MAKEFLAGS) -C $(@D) $(@F)

# Make is not made aware of per-object dependencies to avoid limiting building parallelization
# But to build the less dependent modules first, we manually select their order here:
EXTRA_LIBRARIES += \
  $(LIBQOGECOIN_UTIL) \
  $(LIBQOGECOIN_COMMON) \
  $(LIBQOGECOIN_CONSENSUS) \
  $(LIBQOGECOIN_NODE) \
  $(LIBQOGECOIN_CLI) \
  $(LIBQOGECOIN_IPC) \
  $(LIBQOGECOIN_WALLET) \
  $(LIBQOGECOIN_WALLET_TOOL) \
  $(LIBQOGECOIN_ZMQ)

if BUILD_QOGECOIND
  bin_PROGRAMS += qogecoind
endif

if BUILD_QOGECOIN_NODE
  bin_PROGRAMS += qogecoin-node
endif

if BUILD_QOGECOIN_CLI
  bin_PROGRAMS += qogecoin-cli
endif

if BUILD_QOGECOIN_TX
  bin_PROGRAMS += qogecoin-tx
endif

if ENABLE_WALLET
if BUILD_QOGECOIN_WALLET
  bin_PROGRAMS += qogecoin-wallet
endif
endif

if BUILD_QOGECOIN_UTIL
  bin_PROGRAMS += qogecoin-util
endif

if BUILD_QOGECOIN_CHAINSTATE
  bin_PROGRAMS += qogecoin-chainstate
endif

.PHONY: FORCE check-symbols check-security
# qogecoin core #
QOGECOIN_CORE_H = \
  addrdb.h \
  addrman.h \
  addrman_impl.h \
  attributes.h \
  banman.h \
  base58.h \
  bech32.h \
  blockencodings.h \
  blockfilter.h \
  chain.h \
  chainparams.h \
  chainparamsbase.h \
  chainparamsseeds.h \
  checkqueue.h \
  clientversion.h \
  coins.h \
  common/bloom.h \
  compat.h \
  compat/assumptions.h \
  compat/byteswap.h \
  compat/cpuid.h \
  compat/endian.h \
  compat/sanity.h \
  compressor.h \
  consensus/consensus.h \
  consensus/tx_check.h \
  consensus/tx_verify.h \
  core_io.h \
  core_memusage.h \
  cuckoocache.h \
  dbwrapper.h \
  deploymentinfo.h \
  deploymentstatus.h \
  external_signer.h \
  flatfile.h \
  fs.h \
  httprpc.h \
  httpserver.h \
  i2p.h \
  index/base.h \
  index/blockfilterindex.h \
  index/coinstatsindex.h \
  index/disktxpos.h \
  index/txindex.h \
  indirectmap.h \
  init.h \
  init/common.h \
  interfaces/chain.h \
  interfaces/echo.h \
  interfaces/handler.h \
  interfaces/init.h \
  interfaces/ipc.h \
  interfaces/node.h \
  interfaces/wallet.h \
  key.h \
  key_io.h \
  logging.h \
  logging/timer.h \
  mapport.h \
  memusage.h \
  merkleblock.h \
  net.h \
  net_permissions.h \
  net_processing.h \
  net_types.h \
  netaddress.h \
  netbase.h \
  netgroup.h \
  netmessagemaker.h \
  node/blockstorage.h \
  node/caches.h \
  node/chainstate.h \
  node/coin.h \
  node/coinstats.h \
  node/context.h \
  node/miner.h \
  node/minisketchwrapper.h \
  node/psbt.h \
  node/transaction.h \
  node/ui_interface.h \
  node/utxo_snapshot.h \
  noui.h \
  outputtype.h \
  policy/feerate.h \
  policy/fees.h \
  policy/packages.h \
  policy/policy.h \
  policy/rbf.h \
  policy/settings.h \
  pow.h \
  protocol.h \
  psbt.h \
  random.h \
  randomenv.h \
  rest.h \
  reverse_iterator.h \
  rpc/blockchain.h \
  rpc/client.h \
  rpc/mempool.h \
  rpc/mining.h \
  rpc/protocol.h \
  rpc/rawtransaction_util.h \
  rpc/register.h \
  rpc/request.h \
  rpc/server.h \
  rpc/server_util.h \
  rpc/util.h \
  scheduler.h \
  script/descriptor.h \
  script/keyorigin.h \
  script/miniscript.h \
  script/sigcache.h \
  script/sign.h \
  script/signingprovider.h \
  script/standard.h \
  shutdown.h \
  signet.h \
  streams.h \
  support/allocators/secure.h \
  support/allocators/zeroafterfree.h \
  support/cleanse.h \
  support/events.h \
  support/lockedpool.h \
  sync.h \
  threadinterrupt.h \
  threadsafety.h \
  timedata.h \
  torcontrol.h \
  txdb.h \
  txmempool.h \
  txorphanage.h \
  txrequest.h \
  undo.h \
  util/asmap.h \
  util/bip32.h \
  util/bytevectorhash.h \
  util/check.h \
  util/epochguard.h \
  util/error.h \
  util/fastrange.h \
  util/fees.h \
  util/getuniquepath.h \
  util/golombrice.h \
  util/hash_type.h \
  util/hasher.h \
  util/macros.h \
  util/message.h \
  util/moneystr.h \
  util/overflow.h \
  util/overloaded.h \
  util/rbf.h \
  util/readwritefile.h \
  util/serfloat.h \
  util/settings.h \
  util/sock.h \
  util/spanparsing.h \
  util/string.h \
  util/syscall_sandbox.h \
  util/system.h \
  util/thread.h \
  util/threadnames.h \
  util/time.h \
  util/tokenpipe.h \
  util/trace.h \
  util/translation.h \
  util/types.h \
  util/ui_change_type.h \
  util/url.h \
  util/vector.h \
  validation.h \
  validationinterface.h \
  versionbits.h \
  wallet/bdb.h \
  wallet/coincontrol.h \
  wallet/coinselection.h \
  wallet/context.h \
  wallet/crypter.h \
  wallet/db.h \
  wallet/dump.h \
  wallet/external_signer_scriptpubkeyman.h \
  wallet/feebumper.h \
  wallet/fees.h \
  wallet/ismine.h \
  wallet/load.h \
  wallet/receive.h \
  wallet/rpc/util.h \
  wallet/rpc/wallet.h \
  wallet/salvage.h \
  wallet/scriptpubkeyman.h \
  wallet/spend.h \
  wallet/sqlite.h \
  wallet/transaction.h \
  wallet/wallet.h \
  wallet/walletdb.h \
  wallet/wallettool.h \
  wallet/walletutil.h \
  walletinitinterface.h \
  warnings.h \
  zmq/zmqabstractnotifier.h \
  zmq/zmqnotificationinterface.h \
  zmq/zmqpublishnotifier.h \
  zmq/zmqrpc.h \
  zmq/zmqutil.h


obj/build.h: FORCE
	@$(MKDIR_P) $(builddir)/obj
	@$(top_srcdir)/share/genbuild.sh "$(abs_top_builddir)/src/obj/build.h" \
	  "$(abs_top_srcdir)"
libqogecoin_util_a-clientversion.$(OBJEXT): obj/build.h

ipc/capnp/libqogecoin_ipc_a-ipc.$(OBJEXT): $(libqogecoin_ipc_mpgen_input:=.h)

# server: shared between qogecoind and qogecoin-qt
# Contains code accessing mempool and chain state that is meant to be separated
# from wallet and gui code (see node/README.md). Shared code should go in
# libqogecoin_common or libqogecoin_util libraries, instead.
libqogecoin_node_a_CPPFLAGS = $(AM_CPPFLAGS) $(QOGECOIN_INCLUDES) $(MINIUPNPC_CPPFLAGS) $(NATPMP_CPPFLAGS) $(EVENT_CFLAGS) $(EVENT_PTHREADS_CFLAGS)
libqogecoin_node_a_CXXFLAGS = $(AM_CXXFLAGS) $(PIE_FLAGS)
libqogecoin_node_a_SOURCES = \
  addrdb.cpp \
  addrman.cpp \
  banman.cpp \
  blockencodings.cpp \
  blockfilter.cpp \
  chain.cpp \
  consensus/tx_verify.cpp \
  dbwrapper.cpp \
  deploymentstatus.cpp \
  flatfile.cpp \
  httprpc.cpp \
  httpserver.cpp \
  i2p.cpp \
  index/base.cpp \
  index/blockfilterindex.cpp \
  index/coinstatsindex.cpp \
  index/txindex.cpp \
  init.cpp \
  mapport.cpp \
  net.cpp \
  netgroup.cpp \
  net_processing.cpp \
  node/blockstorage.cpp \
  node/caches.cpp \
  node/chainstate.cpp \
  node/coin.cpp \
  node/coinstats.cpp \
  node/context.cpp \
  node/interfaces.cpp \
  node/miner.cpp \
  node/minisketchwrapper.cpp \
  node/psbt.cpp \
  node/transaction.cpp \
  node/ui_interface.cpp \
  noui.cpp \
  policy/fees.cpp \
  policy/packages.cpp \
  policy/rbf.cpp \
  policy/settings.cpp \
  pow.cpp \
  rest.cpp \
  rpc/blockchain.cpp \
  rpc/mempool.cpp \
  rpc/mining.cpp \
  rpc/misc.cpp \
  rpc/net.cpp \
  rpc/rawtransaction.cpp \
  rpc/server.cpp \
  rpc/server_util.cpp \
  rpc/txoutproof.cpp \
  script/sigcache.cpp \
  shutdown.cpp \
  signet.cpp \
  timedata.cpp \
  torcontrol.cpp \
  txdb.cpp \
  txmempool.cpp \
  txorphanage.cpp \
  txrequest.cpp \
  validation.cpp \
  validationinterface.cpp \
  versionbits.cpp \
  $(QOGECOIN_CORE_H)

if ENABLE_WALLET
libqogecoin_node_a_SOURCES += wallet/init.cpp
endif
if !ENABLE_WALLET
libqogecoin_node_a_SOURCES += dummywallet.cpp
endif

if ENABLE_ZMQ
libqogecoin_zmq_a_CPPFLAGS = $(AM_CPPFLAGS) $(QOGECOIN_INCLUDES) $(ZMQ_CFLAGS)
libqogecoin_zmq_a_CXXFLAGS = $(AM_CXXFLAGS) $(PIE_FLAGS)
libqogecoin_zmq_a_SOURCES = \
  zmq/zmqabstractnotifier.cpp \
  zmq/zmqnotificationinterface.cpp \
  zmq/zmqpublishnotifier.cpp \
  zmq/zmqrpc.cpp \
  zmq/zmqutil.cpp
endif


# wallet: shared between qogecoind and qogecoin-qt, but only linked
# when wallet enabled
libqogecoin_wallet_a_CPPFLAGS = $(AM_CPPFLAGS) $(QOGECOIN_INCLUDES) $(SQLITE_CFLAGS)
libqogecoin_wallet_a_CXXFLAGS = $(AM_CXXFLAGS) $(PIE_FLAGS)
libqogecoin_wallet_a_SOURCES = \
  wallet/coincontrol.cpp \
  wallet/context.cpp \
  wallet/crypter.cpp \
  wallet/db.cpp \
  wallet/dump.cpp \
  wallet/external_signer_scriptpubkeyman.cpp \
  wallet/feebumper.cpp \
  wallet/fees.cpp \
  wallet/interfaces.cpp \
  wallet/load.cpp \
  wallet/receive.cpp \
  wallet/rpc/addresses.cpp \
  wallet/rpc/backup.cpp \
  wallet/rpc/coins.cpp \
  wallet/rpc/encrypt.cpp \
  wallet/rpc/spend.cpp \
  wallet/rpc/signmessage.cpp \
  wallet/rpc/transactions.cpp \
  wallet/rpc/util.cpp \
  wallet/rpc/wallet.cpp \
  wallet/scriptpubkeyman.cpp \
  wallet/spend.cpp \
  wallet/transaction.cpp \
  wallet/wallet.cpp \
  wallet/walletdb.cpp \
  wallet/walletutil.cpp \
  wallet/coinselection.cpp \
  $(QOGECOIN_CORE_H)

if USE_SQLITE
libqogecoin_wallet_a_SOURCES += wallet/sqlite.cpp
endif
if USE_BDB
libqogecoin_wallet_a_SOURCES += wallet/bdb.cpp wallet/salvage.cpp
endif

libqogecoin_wallet_tool_a_CPPFLAGS = $(AM_CPPFLAGS) $(QOGECOIN_INCLUDES)
libqogecoin_wallet_tool_a_CXXFLAGS = $(AM_CXXFLAGS) $(PIE_FLAGS)
libqogecoin_wallet_tool_a_SOURCES = \
  wallet/wallettool.cpp \
  $(QOGECOIN_CORE_H)

# crypto primitives library
crypto_libqogecoin_crypto_base_la_CPPFLAGS = $(AM_CPPFLAGS)

# Specify -static in both CXXFLAGS and LDFLAGS so libtool will only build a
# static version of this library. We don't need a dynamic version, and a dynamic
# version can't be used on windows anyway because the library doesn't currently
# export DLL symbols.
crypto_libqogecoin_crypto_base_la_CXXFLAGS = $(AM_CXXFLAGS) $(PIE_FLAGS) -static
crypto_libqogecoin_crypto_base_la_LDFLAGS = $(AM_LDFLAGS) -static

crypto_libqogecoin_crypto_base_la_SOURCES = \
  crypto/aes.cpp \
  crypto/aes.h \
  crypto/chacha_poly_aead.h \
  crypto/chacha_poly_aead.cpp \
  crypto/chacha20.h \
  crypto/chacha20.cpp \
  crypto/common.h \
  crypto/hkdf_sha256_32.cpp \
  crypto/hkdf_sha256_32.h \
  crypto/hmac_sha256.cpp \
  crypto/hmac_sha256.h \
  crypto/hmac_sha512.cpp \
  crypto/hmac_sha512.h \
  crypto/poly1305.h \
  crypto/poly1305.cpp \
  crypto/muhash.h \
  crypto/muhash.cpp \
  crypto/ripemd160.cpp \
  crypto/ripemd160.h \
  crypto/sha1.cpp \
  crypto/sha1.h \
  crypto/sha256.cpp \
  crypto/sha256.h \
  crypto/sha3.cpp \
  crypto/sha3.h \
  crypto/sha512.cpp \
  crypto/sha512.h \
  crypto/siphash.cpp \
  crypto/siphash.h

if USE_ASM
crypto_libqogecoin_crypto_base_la_SOURCES += crypto/sha256_sse4.cpp
endif

# See explanation for -static in crypto_libqogecoin_crypto_base_la's LDFLAGS and
# CXXFLAGS above
crypto_libqogecoin_crypto_sse41_la_LDFLAGS = $(AM_LDFLAGS) -static
crypto_libqogecoin_crypto_sse41_la_CXXFLAGS = $(AM_CXXFLAGS) $(PIE_FLAGS) -static
crypto_libqogecoin_crypto_sse41_la_CPPFLAGS = $(AM_CPPFLAGS)
crypto_libqogecoin_crypto_sse41_la_CXXFLAGS += $(SSE41_CXXFLAGS)
crypto_libqogecoin_crypto_sse41_la_CPPFLAGS += -DENABLE_SSE41
crypto_libqogecoin_crypto_sse41_la_SOURCES = crypto/sha256_sse41.cpp

# See explanation for -static in crypto_libqogecoin_crypto_base_la's LDFLAGS and
# CXXFLAGS above
crypto_libqogecoin_crypto_avx2_la_LDFLAGS = $(AM_LDFLAGS) -static
crypto_libqogecoin_crypto_avx2_la_CXXFLAGS = $(AM_CXXFLAGS) $(PIE_FLAGS) -static
crypto_libqogecoin_crypto_avx2_la_CPPFLAGS = $(AM_CPPFLAGS)
crypto_libqogecoin_crypto_avx2_la_CXXFLAGS += $(AVX2_CXXFLAGS)
crypto_libqogecoin_crypto_avx2_la_CPPFLAGS += -DENABLE_AVX2
crypto_libqogecoin_crypto_avx2_la_SOURCES = crypto/sha256_avx2.cpp

# See explanation for -static in crypto_libqogecoin_crypto_base_la's LDFLAGS and
# CXXFLAGS above
crypto_libqogecoin_crypto_x86_shani_la_LDFLAGS = $(AM_LDFLAGS) -static
crypto_libqogecoin_crypto_x86_shani_la_CXXFLAGS = $(AM_CXXFLAGS) $(PIE_FLAGS) -static
crypto_libqogecoin_crypto_x86_shani_la_CPPFLAGS = $(AM_CPPFLAGS)
crypto_libqogecoin_crypto_x86_shani_la_CXXFLAGS += $(X86_SHANI_CXXFLAGS)
crypto_libqogecoin_crypto_x86_shani_la_CPPFLAGS += -DENABLE_X86_SHANI
crypto_libqogecoin_crypto_x86_shani_la_SOURCES = crypto/sha256_x86_shani.cpp

# See explanation for -static in crypto_libqogecoin_crypto_base_la's LDFLAGS and
# CXXFLAGS above
crypto_libqogecoin_crypto_arm_shani_la_LDFLAGS = $(AM_LDFLAGS) -static
crypto_libqogecoin_crypto_arm_shani_la_CXXFLAGS = $(AM_CXXFLAGS) $(PIE_FLAGS) -static
crypto_libqogecoin_crypto_arm_shani_la_CPPFLAGS = $(AM_CPPFLAGS)
crypto_libqogecoin_crypto_arm_shani_la_CXXFLAGS += $(ARM_SHANI_CXXFLAGS)
crypto_libqogecoin_crypto_arm_shani_la_CPPFLAGS += -DENABLE_ARM_SHANI
crypto_libqogecoin_crypto_arm_shani_la_SOURCES = crypto/sha256_arm_shani.cpp

# consensus: shared between all executables that validate any consensus rules.
libqogecoin_consensus_a_CPPFLAGS = $(AM_CPPFLAGS) $(QOGECOIN_INCLUDES) $(LIBOQS_CFLAGS)
libqogecoin_consensus_a_CXXFLAGS = $(AM_CXXFLAGS) $(PIE_FLAGS)
libqogecoin_consensus_a_SOURCES = \
  arith_uint256.cpp \
  arith_uint256.h \
  consensus/amount.h \
  consensus/merkle.cpp \
  consensus/merkle.h \
  consensus/params.h \
  consensus/tx_check.cpp \
  consensus/validation.h \
  hash/yescrypt/yescrypt.h \
  hash/yescrypt/yescrypt.c \
  hash/yescrypt/sha256.h \
  hash/yescrypt/sha256_c.h \
  hash/yescrypt/yescrypt-best_c.h \
  hash/yescrypt/sysendian.h \
  hash/yescrypt/yescrypt-platform_c.h \
  hash/yescrypt/yescrypt-opt_c.h \
  hash/yescrypt/yescrypt-simd_c.h \
  hash.cpp \
  hash.h \
  prevector.h \
  primitives/block.cpp \
  primitives/block.h \
  primitives/transaction.cpp \
  primitives/transaction.h \
  pubkey.cpp \
  pubkey.h \
  script/qogecoinconsensus.cpp \
  script/interpreter.cpp \
  script/interpreter.h \
  script/script.cpp \
  script/script.h \
  script/script_error.cpp \
  script/script_error.h \
  serialize.h \
  span.h \
  tinyformat.h \
  uint256.cpp \
  uint256.h \
  util/strencodings.cpp \
  util/strencodings.h \
  version.h

# common: shared between qogecoind, and qogecoin-qt and non-server tools
libqogecoin_common_a_CPPFLAGS = $(AM_CPPFLAGS) $(QOGECOIN_INCLUDES)
libqogecoin_common_a_CXXFLAGS = $(AM_CXXFLAGS) $(PIE_FLAGS)
libqogecoin_common_a_SOURCES = \
  base58.cpp \
  bech32.cpp \
  chainparams.cpp \
  coins.cpp \
  common/bloom.cpp \
  compressor.cpp \
  core_read.cpp \
  core_write.cpp \
  deploymentinfo.cpp \
  external_signer.cpp \
  init/common.cpp \
  key.cpp \
  key_io.cpp \
  merkleblock.cpp \
  net_types.cpp \
  netaddress.cpp \
  netbase.cpp \
  net_permissions.cpp \
  outputtype.cpp \
  policy/feerate.cpp \
  policy/policy.cpp \
  protocol.cpp \
  psbt.cpp \
  rpc/rawtransaction_util.cpp \
  rpc/external_signer.cpp \
  rpc/util.cpp \
  scheduler.cpp \
  script/descriptor.cpp \
  script/miniscript.cpp \
  script/sign.cpp \
  script/signingprovider.cpp \
  script/standard.cpp \
  warnings.cpp \
  $(QOGECOIN_CORE_H)

# util: shared between all executables.
# This library *must* be included to make sure that the glibc
# sanity checks are linked.
libqogecoin_util_a_CPPFLAGS = $(AM_CPPFLAGS) $(QOGECOIN_INCLUDES)
libqogecoin_util_a_CXXFLAGS = $(AM_CXXFLAGS) $(PIE_FLAGS)
libqogecoin_util_a_SOURCES = \
  support/lockedpool.cpp \
  chainparamsbase.cpp \
  clientversion.cpp \
  compat/glibcxx_sanity.cpp \
  fs.cpp \
  interfaces/echo.cpp \
  interfaces/handler.cpp \
  interfaces/init.cpp \
  logging.cpp \
  random.cpp \
  randomenv.cpp \
  rpc/request.cpp \
  support/cleanse.cpp \
  sync.cpp \
  threadinterrupt.cpp \
  util/asmap.cpp \
  util/bip32.cpp \
  util/bytevectorhash.cpp \
  util/check.cpp \
  util/error.cpp \
  util/fees.cpp \
  util/getuniquepath.cpp \
  util/hasher.cpp \
  util/sock.cpp \
  util/system.cpp \
  util/message.cpp \
  util/moneystr.cpp \
  util/rbf.cpp \
  util/readwritefile.cpp \
  util/settings.cpp \
  util/thread.cpp \
  util/threadnames.cpp \
  util/serfloat.cpp \
  util/spanparsing.cpp \
  util/strencodings.cpp \
  util/string.cpp \
  util/syscall_sandbox.cpp \
  util/time.cpp \
  util/tokenpipe.cpp \
  $(QOGECOIN_CORE_H)

if USE_LIBEVENT
libqogecoin_util_a_SOURCES += util/url.cpp
endif

# cli: shared between qogecoin-cli and qogecoin-qt
libqogecoin_cli_a_CPPFLAGS = $(AM_CPPFLAGS) $(QOGECOIN_INCLUDES)
libqogecoin_cli_a_CXXFLAGS = $(AM_CXXFLAGS) $(PIE_FLAGS)
libqogecoin_cli_a_SOURCES = \
  compat/stdin.h \
  compat/stdin.cpp \
  rpc/client.cpp \
  $(QOGECOIN_CORE_H)

nodist_libqogecoin_util_a_SOURCES = $(srcdir)/obj/build.h
#

# qogecoind & qogecoin-node binaries #
qogecoin_daemon_sources = qogecoind.cpp
qogecoin_bin_cppflags = $(AM_CPPFLAGS) $(QOGECOIN_INCLUDES)
qogecoin_bin_cxxflags = $(AM_CXXFLAGS) $(PIE_FLAGS)
qogecoin_bin_ldflags = $(RELDFLAGS) $(AM_LDFLAGS) $(LIBTOOL_APP_LDFLAGS) $(PTHREAD_FLAGS)

if TARGET_WINDOWS
qogecoin_daemon_sources += qogecoind-res.rc
endif

qogecoin_bin_ldadd = \
  $(LIBQOGECOIN_WALLET) \
  $(LIBQOGECOIN_COMMON) \
  $(LIBQOGECOIN_UTIL) \
  $(LIBUNIVALUE) \
  $(LIBQOGECOIN_ZMQ) \
  $(LIBQOGECOIN_CONSENSUS) \
  $(LIBQOGECOIN_CRYPTO) \
  $(LIBLEVELDB) \
  $(LIBMEMENV) \
  $(LIBSECP256K1)

qogecoin_bin_ldadd += $(BDB_LIBS) $(MINIUPNPC_LIBS) $(NATPMP_LIBS) $(EVENT_PTHREADS_LIBS) $(EVENT_LIBS) $(ZMQ_LIBS) $(SQLITE_LIBS) $(LIBOQS_LIBS)

qogecoind_SOURCES = $(qogecoin_daemon_sources) init/qogecoind.cpp
qogecoind_CPPFLAGS = $(qogecoin_bin_cppflags)
qogecoind_CXXFLAGS = $(qogecoin_bin_cxxflags)
qogecoind_LDFLAGS = $(qogecoin_bin_ldflags)
qogecoind_LDADD = $(LIBQOGECOIN_NODE) $(qogecoin_bin_ldadd)

qogecoin_node_SOURCES = $(qogecoin_daemon_sources) init/qogecoin-node.cpp
qogecoin_node_CPPFLAGS = $(qogecoin_bin_cppflags)
qogecoin_node_CXXFLAGS = $(qogecoin_bin_cxxflags)
qogecoin_node_LDFLAGS = $(qogecoin_bin_ldflags)
qogecoin_node_LDADD = $(LIBQOGECOIN_NODE) $(qogecoin_bin_ldadd) $(LIBQOGECOIN_IPC) $(LIBMULTIPROCESS_LIBS)

# qogecoin-cli binary #
qogecoin_cli_SOURCES = qogecoin-cli.cpp
qogecoin_cli_CPPFLAGS = $(AM_CPPFLAGS) $(QOGECOIN_INCLUDES) $(EVENT_CFLAGS)
qogecoin_cli_CXXFLAGS = $(AM_CXXFLAGS) $(PIE_FLAGS)
qogecoin_cli_LDFLAGS = $(RELDFLAGS) $(AM_LDFLAGS) $(LIBTOOL_APP_LDFLAGS) $(PTHREAD_FLAGS)

if TARGET_WINDOWS
qogecoin_cli_SOURCES += qogecoin-cli-res.rc
endif

qogecoin_cli_LDADD = \
  $(LIBQOGECOIN_CLI) \
  $(LIBUNIVALUE) \
  $(LIBQOGECOIN_UTIL) \
  $(LIBQOGECOIN_CRYPTO)

qogecoin_cli_LDADD += $(EVENT_LIBS)
#

# qogecoin-tx binary #
qogecoin_tx_SOURCES = qogecoin-tx.cpp
qogecoin_tx_CPPFLAGS = $(AM_CPPFLAGS) $(QOGECOIN_INCLUDES)
qogecoin_tx_CXXFLAGS = $(AM_CXXFLAGS) $(PIE_FLAGS)
qogecoin_tx_LDFLAGS = $(RELDFLAGS) $(AM_LDFLAGS) $(LIBTOOL_APP_LDFLAGS) $(PTHREAD_FLAGS)

if TARGET_WINDOWS
qogecoin_tx_SOURCES += qogecoin-tx-res.rc
endif

qogecoin_tx_LDADD = \
  $(LIBUNIVALUE) \
  $(LIBQOGECOIN_COMMON) \
  $(LIBQOGECOIN_UTIL) \
  $(LIBQOGECOIN_CONSENSUS) \
  $(LIBQOGECOIN_CRYPTO) \
  $(LIBSECP256K1) \
  $(LIBOQS_LIBS)
#

# qogecoin-wallet binary #
qogecoin_wallet_SOURCES = qogecoin-wallet.cpp
qogecoin_wallet_SOURCES += init/qogecoin-wallet.cpp
qogecoin_wallet_CPPFLAGS = $(qogecoin_bin_cppflags)
qogecoin_wallet_CXXFLAGS = $(qogecoin_bin_cxxflags)
qogecoin_wallet_LDFLAGS = $(qogecoin_bin_ldflags)
qogecoin_wallet_LDADD = \
  $(LIBQOGECOIN_WALLET_TOOL) \
  $(LIBQOGECOIN_WALLET) \
  $(LIBQOGECOIN_COMMON) \
  $(LIBQOGECOIN_UTIL) \
  $(LIBUNIVALUE) \
  $(LIBQOGECOIN_CONSENSUS) \
  $(LIBQOGECOIN_CRYPTO) \
  $(LIBSECP256K1) \
  $(BDB_LIBS) \
  $(SQLITE_LIBS) \
  $(LIBOQS_LIBS)

if TARGET_WINDOWS
qogecoin_wallet_SOURCES += qogecoin-wallet-res.rc
endif
#

# qogecoin-util binary #
qogecoin_util_SOURCES = qogecoin-util.cpp
qogecoin_util_CPPFLAGS = $(AM_CPPFLAGS) $(QOGECOIN_INCLUDES)
qogecoin_util_CXXFLAGS = $(AM_CXXFLAGS) $(PIE_FLAGS)
qogecoin_util_LDFLAGS = $(RELDFLAGS) $(AM_LDFLAGS) $(LIBTOOL_APP_LDFLAGS) $(PTHREAD_FLAGS)

if TARGET_WINDOWS
qogecoin_util_SOURCES += qogecoin-util-res.rc
endif

qogecoin_util_LDADD = \
  $(LIBQOGECOIN_COMMON) \
  $(LIBQOGECOIN_UTIL) \
  $(LIBUNIVALUE) \
  $(LIBQOGECOIN_CONSENSUS) \
  $(LIBQOGECOIN_CRYPTO) \
  $(LIBSECP256K1) \
  $(LIBOQS_LIBS)
#

# qogecoin-chainstate binary #
qogecoin_chainstate_SOURCES = qogecoin-chainstate.cpp
qogecoin_chainstate_CPPFLAGS = $(AM_CPPFLAGS) $(QOGECOIN_INCLUDES)
qogecoin_chainstate_CXXFLAGS = $(AM_CXXFLAGS) $(PIE_FLAGS)

# $(LIBTOOL_APP_LDFLAGS) deliberately omitted here so that we can test linking
# qogecoin-chainstate against libqogecoinkernel as a shared or static library by
# setting --{en,dis}able-shared.
qogecoin_chainstate_LDFLAGS = $(RELDFLAGS) $(AM_LDFLAGS) $(PTHREAD_FLAGS)
qogecoin_chainstate_LDADD = $(LIBQOGECOINKERNEL)
#

# qogecoinkernel library #
if BUILD_QOGECOIN_KERNEL_LIB
lib_LTLIBRARIES += $(LIBQOGECOINKERNEL)

libqogecoinkernel_la_LDFLAGS = $(AM_LDFLAGS) -no-undefined $(RELDFLAGS) $(PTHREAD_FLAGS)
libqogecoinkernel_la_LIBADD = $(LIBQOGECOIN_CRYPTO) $(LIBUNIVALUE) $(LIBLEVELDB) $(LIBMEMENV) $(LIBSECP256K1)
libqogecoinkernel_la_CPPFLAGS = $(AM_CPPFLAGS) -I$(builddir)/obj -I$(srcdir)/secp256k1/include -DBUILD_QOGECOIN_INTERNAL $(BOOST_CPPFLAGS) $(LEVELDB_CPPFLAGS) -I$(srcdir)/$(UNIVALUE_INCLUDE_DIR_INT)

# libqogecoinkernel requires default symbol visibility, explicitly specify that
# here so that things still work even when user configures with
#   --enable-reduce-exports
#
# Note this is a quick hack that will be removed as we incrementally define what
# to export from the library.
libqogecoinkernel_la_CXXFLAGS = $(AM_CXXFLAGS) $(PIE_FLAGS) -fvisibility=default

# TODO: For now, Specify -static in both CXXFLAGS and LDFLAGS when building for
#       windows targets so libtool will only build a static version of this
#       library. There are unresolved problems when building dll's for mingw-w64
#       and attempting to statically embed libstdc++, libpthread, etc.
if TARGET_WINDOWS
libqogecoinkernel_la_LDFLAGS += -static
libqogecoinkernel_la_CXXFLAGS += -static
endif

# TODO: libqogecoinkernel is a work in progress consensus engine library, as more
#       and more modules are decoupled from the consensus engine, this list will
#       shrink to only those which are absolutely necessary. For example, things
#       like index/*.cpp will be removed.
libqogecoinkernel_la_SOURCES = \
  kernel/qogecoinkernel.cpp \
  arith_uint256.cpp \
  blockfilter.cpp \
  chain.cpp \
  chainparamsbase.cpp \
  chainparams.cpp \
  clientversion.cpp \
  coins.cpp \
  compat/glibcxx_sanity.cpp \
  compressor.cpp \
  consensus/merkle.cpp \
  consensus/tx_check.cpp \
  consensus/tx_verify.cpp \
  core_read.cpp \
  dbwrapper.cpp \
  deploymentinfo.cpp \
  deploymentstatus.cpp \
  flatfile.cpp \
  fs.cpp \
  hash.cpp \
  index/base.cpp \
  index/blockfilterindex.cpp \
  index/coinstatsindex.cpp \
  init/common.cpp \
  key.cpp \
  logging.cpp \
  netaddress.cpp \
  node/blockstorage.cpp \
  node/chainstate.cpp \
  node/coinstats.cpp \
  node/ui_interface.cpp \
  policy/feerate.cpp \
  policy/fees.cpp \
  policy/packages.cpp \
  policy/policy.cpp \
  policy/rbf.cpp \
  policy/settings.cpp \
  pow.cpp \
  primitives/block.cpp \
  primitives/transaction.cpp \
  pubkey.cpp \
  random.cpp \
  randomenv.cpp \
  scheduler.cpp \
  script/interpreter.cpp \
  script/script.cpp \
  script/script_error.cpp \
  script/sigcache.cpp \
  script/standard.cpp \
  shutdown.cpp \
  signet.cpp \
  support/cleanse.cpp \
  support/lockedpool.cpp \
  sync.cpp \
  threadinterrupt.cpp \
  timedata.cpp \
  txdb.cpp \
  txmempool.cpp \
  uint256.cpp \
  util/asmap.cpp \
  util/bytevectorhash.cpp \
  util/check.cpp \
  util/getuniquepath.cpp \
  util/hasher.cpp \
  util/moneystr.cpp \
  util/rbf.cpp \
  util/serfloat.cpp \
  util/settings.cpp \
  util/strencodings.cpp \
  util/syscall_sandbox.cpp \
  util/system.cpp \
  util/thread.cpp \
  util/threadnames.cpp \
  util/time.cpp \
  util/tokenpipe.cpp \
  validation.cpp \
  validationinterface.cpp \
  versionbits.cpp \
  warnings.cpp

# Required for obj/build.h to be generated first.
# More details: https://www.gnu.org/software/automake/manual/html_node/Built-Sources-Example.html
libqogecoinkernel_la-clientversion.l$(OBJEXT): obj/build.h
endif # BUILD_QOGECOIN_KERNEL_LIB
#

# qogecoinconsensus library #
if BUILD_QOGECOIN_LIBS
lib_LTLIBRARIES += $(LIBQOGECOINCONSENSUS)

include_HEADERS = script/qogecoinconsensus.h
libqogecoinconsensus_la_SOURCES = support/cleanse.cpp $(crypto_libqogecoin_crypto_base_la_SOURCES) $(libqogecoin_consensus_a_SOURCES)

libqogecoinconsensus_la_LDFLAGS = $(AM_LDFLAGS) -no-undefined $(RELDFLAGS)
libqogecoinconsensus_la_LIBADD = $(LIBSECP256K1)
libqogecoinconsensus_la_CPPFLAGS = $(AM_CPPFLAGS) -I$(builddir)/obj -I$(srcdir)/secp256k1/include -DBUILD_QOGECOIN_INTERNAL
libqogecoinconsensus_la_CXXFLAGS = $(AM_CXXFLAGS) $(PIE_FLAGS)

endif
#

CTAES_DIST =  crypto/ctaes/bench.c
CTAES_DIST += crypto/ctaes/ctaes.c
CTAES_DIST += crypto/ctaes/ctaes.h
CTAES_DIST += crypto/ctaes/README.md
CTAES_DIST += crypto/ctaes/test.c

CLEANFILES = $(EXTRA_LIBRARIES)

CLEANFILES += *.gcda *.gcno
CLEANFILES += compat/*.gcda compat/*.gcno
CLEANFILES += consensus/*.gcda consensus/*.gcno
CLEANFILES += crc32c/src/*.gcda crc32c/src/*.gcno
CLEANFILES += crypto/*.gcda crypto/*.gcno
CLEANFILES += index/*.gcda index/*.gcno
CLEANFILES += interfaces/*.gcda interfaces/*.gcno
CLEANFILES += node/*.gcda node/*.gcno
CLEANFILES += policy/*.gcda policy/*.gcno
CLEANFILES += primitives/*.gcda primitives/*.gcno
CLEANFILES += rpc/*.gcda rpc/*.gcno
CLEANFILES += script/*.gcda script/*.gcno
CLEANFILES += support/*.gcda support/*.gcno
CLEANFILES += univalue/*.gcda univalue/*.gcno
CLEANFILES += util/*.gcda util/*.gcno
CLEANFILES += wallet/*.gcda wallet/*.gcno
CLEANFILES += wallet/test/*.gcda wallet/test/*.gcno
CLEANFILES += zmq/*.gcda zmq/*.gcno
CLEANFILES += obj/build.h

EXTRA_DIST = $(CTAES_DIST)


config/qogecoin-config.h: config/stamp-h1
	@$(MAKE) -C $(top_builddir) $(subdir)/$(@)
config/stamp-h1: $(top_srcdir)/$(subdir)/config/qogecoin-config.h.in $(top_builddir)/config.status
	$(AM_V_at)$(MAKE) -C $(top_builddir) $(subdir)/$(@)
$(top_srcdir)/$(subdir)/config/qogecoin-config.h.in:  $(am__configure_deps)
	$(AM_V_at)$(MAKE) -C $(top_srcdir) $(subdir)/config/qogecoin-config.h.in

clean-local:
	-$(MAKE) -C secp256k1 clean
	-rm -f leveldb/*/*.gcda leveldb/*/*.gcno leveldb/helpers/memenv/*.gcda leveldb/helpers/memenv/*.gcno
	-rm -f config.h
	-rm -rf test/__pycache__

.rc.o:
	@test -f $(WINDRES)
	## FIXME: How to get the appropriate modulename_CPPFLAGS in here?
	$(AM_V_GEN) $(WINDRES) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(CPPFLAGS) -DWINDRES_PREPROC -i $< -o $@

check-symbols: $(bin_PROGRAMS)
	@echo "Running symbol and dynamic library checks..."
	$(AM_V_at) $(PYTHON) $(top_srcdir)/contrib/devtools/symbol-check.py $(bin_PROGRAMS)

check-security: $(bin_PROGRAMS)
if HARDEN
	@echo "Checking binary security..."
	$(AM_V_at) $(PYTHON) $(top_srcdir)/contrib/devtools/security-check.py $(bin_PROGRAMS)
endif

libqogecoin_ipc_mpgen_input = \
  ipc/capnp/echo.capnp \
  ipc/capnp/init.capnp
EXTRA_DIST += $(libqogecoin_ipc_mpgen_input)
%.capnp:

if BUILD_MULTIPROCESS
LIBQOGECOIN_IPC=libqogecoin_ipc.a
libqogecoin_ipc_a_SOURCES = \
  ipc/capnp/context.h \
  ipc/capnp/init-types.h \
  ipc/capnp/protocol.cpp \
  ipc/capnp/protocol.h \
  ipc/context.h \
  ipc/exception.h \
  ipc/interfaces.cpp \
  ipc/process.cpp \
  ipc/process.h \
  ipc/protocol.h
libqogecoin_ipc_a_CPPFLAGS = $(AM_CPPFLAGS) $(QOGECOIN_INCLUDES)
libqogecoin_ipc_a_CXXFLAGS = $(AM_CXXFLAGS) $(PIE_FLAGS) $(LIBMULTIPROCESS_CFLAGS)

include $(MPGEN_PREFIX)/include/mpgen.mk
libqogecoin_ipc_mpgen_output = \
  $(libqogecoin_ipc_mpgen_input:=.c++) \
  $(libqogecoin_ipc_mpgen_input:=.h) \
  $(libqogecoin_ipc_mpgen_input:=.proxy-client.c++) \
  $(libqogecoin_ipc_mpgen_input:=.proxy-server.c++) \
  $(libqogecoin_ipc_mpgen_input:=.proxy-types.c++) \
  $(libqogecoin_ipc_mpgen_input:=.proxy-types.h) \
  $(libqogecoin_ipc_mpgen_input:=.proxy.h)
nodist_libqogecoin_ipc_a_SOURCES = $(libqogecoin_ipc_mpgen_output)
CLEANFILES += $(libqogecoin_ipc_mpgen_output)
endif

include Makefile.minisketch.include

include Makefile.crc32c.include
include Makefile.leveldb.include

include Makefile.test_util.include
include Makefile.test_fuzz.include

if ENABLE_TESTS
include Makefile.test.include
endif

if ENABLE_BENCH
include Makefile.bench.include
endif

if ENABLE_QT
include Makefile.qt.include
endif

if ENABLE_QT_TESTS
include Makefile.qttest.include
endif

include Makefile.univalue.include

if TARGET_DARWIN
CFLAGS += -O3 -msse4.1 -funroll-loops -fomit-frame-pointer
else
CFLAGS += -msse4.1 -fPIC
endif

if TARGET_DARWIN
CFLAGS += -O3 -msse4.1 -funroll-loops -fomit-frame-pointer
else
CFLAGS += -msse4.1 -fPIC
endif

if TARGET_DARWIN
CFLAGS += -O3 -msse4.1 -funroll-loops -fomit-frame-pointer
else
CFLAGS += -msse4.1 -fPIC
endif
