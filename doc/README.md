Qogecoin Core
=============

Setup
---------------------
Qogecoin Core is the original Qogecoin client and it builds the backbone of the network. It downloads and, by default, stores the entire history of Qogecoin transactions, which requires a few hundred gigabytes of disk space. Depending on the speed of your computer and network connection, the synchronization process can take anywhere from a few hours to a day or more.

To download Qogecoin Core, visit [qogecoincore.org](https://qogecoincore.org/en/download/).

Running
---------------------
The following are some helpful notes on how to run Qogecoin Core on your native platform.

### Unix

Unpack the files into a directory and run:

- `bin/qogecoin-qt` (GUI) or
- `bin/qogecoind` (headless)

### Windows

Unpack the files into a directory, and then run qogecoin-qt.exe.

### macOS

Drag Qogecoin Core to your applications folder, and then run Qogecoin Core.

### Need Help?

* See the documentation at the [Qogecoin Wiki](https://en.qogecoin.it/wiki/Main_Page)
for help and more information.
* Ask for help on [Qogecoin StackExchange](https://qogecoin.stackexchange.com).
* Ask for help on #qogecoin on Libera Chat. If you don't have an IRC client, you can use [web.libera.chat](https://web.libera.chat/#qogecoin).
* Ask for help on the [QogecoinTalk](https://qogecointalk.org/) forums, in the [Technical Support board](https://qogecointalk.org/index.php?board=4.0).

Building
---------------------
The following are developer notes on how to build Qogecoin Core on your native platform. They are not complete guides, but include notes on the necessary libraries, compile flags, etc.

- [Dependencies](dependencies.md)
- [Ubuntu 24.04 Build Tutorial](build-ubuntu-2404.md) — **current, tested build path for this fork** (validated end-to-end with liboqs Option A static build)
- [macOS Build Notes](build-osx-DEPRECATED.md) *(deprecated — untested for this fork)*
- [Unix Build Notes](build-unix-DEPRECATED.md) *(deprecated — untested for this fork)*
- [Windows Build Notes](build-windows-DEPRECATED.md) *(deprecated — untested for this fork)*
- [FreeBSD Build Notes](build-freebsd-DEPRECATED.md) *(deprecated — untested for this fork)*
- [OpenBSD Build Notes](build-openbsd-DEPRECATED.md) *(deprecated — untested for this fork)*
- [NetBSD Build Notes](build-netbsd-DEPRECATED.md) *(deprecated — untested for this fork)*
- [Android Build Notes](build-android-DEPRECATED.md) *(deprecated — untested for this fork)*

Development
---------------------
The Qogecoin repo's [root README](/README.md) contains relevant information on the development process and automated testing.

- [Developer Notes](developer-notes.md)
- [Productivity Notes](productivity.md)
- [Release Notes](release-notes.md)
- [Release Process](release-process.md)
- [Source Code Documentation (External Link)](https://doxygen.qogecoincore.org/)
- [Translation Process](translation_process.md)
- [Translation Strings Policy](translation_strings_policy.md)
- [JSON-RPC Interface](JSON-RPC-interface.md)
- [Unauthenticated REST Interface](REST-interface.md)
- [Shared Libraries](shared-libraries.md)
- [BIPS](bips.md)
- [Dnsseed Policy](dnsseed-policy.md)
- [Benchmarking](benchmarking.md)

### Resources
* Discuss on the [QogecoinTalk](https://qogecointalk.org/) forums, in the [Development & Technical Discussion board](https://qogecointalk.org/index.php?board=6.0).
* Discuss project-specific development on #qogecoin-core-dev on Libera Chat. If you don't have an IRC client, you can use [web.libera.chat](https://web.libera.chat/#qogecoin-core-dev).

### Miscellaneous
- [Assets Attribution](assets-attribution.md)
- [Assumeutxo design](assumeutxo.md)
- [qogecoin.conf Configuration File](qogecoin-conf.md)
- [CJDNS Support](cjdns.md)
- [Files](files.md)
- [Fuzz-testing](fuzzing.md)
- [I2P Support](i2p.md)
- [Init Scripts (systemd/upstart/openrc)](init.md)
- [Managing Wallets](managing-wallets.md)
- [Multisig Tutorial](multisig-tutorial.md)
- [P2P bad ports definition and list](p2p-bad-ports.md)
- [PSBT support](psbt.md)
- [Reduce Memory](reduce-memory.md)
- [Reduce Traffic](reduce-traffic.md)
- [Tor Support](tor.md)
- [Transaction Relay Policy](policy/README.md)
- [ZMQ](zmq.md)

License
---------------------
Distributed under the [MIT software license](/COPYING).
