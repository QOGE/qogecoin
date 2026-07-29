# Building QOGE/qogecoin from a Fresh Ubuntu 24.04 Install

Both build fixes referenced in earlier versions of this document are now
merged into the real repo (commit `dbea00cc7`, `qoge-fork stable`/`main`),
independently reviewed by both Codex and Grok Build with zero findings on
either. A plain `git clone` now brings in the correct, working state
automatically — no manual file transfer, no local patching, no
workaround steps of any kind.

## Step 1 — Base dependencies

```bash
sudo apt-get update
sudo apt-get install -y build-essential cmake ninja-build git golang-go \
  libssl-dev pkg-config libboost-all-dev libevent-dev libsqlite3-dev \
  autoconf automake libtool bsdmainutils python3
```

## Step 2 — Clone and checkout

```bash
cd ~
git clone https://github.com/QOGE/qogecoin.git
cd qogecoin
git checkout stable
```

## Step 3 — Generate configure

```bash
./autogen.sh
```

`configure` is a generated file, not tracked in the repo — this step is
required before anything else will work.

## Step 4 — Build the depends tree

```bash
cd ~/qogecoin
make -C depends HOST=$(gcc -dumpmachine) \
  NO_QT=1 NO_BDB=1 NO_ZMQ=1 NO_UPNP=1 NO_NATPMP=1 NO_USDT=1 \
  ALLOW_HOST_PACKAGES=1 install
```

**Why the full `install` target, not just individual packages:** the
file `depends/x86_64-linux-gnu/share/config.site` — required by
`configure` in Step 5 — is only ever generated as a dependency of the
top-level `install` target, never of any individual package target.

**Why the `NO_*` flags:** the plain `install` target's default package
set also builds Qt, BerkeleyDB, ZeroMQ, miniupnpc, libnatpmp, and
systemtap support from source — none of which this project needs. Every
`configure` invocation here has always used `--without-gui --without-zmq
--without-miniupnpc --without-natpmp`, and BDB/USDT have never been
present or needed. Qt alone can take 15–40+ minutes to build from
source; these flags skip all of it.

This step should complete cleanly on the first try — Boost now fetches
correctly from `archives.boost.io`, with an automatic fallback to a
QOGE-hosted mirror if that's ever unreachable.

**Verify:**

```bash
ls -la ~/qogecoin/depends/x86_64-linux-gnu/
```

Should show both `lib/` and `share/`.

## Step 5 — Configure

```bash
cd ~/qogecoin
CONFIG_SITE=$PWD/depends/x86_64-linux-gnu/share/config.site \
./configure --without-miniupnpc --without-natpmp --enable-tests \
--disable-bench --without-gui --without-zmq
```

**Should show `"liboqs: Option A — static lib at
.../depends/x86_64-linux-gnu/lib/liboqs.a"`.** If it shows Option B
instead, confirm you're actually on a commit that includes `dbea00cc7`
or later (`git log -1 --oneline`) — this was a known, now-fixed
detection bug in earlier commits on `stable`.

## Step 6 — Build

```bash
make -j$(nproc) src/qogecoind src/qogecoin-cli
```

## Verify the result

```bash
ls -la ~/qogecoin/src/qogecoind ~/qogecoin/src/qogecoin-cli
~/qogecoin/src/qogecoin-cli --version
```

## Troubleshooting: `share/` missing after Step 4

If it happens anyway — the depends system may be trusting a stale
cached tarball for some package. Check:

```bash
tar -tzf ~/qogecoin/depends/built/x86_64-linux-gnu/<package>/<package>-*.tar.* | grep -c "^"
```

A healthy tarball has hundreds of entries. A suspiciously low count
(e.g. 40) with no `share/` entries means it was cached before finishing
correctly — remove it and rebuild:

```bash
rm ~/qogecoin/depends/built/x86_64-linux-gnu/<package>/<package>-*.tar.*
make -C depends HOST=$(gcc -dumpmachine) NO_QT=1 NO_BDB=1 NO_ZMQ=1 \
  NO_UPNP=1 NO_NATPMP=1 NO_USDT=1 ALLOW_HOST_PACKAGES=1 install
```

**Note, worth remembering if a stray `rm -f *packagename*` ever seems to
silently do nothing:** bash's default glob doesn't match dotfiles.
Depends system stamp files often start with a leading `.` (e.g.
`.stamp_fetched-boost-...`) — use the exact filename, or `find ... -iname
"*name*"` first to confirm what's actually there before trying to
remove it.
