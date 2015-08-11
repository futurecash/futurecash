// Copyright (c) 2010 Satoshi Nakamoto
// Copyright (c) 2009-2012 The Bitcoin developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "assert.h"

#include "chainparams.h"
#include "main.h"
#include "util.h"

#include <boost/assign/list_of.hpp>

using namespace boost::assign;

struct SeedSpec6 {
    uint8_t addr[16];
    uint16_t port;
};

#include "chainparamsseeds.h"

//
// Main network
//

// Convert the pnSeeds6 array into usable address objects.
static void convertSeed6(std::vector<CAddress> &vSeedsOut, const SeedSpec6 *data, unsigned int count)
{
    // It'll only connect to one or two seed nodes because once it connects,
    // it'll get a pile of addresses with newer timestamps.
    // Seed nodes are given a random 'last seen time' of between one and two
    // weeks ago.
    const int64_t nOneWeek = 7*24*60*60;
    for (unsigned int i = 0; i < count; i++)
    {
        struct in6_addr ip;
        memcpy(&ip, data[i].addr, sizeof(ip));
        CAddress addr(CService(ip, data[i].port));
        addr.nTime = GetTime() - GetRand(nOneWeek) - nOneWeek;
        vSeedsOut.push_back(addr);
    }
}

class CMainParams : public CChainParams {
public:
    CMainParams() {
        // The message start string is designed to be unlikely to occur in normal data.
        // The characters are rarely used upper ASCII, not valid as UTF-8, and produce
        // a large 4-byte int at any alignment.
        pchMessageStart[0] = 0x9c;
        pchMessageStart[1] = 0x85;
        pchMessageStart[2] = 0x19;
        pchMessageStart[3] = 0x2f;
        vAlertPubKey = ParseHex("04ddab6b53a1ddb980cc5fbe689fce5855f55f0d1a0df8b9f027731c05c6bf2dd25dcf63d6fc7e816bd18fed0a92bc8e8f9783bf01e721c154915882d63fb28d07");
        vchSyncCheckpointPubKey = ParseHex("044b950f86bc631b6fd7bdcfa5331a2716a7c279e92849ea02e13819a14da9d774f16acc092d5148dc4f3604bd054c3673e5cad11514fe1b2eaa939f8b030d1411");
        nDefaultPort = 20005;
        nRPCPort = 20007;
        bnProofOfWorkLimit = CBigNum(~uint256(0) >> 16);

        // Build the genesis block. Note that the output of the genesis coinbase cannot
        // be spent as it did not originally exist in the database.
        const char* pszTimestamp = "May 23, 1983. Emmett Brown Commended. Local Inventor Receives Civic Award.";
        std::vector<CTxIn> vin;
        vin.resize(1);
        vin[0].scriptSig = CScript() << 0 << CBigNum(42) << vector<unsigned char>((const unsigned char*)pszTimestamp, (const unsigned char*)pszTimestamp + strlen(pszTimestamp));
        std::vector<CTxOut> vout;
        vout.resize(1);
        vout[0].SetEmpty();
        CTransaction txNew(1, 1439337600, vin, vout, 0);
        genesis.vtx.push_back(txNew);
        genesis.hashPrevBlock = 0;
        genesis.hashMerkleRoot = genesis.BuildMerkleTree();
        genesis.nVersion = 1;
        genesis.nTime    = 1439337600;
        genesis.nBits    = bnProofOfWorkLimit.GetCompact();
        genesis.nNonce   = 167190;

        hashGenesisBlock = uint256("0x0000fbad6508164792ff5e4782f89811d188a391bc5dd47fee4af46bf346014d");
        if (false && (genesis.GetHash() != hashGenesisBlock))
        {
            uint256 hashTarget = CBigNum().SetCompact(genesis.nBits).getuint256();
            while (genesis.GetHash() > hashTarget)
            {
                ++genesis.nNonce;
                if (genesis.nNonce == 0)
                {
                    printf("NONCE WRAPPED, incrementing time");
                    ++genesis.nTime;
                }
            }
            cout << "genesis: \n" << genesis.ToString() << endl;
            cout << "genesis.GetHash(): " << genesis.GetHash().ToString() << endl;
            cout << "genesis.hashMerkleRoot: " << genesis.hashMerkleRoot.ToString() << endl;
            cout << "genesis.nTime: " << genesis.nTime << endl;
            cout << "genesis.nNonce: " << genesis.nNonce << endl;
        }

        assert(hashGenesisBlock == uint256("0x0000fbad6508164792ff5e4782f89811d188a391bc5dd47fee4af46bf346014d"));
        assert(genesis.hashMerkleRoot == uint256("0xe3833561997100c1973551777205f817d3768ac19cc7c8ea96cb28705555e0c8"));

        base58Prefixes[PUBKEY_ADDRESS] = list_of(96);
        base58Prefixes[SCRIPT_ADDRESS] = list_of(85);
        base58Prefixes[SECRET_KEY] =     list_of(224);
        base58Prefixes[EXT_PUBLIC_KEY] = list_of(0x04)(0x88)(0xB2)(0x1E);
        base58Prefixes[EXT_SECRET_KEY] = list_of(0x04)(0x88)(0xAD)(0xE4);

        convertSeed6(vFixedSeeds, pnSeed6_main, ARRAYLEN(pnSeed6_main));

        bnProofOfStakeLimit = CBigNum(~uint256(0));
        bnProofOfStakeLimitV2 = CBigNum(~uint256(0));

        nBasicPoWReward = 1800 * COIN;
        nLaunchTime = genesis.nTime;
        nMinDelay = 2; //seconds

        nCoinbaseMaturity = 120;
        nStakeMinConfirmations = 120;

        nStakeMinAge = 60 * 60; // 60 minutes
        nModifierInterval = 10 * 60; // time to elapse before new modifier is computed

        nStakeCoinYearReward = 20 * CENT;

        nLastPOWBlock = 2980;
        nFirstPoSBlock = 2780;
       // nPoSGranularity = 7;
       // nTargetSpacing =  (nPoSGranularity + 1) * 4;
        nTargetSpacing = 60;
        nTargetTimespan = 10 * nTargetSpacing;

        nMaxMoney = nLastPOWBlock * nBasicPoWReward;
    }

    virtual const CBlock& GenesisBlock() const { return genesis; }
    virtual Network NetworkID() const { return CChainParams::MAIN; }

    virtual const vector<CAddress>& FixedSeeds() const {
        return vFixedSeeds;
    }
protected:
    CBlock genesis;
    vector<CAddress> vFixedSeeds;
};
static CMainParams mainParams;


//
// Testnet
//

class CTestNetParams : public CMainParams {
public:
    CTestNetParams() {
        // The message start string is designed to be unlikely to occur in normal data.
        // The characters are rarely used upper ASCII, not valid as UTF-8, and produce
        // a large 4-byte int at any alignment.
        pchMessageStart[0] = 0x9d;
        pchMessageStart[1] = 0x86;
        pchMessageStart[2] = 0x1a;
        pchMessageStart[3] = 0x30;

        bnProofOfWorkLimit = CBigNum(~uint256(0) >> 10);
        vAlertPubKey = ParseHex("04ddab6b53a1ddb980cc5fbe689fce5855f55f0d1a0df8b9f027731c05c6bf2dd25dcf63d6fc7e816bd18fed0a92bc8e8f9783bf01e721c154915882d63fb28d07");
        nDefaultPort = 20010;
        nRPCPort = 20012;
        strDataDir = "testnet";

        // Modify the testnet genesis block so the timestamp is valid for a later start.
        genesis.nBits  = bnProofOfWorkLimit.GetCompact();
        genesis.nNonce = 514471;
        hashGenesisBlock = uint256("0x001690ba4ea8ee5d4c6b9da04fd573900b8c1ed34cb1272b54e30464c710bdf8");
        if (false && (genesis.GetHash() != hashGenesisBlock))
        {
            uint256 hashTarget = CBigNum().SetCompact(genesis.nBits).getuint256();
            while (genesis.GetHash() > hashTarget)
            {
                ++genesis.nNonce;
                if (genesis.nNonce == 0)
                {
                    printf("NONCE WRAPPED, incrementing time");
                    ++genesis.nTime;
                }
            }
            cout << "genesis: \n" << genesis.ToString() << endl;
            cout << "genesis.GetHash(): " << genesis.GetHash().ToString() << endl;
            cout << "genesis.hashMerkleRoot: " << genesis.hashMerkleRoot.ToString() << endl;
            cout << "genesis.nTime: " << genesis.nTime << endl;
            cout << "genesis.nNonce: " << genesis.nNonce << endl;
        }

        assert(hashGenesisBlock == uint256("0x001690ba4ea8ee5d4c6b9da04fd573900b8c1ed34cb1272b54e30464c710bdf8"));

        vFixedSeeds.clear();
        vSeeds.clear();

        base58Prefixes[PUBKEY_ADDRESS] = list_of(127);
        base58Prefixes[SCRIPT_ADDRESS] = list_of(196);
        base58Prefixes[SECRET_KEY]     = list_of(255);
        base58Prefixes[EXT_PUBLIC_KEY] = list_of(0x04)(0x35)(0x87)(0xCF);
        base58Prefixes[EXT_SECRET_KEY] = list_of(0x04)(0x35)(0x83)(0x94);

        convertSeed6(vFixedSeeds, pnSeed6_test, ARRAYLEN(pnSeed6_test));

        bnProofOfStakeLimit = CBigNum(~uint256(0) >> 20);

        nCoinbaseMaturity = 10;
        nStakeMinConfirmations = 10;

        nStakeMinAge = 60 * 60;
        nModifierInterval = 10 * 60;

        nStakeCoinYearReward = 10 * CENT; // 10% per year

        nLastPOWBlock = 0x7fffffff;

        nMaxMoney = 2000000000 * COIN;
    }
    virtual Network NetworkID() const { return CChainParams::TESTNET; }
};
static CTestNetParams testNetParams;


//
// Regression test
//
class CRegTestParams : public CTestNetParams {
public:
    CRegTestParams() {
        pchMessageStart[0] = 0xf0;
        pchMessageStart[1] = 0xf1;
        pchMessageStart[2] = 0xf2;
        pchMessageStart[3] = 0xf3;
        bnProofOfWorkLimit = CBigNum(~uint256(0) >> 1);
        genesis.nTime = 1411111111;
        genesis.nBits  = bnProofOfWorkLimit.GetCompact();
        genesis.nNonce = 516113;
        hashGenesisBlock = genesis.GetHash();
        nDefaultPort = 18444;
        strDataDir = "regtest";
        cout << "regtest genesis.GetHash(): " << genesis.GetHash().ToString() << endl;
        assert(hashGenesisBlock == uint256("0xee49bd39ff3183e373d488b94fc3f22201c0357b312aa6447612bffba388a06e"));

        vSeeds.clear();  // Regtest mode doesn't have any DNS seeds.
    }

    virtual bool RequireRPCPassword() const { return false; }
    virtual Network NetworkID() const { return CChainParams::REGTEST; }
};
static CRegTestParams regTestParams;

static CChainParams *pCurrentParams = &mainParams;

const CChainParams &Params() {
    return *pCurrentParams;
}

void SelectParams(CChainParams::Network network) {
    switch (network) {
        case CChainParams::MAIN:
            pCurrentParams = &mainParams;
            break;
        case CChainParams::TESTNET:
            pCurrentParams = &testNetParams;
            break;
        case CChainParams::REGTEST:
            pCurrentParams = &regTestParams;
            break;
        default:
            assert(false && "Unimplemented network");
            return;
    }
}

bool SelectParamsFromCommandLine() {
    bool fRegTest = GetBoolArg("-regtest", false);
    bool fTestNet = GetBoolArg("-testnet", false);

    if (fTestNet && fRegTest) {
        return false;
    }

    if (fRegTest) {
        SelectParams(CChainParams::REGTEST);
    } else if (fTestNet) {
        SelectParams(CChainParams::TESTNET);
    } else {
        SelectParams(CChainParams::MAIN);
    }
    return true;
}
