Rupaya Core integration/staging repository
==========================================

[![master Actions Status](https://github.com/safuapy/rupaya/workflows/CI%20Actions%20for%20Rupaya/badge.svg)](https://github.com/safuapy/rupaya/actions)
[![GitHub release (latest by date)](https://img.shields.io/github/v/release/safuapy/rupaya?color=%235c4b7d&cacheSeconds=3600)](https://github.com/safuapy/rupaya/releases)

## What is Rupaya?

Rupaya (RUPX) is an open source Proof-of-Stake cryptocurrency focused on:

1. **Privacy** — SHIELD (zk-SNARKs based privacy protocol)
2. **Energy Efficiency** — Proof-of-Stake consensus with masternode tier-two network
3. **Decentralized Governance** — DAO with monthly treasury, proposals, and voting
4. **Fast Transactions** — 60-second block times with ChainLocks
5. **Ease of Use** — Full Qt GUI wallet for desktop platforms

## Tokenomics

| Parameter | Value |
|---|---|
| Ticker | RUPX |
| Total Supply | 100,000,000 RUPX |
| Block 1 (Premine) | 100,000,000 RUPX |
| Blocks 2-100 | 0 RUPX (PoW bootstrap) |
| Block 101+ (PoS) | 10 RUPX/block (~5.2% APY) |
| Masternode Reward | 4 RUPX/block (40%) |
| Staker Reward | 6 RUPX/block (60%) |
| Block Time | 60 seconds |
| MN Collateral | 10,000 RUPX |

## Chain Parameters

| Parameter | Mainnet | Testnet | Regtest |
|---|---|---|---|
| P2P Port | 8765 | 8766 | 8767 |
| RPC Port | 8768 | 8769 | 8770 |
| Magic Bytes | 0x52555041 | 0x72757054 | 0x72757052 |
| Address Prefix | R (60) | t/n (111) | t/n (111) |
| Bech32 HRP | rs | rstest | rstest |
| BIP44 Coin Type | 499 | 1 | 1 |
| Data Directory | ~/.rupx | ~/.rupx/testnet | ~/.rupx/regtest |

## Downloads

CI build artifacts are available for each platform:

- **Linux** (x86_64): `rupxd`, `rupx-cli`, `rupx-qt` — [CI Artifacts](https://github.com/safuapy/rupaya/actions)
- **macOS**: `Rupaya-*.dmg` — [CI Artifacts](https://github.com/safuapy/rupaya/actions)
- **Windows**: `Rupaya-*-win64-setup.exe` — [CI Artifacts](https://github.com/safuapy/rupaya/actions)

> **Note:** macOS .dmg and Windows .exe are currently unsigned. For production distribution, code signing certificates (Apple Developer ID + Windows EV cert) must be configured as GitHub Actions secrets.

## Build from Source

See `doc/build-unix.md`, `doc/build-osx.md`, and `doc/build-windows.md` for platform-specific build instructions.

Quick start (Linux/macOS):

```bash
./autogen.sh
./configure
make -j$(nproc)
```

## License

Rupaya Core is released under the terms of the MIT license. See [COPYING](COPYING) for more information.

## Development

The contribution workflow is described in [CONTRIBUTING.md](CONTRIBUTING.md) and useful hints for developers can be found in [doc/developer-notes.md](doc/developer-notes.md).

## Testing

Developers are strongly encouraged to write unit tests for new code. Unit tests can be compiled and run with:

```bash
make check
```

Functional/regression tests:

```bash
test/functional/test_runner.py
```

The CI systems make sure that every pull request is built for Windows, Linux, and macOS, and that unit/sanity tests are run automatically.

## Community

- Website: [https://www.rupaya.io](https://www.rupaya.io)
- GitHub: [https://github.com/safuapy/rupaya](https://github.com/safuapy/rupaya)
