// Copyright (c) 2021 The PIVX Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or https://www.opensource.org/licenses/mit-license.php.

#ifndef RUPX_WALLET_TEST_POS_TEST_FIXTURE_H
#define RUPX_WALLET_TEST_POS_TEST_FIXTURE_H

#include "test/test_rupx.h"

class CWallet;

/*
 * A text fixture with a preloaded 250-blocks regtest chain running running on PoS
 * and a wallet containing the key used for the coinbase outputs.
 */
struct TestPoSChainSetup: public TestChainSetup
{
    std::unique_ptr<CWallet> pwalletMain;

    TestPoSChainSetup();
    ~TestPoSChainSetup();
};

#endif // RUPX_WALLET_TEST_POS_TEST_FIXTURE_H
