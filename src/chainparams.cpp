// Copyright (c) 2010 Satoshi Nakamoto
// Copyright (c) 2009-2015 The Bitcoin developers
// Copyright (c) 2014-2015 The Dash developers
// Copyright (c) 2015-2022 The PIVX Core developers
// Copyright (c) 2024 The Rupaya Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "chainparams.h"

#include "chainparamsseeds.h"
#include "consensus/merkle.h"
#include "tinyformat.h"
#include "utilstrencodings.h"

#include <assert.h>

static CBlock CreateGenesisBlock(const char* pszTimestamp, const CScript& genesisOutputScript, uint32_t nTime, uint32_t nNonce, uint32_t nBits, int32_t nVersion, const CAmount& genesisReward)
{
    CMutableTransaction txNew;
    txNew.nVersion = 1;
    txNew.vin.resize(1);
    txNew.vout.resize(1);
    txNew.vin[0].scriptSig = CScript() << 486604799 << CScriptNum(4) << std::vector<unsigned char>((const unsigned char*)pszTimestamp, (const unsigned char*)pszTimestamp + strlen(pszTimestamp));
    txNew.vout[0].nValue = genesisReward;
    txNew.vout[0].scriptPubKey = genesisOutputScript;

    CBlock genesis;
    genesis.vtx.push_back(std::make_shared<const CTransaction>(std::move(txNew)));
    genesis.hashPrevBlock.SetNull();
    genesis.nVersion = nVersion;
    genesis.nTime    = nTime;
    genesis.nBits    = nBits;
    genesis.nNonce   = nNonce;
    genesis.hashMerkleRoot = BlockMerkleRoot(genesis);
    return genesis;
}

void CChainParams::UpdateNetworkUpgradeParameters(Consensus::UpgradeIndex idx, int nActivationHeight)
{
    assert(IsRegTestNet()); // only available for regtest
    assert(idx > Consensus::BASE_NETWORK && idx < Consensus::MAX_NETWORK_UPGRADES);
    consensus.vUpgrades[idx].nActivationHeight = nActivationHeight;
}

/**
 * Build the genesis block. Note that the output of the genesis coinbase cannot
 * be spent as it did not originally exist in the database.
 *
 * CBlock(hash=00000ffd590b14, ver=1, hashPrevBlock=00000000000000, hashMerkleRoot=e0028e, nTime=1390095618, nBits=1e0ffff0, nNonce=28917698, vtx=1)
 *   CTransaction(hash=e0028e, ver=1, vin.size=1, vout.size=1, nLockTime=0)
 *     CTxIn(COutPoint(000000, -1), coinbase 04ffff001d01044c5957697265642030392f4a616e2f3230313420546865204772616e64204578706572696d656e7420476f6573204c6976653a204f76657273746f636b2e636f6d204973204e6f7720416363657074696e6720426974636f696e73)
 *     CTxOut(nValue=50.00000000, scriptPubKey=0xA9037BAC7050C479B121CF)
 *   vMerkleTree: e0028e
 */
static CBlock CreateGenesisBlock(uint32_t nTime, uint32_t nNonce, uint32_t nBits, int32_t nVersion, const CAmount& genesisReward)
{
    const char* pszTimestamp = "Rupaya Genesis Block - RUPX - Proof of Stake Privacy Coin";
    const CScript genesisOutputScript = CScript() << ParseHex("04c10e83b2703ccf322f7dbd62dd5855ac7c10bd055814ce121ba32607d573b8810c02c0582aed05b4deb9c4b77b26d92428c61256cd42774babea0a073b2ed0c9") << OP_CHECKSIG;
    return CreateGenesisBlock(pszTimestamp, genesisOutputScript, nTime, nNonce, nBits, nVersion, genesisReward);
}

// this one is for testing only
static Consensus::LLMQParams llmq_test = {
        .type = Consensus::LLMQ_TEST,
        .name = "llmq_test",
        .size = 3,
        .minSize = 2,
        .threshold = 2,

        .dkgInterval = 20, // one every 20 minutes
        .dkgPhaseBlocks = 2,
        .dkgMiningWindowStart = 10, // dkgPhaseBlocks * 5 = after finalization
        .dkgMiningWindowEnd = 15,
        .dkgBadVotesThreshold = 2,

        .signingActiveQuorumCount = 2, // just a few ones to allow easier testing

        .keepOldConnections = 3,
        .recoveryMembers = 3,

        .cacheDkgInterval = 60,
};

static Consensus::LLMQParams llmq50_60 = {
        .type = Consensus::LLMQ_50_60,
        .name = "llmq_50_60",
        .size = 50,
        .minSize = 40,
        .threshold = 30,

        .dkgInterval = 60, // one DKG per hour
        .dkgPhaseBlocks = 6,
        .dkgMiningWindowStart = 30, // dkgPhaseBlocks * 5 = after finalization
        .dkgMiningWindowEnd = 40,
        .dkgBadVotesThreshold = 40,

        .signingActiveQuorumCount = 24, // a full day worth of LLMQs

        .keepOldConnections = 25,
        .recoveryMembers = 25,

        .cacheDkgInterval = 600,
};

static Consensus::LLMQParams llmq400_60 = {
        .type = Consensus::LLMQ_400_60,
        .name = "llmq_400_60",
        .size = 400,
        .minSize = 300,
        .threshold = 240,

        .dkgInterval = 60 * 12, // one DKG every 12 hours
        .dkgPhaseBlocks = 10,
        .dkgMiningWindowStart = 50, // dkgPhaseBlocks * 5 = after finalization
        .dkgMiningWindowEnd = 70,
        .dkgBadVotesThreshold = 300,

        .signingActiveQuorumCount = 4, // two days worth of LLMQs

        .keepOldConnections = 5,
        .recoveryMembers = 100,

        .cacheDkgInterval = 60 * 12 * 10, // dkgInterval * 10
};

// Used for deployment and min-proto-version signaling, so it needs a higher threshold
static Consensus::LLMQParams llmq400_85 = {
        .type = Consensus::LLMQ_400_85,
        .name = "llmq_400_85",
        .size = 400,
        .minSize = 350,
        .threshold = 340,

        .dkgInterval = 60 * 24, // one DKG every 24 hours
        .dkgPhaseBlocks = 10,
        .dkgMiningWindowStart = 50, // dkgPhaseBlocks * 5 = after finalization
        .dkgMiningWindowEnd = 70, // give it a larger mining window to make sure it is mined
        .dkgBadVotesThreshold = 300,

        .signingActiveQuorumCount = 4, // four days worth of LLMQs

        .keepOldConnections = 5,
        .recoveryMembers = 100,

        .cacheDkgInterval = 60 * 24 * 10, // dkgInterval * 10
};

/**
 * Main network
 */
/**
 * What makes a good checkpoint block?
 * + Is surrounded by blocks with reasonable timestamps
 *   (no blocks before with a timestamp after, none after with
 *    timestamp before)
 * + Contains no strange transactions
 */
// Rupaya: No hardcoded checkpoints — clean chain from genesis
static MapCheckpoints mapCheckpoints = {};

static const CCheckpointData data = {
    &mapCheckpoints,
    0,
    0,
    0
};

// Rupaya testnet: No checkpoints
static MapCheckpoints mapCheckpointsTestnet = {};

static const CCheckpointData dataTestnet = {
    &mapCheckpointsTestnet,
    0,
    0,
    0
};

static MapCheckpoints mapCheckpointsRegtest = {};
static const CCheckpointData dataRegtest = {
    &mapCheckpointsRegtest,
    0,
    0,
    0
};

class CMainParams : public CChainParams
{
public:
    CMainParams()
    {
        strNetworkID = "main";

        // Rupaya genesis — new timestamp, nonce, zero reward (premine is at block 1)
        genesis = CreateGenesisBlock(1722384000, 1, 0x1e0ffff0, 1, 0);
        consensus.hashGenesisBlock = genesis.GetHash();
        // Genesis hash and merkle root will be set after first build
        // assert(consensus.hashGenesisBlock == uint256S("0x..."));
        // assert(genesis.hashMerkleRoot == uint256S("0x..."));

        consensus.fPowAllowMinDifficultyBlocks = false;
        consensus.fPowNoRetargeting = false;
        consensus.powLimit   = uint256S("0x00000fffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");
        consensus.posLimitV1 = uint256S("0x000000ffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");
        consensus.posLimitV2 = uint256S("0x00000fffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");
        consensus.nBudgetCycleBlocks = 43200;       // approx. 30 days at 60s blocks
        consensus.nBudgetFeeConfirmations = 6;
        consensus.nCoinbaseMaturity = 100;
        consensus.nFutureTimeDriftPoW = 7200;       // only used in PoW phase (blocks 1-100)
        consensus.nFutureTimeDriftPoS = 180;
        consensus.nMaxMoneyOut = 100000000 * COIN;  // 100M total supply
        consensus.nMNCollateralAmt = 10000 * COIN;
        consensus.nMNBlockReward = 4 * COIN;         // 40% of 10 RUPX PoS reward
        consensus.nNewMNBlockReward = 4 * COIN;      // same — flat reward
        consensus.nMNCollateralMinConf = 15;
        consensus.nProposalEstablishmentTime = 60 * 60 * 24;
        consensus.nStakeMinAge = 60 * 60;            // 1 hour
        consensus.nStakeMinDepth = 600;
        consensus.nTargetTimespan = 40 * 60;
        consensus.nTargetTimespanV2 = 30 * 60;
        consensus.nTargetSpacing = 1 * 60;           // 60 second blocks
        consensus.nTimeSlotLength = 15;
        consensus.nMaxProposalPayments = 6;

        // spork keys — must be replaced with Rupaya keys before mainnet launch
        consensus.strSporkPubKey = "";
        consensus.strSporkPubKeyOld = "";
        consensus.nTime_EnforceNewSporkKey = 0;
        consensus.nTime_RejectOldSporkKey = 0;

        // Network upgrades — Rupaya starts with latest rules from block 1
        // PoS activates at block 101 (after 100-block PoW premine)
        consensus.vUpgrades[Consensus::BASE_NETWORK].nActivationHeight =
                Consensus::NetworkUpgrade::ALWAYS_ACTIVE;
        consensus.vUpgrades[Consensus::UPGRADE_TESTDUMMY].nActivationHeight =
                Consensus::NetworkUpgrade::NO_ACTIVATION_HEIGHT;
        consensus.vUpgrades[Consensus::UPGRADE_POS].nActivationHeight           = 101;
        consensus.vUpgrades[Consensus::UPGRADE_POS_V2].nActivationHeight        =
                Consensus::NetworkUpgrade::ALWAYS_ACTIVE;
        consensus.vUpgrades[Consensus::UPGRADE_BIP65].nActivationHeight         =
                Consensus::NetworkUpgrade::ALWAYS_ACTIVE;
        consensus.vUpgrades[Consensus::UPGRADE_V3_4].nActivationHeight          =
                Consensus::NetworkUpgrade::ALWAYS_ACTIVE;
        consensus.vUpgrades[Consensus::UPGRADE_V4_0].nActivationHeight          =
                Consensus::NetworkUpgrade::ALWAYS_ACTIVE;
        consensus.vUpgrades[Consensus::UPGRADE_V5_0].nActivationHeight          =
                Consensus::NetworkUpgrade::ALWAYS_ACTIVE;
        consensus.vUpgrades[Consensus::UPGRADE_V5_2].nActivationHeight          =
                Consensus::NetworkUpgrade::ALWAYS_ACTIVE;
        consensus.vUpgrades[Consensus::UPGRADE_V5_3].nActivationHeight          =
                Consensus::NetworkUpgrade::ALWAYS_ACTIVE;
        consensus.vUpgrades[Consensus::UPGRADE_V5_5].nActivationHeight          =
                Consensus::NetworkUpgrade::ALWAYS_ACTIVE;
        consensus.vUpgrades[Consensus::UPGRADE_V5_6].nActivationHeight          =
                Consensus::NetworkUpgrade::ALWAYS_ACTIVE;
        consensus.vUpgrades[Consensus::UPGRADE_V6_0].nActivationHeight =
                Consensus::NetworkUpgrade::NO_ACTIVATION_HEIGHT;  // DMN — future

        // No hashActivationBlock values
        consensus.vUpgrades[Consensus::UPGRADE_BIP65].hashActivationBlock = nullopt;
        consensus.vUpgrades[Consensus::UPGRADE_V3_4].hashActivationBlock = nullopt;
        consensus.vUpgrades[Consensus::UPGRADE_V4_0].hashActivationBlock = nullopt;

        /**
         * The message start string is designed to be unlikely to occur in normal data.
         * The characters are rarely used upper ASCII, not valid as UTF-8, and produce
         * a large 4-byte int at any alignment.
         */
        // Rupaya network magic bytes
        pchMessageStart[0] = 0x52;  // R
        pchMessageStart[1] = 0x55;  // U
        pchMessageStart[2] = 0x50;  // P
        pchMessageStart[3] = 0x41;  // A
        nDefaultPort = 8765;

        // DNS seeds — to be added when Rupaya seeders are operational
        // vSeeds.emplace_back("seed.rupaya.io", true);

        // Rupaya base58 prefixes — addresses start with 'R'
        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1, 60);    // R
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1, 16);  // R
        base58Prefixes[STAKING_ADDRESS] = std::vector<unsigned char>(1, 64);   // R
        base58Prefixes[EXCHANGE_ADDRESS] = {0x01, 0xb9, 0xa2};
        base58Prefixes[SECRET_KEY] = std::vector<unsigned char>(1, 180);
        base58Prefixes[EXT_PUBLIC_KEY] = {0x02, 0x2D, 0x25, 0x33};
        base58Prefixes[EXT_SECRET_KEY] = {0x02, 0x21, 0x31, 0x2B};
        // BIP44 coin type 499 (Rupaya) — SLIP-0044
        base58Prefixes[EXT_COIN_TYPE] = {0x80, 0x00, 0x01, 0xF3};

        // Fixed seed nodes (placeholder 127.0.0.1 — replace with real Rupaya nodes)
        vFixedSeeds = std::vector<uint8_t>(std::begin(chainparams_seed_main), std::end(chainparams_seed_main));

        // Reject non-standard transactions by default
        fRequireStandard = true;

        // Sapling — Rupaya SHIELD bech32 HRPs
        bech32HRPs[SAPLING_PAYMENT_ADDRESS]      = "rs";
        bech32HRPs[SAPLING_FULL_VIEWING_KEY]     = "rviews";
        bech32HRPs[SAPLING_INCOMING_VIEWING_KEY] = "rivks";
        bech32HRPs[SAPLING_EXTENDED_SPEND_KEY]   = "r-secret-spending-key-main";
        bech32HRPs[SAPLING_EXTENDED_FVK]         = "rxviews";

        bech32HRPs[BLS_SECRET_KEY]               = "bls-sk";
        bech32HRPs[BLS_PUBLIC_KEY]               = "bls-pk";

        // long living quorum params
        consensus.llmqs[Consensus::LLMQ_50_60] = llmq50_60;
        consensus.llmqs[Consensus::LLMQ_400_60] = llmq400_60;
        consensus.llmqs[Consensus::LLMQ_400_85] = llmq400_85;

        nLLMQConnectionRetryTimeout = 60;

        consensus.llmqTypeChainLocks = Consensus::LLMQ_400_60;

        // Tier two
        nFulfilledRequestExpireTime = 60 * 60; // fulfilled requests expire in 1 hour
    }

    const CCheckpointData& Checkpoints() const
    {
        return data;
    }

};

/**
 * Testnet (v5)
 */
class CTestNetParams : public CChainParams
{
public:
    CTestNetParams()
    {
        strNetworkID = "test";

        // Rupaya testnet genesis — zero reward
        genesis = CreateGenesisBlock(1722384000, 2, 0x1e0ffff0, 1, 0);
        consensus.hashGenesisBlock = genesis.GetHash();
        // assert(consensus.hashGenesisBlock == uint256S("0x..."));
        // assert(genesis.hashMerkleRoot == uint256S("0x..."));

        consensus.fPowAllowMinDifficultyBlocks = true;
        consensus.fPowNoRetargeting = false;
        consensus.powLimit   = uint256S("0x00000fffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");
        consensus.posLimitV1 = uint256S("0x000000ffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");
        consensus.posLimitV2 = uint256S("0x00000fffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");
        consensus.nBudgetCycleBlocks = 144;         // approx 10 cycles per day
        consensus.nBudgetFeeConfirmations = 3;
        consensus.nCoinbaseMaturity = 15;
        consensus.nFutureTimeDriftPoW = 7200;
        consensus.nFutureTimeDriftPoS = 180;
        consensus.nMaxMoneyOut = 100000000 * COIN;  // 100M total supply
        consensus.nMNCollateralAmt = 10000 * COIN;
        consensus.nMNBlockReward = 4 * COIN;
        consensus.nNewMNBlockReward = 4 * COIN;
        consensus.nMNCollateralMinConf = 15;
        consensus.nProposalEstablishmentTime = 60 * 5;
        consensus.nStakeMinAge = 60 * 60;
        consensus.nStakeMinDepth = 100;
        consensus.nTargetTimespan = 40 * 60;
        consensus.nTargetTimespanV2 = 30 * 60;
        consensus.nTargetSpacing = 1 * 60;
        consensus.nTimeSlotLength = 15;
        consensus.nMaxProposalPayments = 20;

        // spork keys — empty for now, set before testnet launch
        consensus.strSporkPubKey = "";
        consensus.strSporkPubKeyOld = "";
        consensus.nTime_EnforceNewSporkKey = 0;
        consensus.nTime_RejectOldSporkKey = 0;

        // Network upgrades — testnet starts with latest rules, PoS from block 201
        consensus.vUpgrades[Consensus::BASE_NETWORK].nActivationHeight =
                Consensus::NetworkUpgrade::ALWAYS_ACTIVE;
        consensus.vUpgrades[Consensus::UPGRADE_TESTDUMMY].nActivationHeight =
                Consensus::NetworkUpgrade::NO_ACTIVATION_HEIGHT;
        consensus.vUpgrades[Consensus::UPGRADE_POS].nActivationHeight           = 201;
        consensus.vUpgrades[Consensus::UPGRADE_POS_V2].nActivationHeight        = 201;
        consensus.vUpgrades[Consensus::UPGRADE_BIP65].nActivationHeight         = 201;
        consensus.vUpgrades[Consensus::UPGRADE_V3_4].nActivationHeight          = 201;
        consensus.vUpgrades[Consensus::UPGRADE_V4_0].nActivationHeight          = 201;
        consensus.vUpgrades[Consensus::UPGRADE_V5_0].nActivationHeight          = 201;
        consensus.vUpgrades[Consensus::UPGRADE_V5_2].nActivationHeight          = 201;
        consensus.vUpgrades[Consensus::UPGRADE_V5_3].nActivationHeight          = 201;
        consensus.vUpgrades[Consensus::UPGRADE_V5_5].nActivationHeight          = 201;
        consensus.vUpgrades[Consensus::UPGRADE_V5_6].nActivationHeight          = 201;
        consensus.vUpgrades[Consensus::UPGRADE_V6_0].nActivationHeight =
                Consensus::NetworkUpgrade::NO_ACTIVATION_HEIGHT;

        // Rupaya testnet magic bytes
        pchMessageStart[0] = 0x72;  // r
        pchMessageStart[1] = 0x75;  // u
        pchMessageStart[2] = 0x70;  // p
        pchMessageStart[3] = 0x54;  // T (testnet)
        nDefaultPort = 8766;

        // DNS seeds — to be added
        // vSeeds.emplace_back("testnet-seed.rupaya.io", true);

        // Rupaya testnet base58 prefixes
        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1, 111);   // t/n
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1, 196);   // 2
        base58Prefixes[STAKING_ADDRESS] = std::vector<unsigned char>(1, 125);  // t
        base58Prefixes[EXCHANGE_ADDRESS] = {0x01, 0xb9, 0xb1};
        base58Prefixes[SECRET_KEY] = std::vector<unsigned char>(1, 239);
        base58Prefixes[EXT_PUBLIC_KEY] = {0x3a, 0x80, 0x61, 0xa0};
        base58Prefixes[EXT_SECRET_KEY] = {0x3a, 0x80, 0x58, 0x37};
        // Testnet BIP44 coin type is '1' (All coins' testnet default)
        base58Prefixes[EXT_COIN_TYPE] = {0x80, 0x00, 0x00, 0x01};

        // Fixed seed nodes (placeholder 127.0.0.1 — replace with real Rupaya testnet nodes)
        vFixedSeeds = std::vector<uint8_t>(std::begin(chainparams_seed_test), std::end(chainparams_seed_test));

        fRequireStandard = false;

        // Sapling — Rupaya testnet bech32 HRPs
        bech32HRPs[SAPLING_PAYMENT_ADDRESS]      = "rstest";
        bech32HRPs[SAPLING_FULL_VIEWING_KEY]     = "rviewtest";
        bech32HRPs[SAPLING_INCOMING_VIEWING_KEY] = "rivktest";
        bech32HRPs[SAPLING_EXTENDED_SPEND_KEY]   = "r-secret-spending-key-test";
        bech32HRPs[SAPLING_EXTENDED_FVK]         = "rxviewtest";

        bech32HRPs[BLS_SECRET_KEY]               = "bls-sk-test";
        bech32HRPs[BLS_PUBLIC_KEY]               = "bls-pk-test";

        // long living quorum params
        consensus.llmqs[Consensus::LLMQ_50_60] = llmq50_60;
        consensus.llmqs[Consensus::LLMQ_400_60] = llmq400_60;
        consensus.llmqs[Consensus::LLMQ_400_85] = llmq400_85;

        nLLMQConnectionRetryTimeout = 60;

        consensus.llmqTypeChainLocks = Consensus::LLMQ_400_60;

        // Tier two
        nFulfilledRequestExpireTime = 60 * 60; // fulfilled requests expire in 1 hour
    }

    const CCheckpointData& Checkpoints() const
    {
        return dataTestnet;
    }
};

/**
 * Regression test
 */
class CRegTestParams : public CChainParams
{
public:
    CRegTestParams()
    {
        strNetworkID = "regtest";

        // Rupaya regtest genesis — zero reward
        genesis = CreateGenesisBlock(1722384000, 1, 0x207fffff, 1, 0);
        consensus.hashGenesisBlock = genesis.GetHash();
        // assert(consensus.hashGenesisBlock == uint256S("0x..."));
        // assert(genesis.hashMerkleRoot == uint256S("0x..."));

        consensus.fPowAllowMinDifficultyBlocks = true;
        consensus.fPowNoRetargeting = true;
        consensus.powLimit   = uint256S("0x7fffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");
        consensus.posLimitV1 = uint256S("0x000000ffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");
        consensus.posLimitV2 = uint256S("0x00000fffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");
        consensus.nBudgetCycleBlocks = 144;         // approx 10 cycles per day
        consensus.nBudgetFeeConfirmations = 3;      // (only 8-blocks window for finalization on regtest)
        consensus.nCoinbaseMaturity = 100;
        consensus.nFutureTimeDriftPoW = 7200;
        consensus.nFutureTimeDriftPoS = 180;
        consensus.nMaxMoneyOut = 100000000 * COIN;  // 100M total supply
        consensus.nMNCollateralAmt = 100 * COIN;
        consensus.nMNBlockReward = 4 * COIN;
        consensus.nNewMNBlockReward = 4 * COIN;
        consensus.nMNCollateralMinConf = 1;
        consensus.nProposalEstablishmentTime = 60 * 5;  // at least 5 min old to make it into a budget
        consensus.nStakeMinAge = 0;
        consensus.nStakeMinDepth = 20;
        consensus.nTargetTimespan = 40 * 60;
        consensus.nTargetTimespanV2 = 30 * 60;
        consensus.nTargetSpacing = 1 * 60;
        consensus.nTimeSlotLength = 15;
        consensus.nMaxProposalPayments = 20;

        /* Spork Key for RegTest:
        WIF private key: 932HEevBSujW2ud7RfB1YF91AFygbBRQj3de3LyaCRqNzKKgWXi
        private key hex: bd4960dcbd9e7f2223f24e7164ecb6f1fe96fc3a416f5d3a830ba5720c84b8ca
        Address: yCvUVd72w7xpimf981m114FSFbmAmne7j9
        */
        consensus.strSporkPubKey = "043969b1b0e6f327de37f297a015d37e2235eaaeeb3933deecd8162c075cee0207b13537618bde640879606001a8136091c62ec272dd0133424a178704e6e75bb7";
        consensus.strSporkPubKeyOld = "";
        consensus.nTime_EnforceNewSporkKey = 0;
        consensus.nTime_RejectOldSporkKey = 0;

        // Network upgrades
        consensus.vUpgrades[Consensus::BASE_NETWORK].nActivationHeight =
                Consensus::NetworkUpgrade::ALWAYS_ACTIVE;
        consensus.vUpgrades[Consensus::UPGRADE_TESTDUMMY].nActivationHeight =
                Consensus::NetworkUpgrade::NO_ACTIVATION_HEIGHT;
        consensus.vUpgrades[Consensus::UPGRADE_POS].nActivationHeight           = 251;
        consensus.vUpgrades[Consensus::UPGRADE_POS_V2].nActivationHeight        = 251;
        consensus.vUpgrades[Consensus::UPGRADE_BIP65].nActivationHeight         =
                Consensus::NetworkUpgrade::ALWAYS_ACTIVE;
        consensus.vUpgrades[Consensus::UPGRADE_V3_4].nActivationHeight          = 251;
        consensus.vUpgrades[Consensus::UPGRADE_V4_0].nActivationHeight          =
                Consensus::NetworkUpgrade::ALWAYS_ACTIVE;
        consensus.vUpgrades[Consensus::UPGRADE_V5_0].nActivationHeight          = 300;
        consensus.vUpgrades[Consensus::UPGRADE_V5_2].nActivationHeight          = 300;
        consensus.vUpgrades[Consensus::UPGRADE_V5_3].nActivationHeight          = 251;
        consensus.vUpgrades[Consensus::UPGRADE_V5_5].nActivationHeight          = 576;
        consensus.vUpgrades[Consensus::UPGRADE_V5_6].nActivationHeight          = 1000;
        consensus.vUpgrades[Consensus::UPGRADE_V6_0].nActivationHeight =
                Consensus::NetworkUpgrade::NO_ACTIVATION_HEIGHT;

        /**
         * The message start string is designed to be unlikely to occur in normal data.
         * The characters are rarely used upper ASCII, not valid as UTF-8, and produce
         * a large 4-byte int at any alignment.
         */
        // Rupaya regtest magic bytes
        pchMessageStart[0] = 0x72;  // r
        pchMessageStart[1] = 0x75;  // u
        pchMessageStart[2] = 0x70;  // p
        pchMessageStart[3] = 0x52;  // R (regtest)
        nDefaultPort = 8767;

        // Rupaya regtest base58 prefixes
        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1, 111);   // t/n
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1, 196);   // 2
        base58Prefixes[STAKING_ADDRESS] = std::vector<unsigned char>(1, 125);  // t
        base58Prefixes[EXCHANGE_ADDRESS] = {0x01, 0xb9, 0xb1};
        base58Prefixes[SECRET_KEY] = std::vector<unsigned char>(1, 239);
        // Regtest BIP32 pubkeys
        base58Prefixes[EXT_PUBLIC_KEY] = {0x3a, 0x80, 0x61, 0xa0};
        // Regtest BIP32 prvkeys
        base58Prefixes[EXT_SECRET_KEY] = {0x3a, 0x80, 0x58, 0x37};
        // Regtest BIP44 coin type is '1' (All coin's testnet default)
        base58Prefixes[EXT_COIN_TYPE] = {0x80, 0x00, 0x00, 0x01};

        // Reject non-standard transactions by default
        fRequireStandard = true;

        // Sapling — Rupaya regtest bech32 HRPs
        bech32HRPs[SAPLING_PAYMENT_ADDRESS]      = "rsregtest";
        bech32HRPs[SAPLING_FULL_VIEWING_KEY]     = "rviewregtest";
        bech32HRPs[SAPLING_INCOMING_VIEWING_KEY] = "rivkregtest";
        bech32HRPs[SAPLING_EXTENDED_SPEND_KEY]   = "r-secret-spending-key-regtest";
        bech32HRPs[SAPLING_EXTENDED_FVK]         = "rxviewregtest";

        bech32HRPs[BLS_SECRET_KEY]               = "bls-sk-test";
        bech32HRPs[BLS_PUBLIC_KEY]               = "bls-pk-test";

        // long living quorum params
        consensus.llmqs[Consensus::LLMQ_TEST] = llmq_test;
        nLLMQConnectionRetryTimeout = 10;

        consensus.llmqTypeChainLocks = Consensus::LLMQ_TEST;

        // Tier two
        nFulfilledRequestExpireTime = 60 * 60; // fulfilled requests expire in 1 hour
    }

    const CCheckpointData& Checkpoints() const
    {
        return dataRegtest;
    }
};

static std::unique_ptr<CChainParams> globalChainParams;

const CChainParams &Params()
{
    assert(globalChainParams);
    return *globalChainParams;
}

std::unique_ptr<CChainParams> CreateChainParams(const std::string& chain)
{
    if (chain == CBaseChainParams::MAIN)
        return std::unique_ptr<CChainParams>(new CMainParams());
    else if (chain == CBaseChainParams::TESTNET)
        return std::unique_ptr<CChainParams>(new CTestNetParams());
    else if (chain == CBaseChainParams::REGTEST)
        return std::unique_ptr<CChainParams>(new CRegTestParams());
    throw std::runtime_error(strprintf("%s: Unknown chain %s.", __func__, chain));
}

void SelectParams(const std::string& network)
{
    SelectBaseParams(network);
    globalChainParams = CreateChainParams(network);
}

void UpdateNetworkUpgradeParameters(Consensus::UpgradeIndex idx, int nActivationHeight)
{
    globalChainParams->UpdateNetworkUpgradeParameters(idx, nActivationHeight);
}
