// Copyright (c) 2012-2014 The Bitcoin Core developers
// Copyright (c) 2017-2019 The PIVX Core developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "script/script.h"
#include "test/test_pivx.h"

#include <boost/test/unit_test.hpp>
#include <limits.h>
#include <stdint.h>

BOOST_FIXTURE_TEST_SUITE(scriptnum_tests, BasicTestingSetup)

/** A selection of numbers that do not trigger int64_t overflow
 *  when added/subtracted. */
static const int64_t values[] = { 0, 1, -2, 127, 128, -255, 256, (1LL << 15) - 1, -(1LL << 16), (1LL << 24) - 1, (1LL << 31), 1 - (1LL << 32), 1LL << 40 };

static const int64_t offsets[] = { 1, 0x79, 0x80, 0x81, 0xFF, 0x7FFF, 0x8000, 0xFFFF, 0x10000};

static void CheckCreateVch(const long& num)
{
    CScriptNum scriptnum(num);
    CScriptNum scriptnum2(scriptnum.getvch(), false);
    BOOST_CHECK(scriptnum == scriptnum2);

    CScriptNum scriptnum3(scriptnum2.getvch(), false);
    BOOST_CHECK(scriptnum == scriptnum3);
}

static void CheckCreateInt(const long& num)
{
    CScriptNum scriptnum(num);
    BOOST_CHECK(scriptnum.getint() == num);
    BOOST_CHECK(CScriptNum(scriptnum.getint()).getvch() == scriptnum.getvch());
}

static void CheckAdd(const long& num1, const long& num2)
{
    const CScriptNum scriptnum1(num1);
    const CScriptNum scriptnum2(num2);
    CScriptNum scriptnum3(num1);
    CScriptNum scriptnum4(num1);

    // int64_t overflow is undefined.
    bool invalid = (((num2 > 0) && (num1 > (std::numeric_limits<long>::max() - num2))) ||
                    ((num2 < 0) && (num1 < (std::numeric_limits<long>::min() - num2))));
    if (!invalid)
    {
        BOOST_CHECK(scriptnum1 + scriptnum2 == CScriptNum(num1 + num2));
        BOOST_CHECK(scriptnum1 + num2 == CScriptNum(num1 + num2));
        BOOST_CHECK(scriptnum2 + num1 == CScriptNum(num1 + num2));
    }
}

static void CheckNegate(const long& num)
{
    const CScriptNum scriptnum(num);

    // -INT64_MIN is undefined
    if (num != std::numeric_limits<long>::min())
        BOOST_CHECK(-scriptnum == CScriptNum(-num));
}

static void CheckSubtract(const long& num1, const long& num2)
{
    const CScriptNum scriptnum1(num1);
    const CScriptNum scriptnum2(num2);
    bool invalid = false;

    // int64_t overflow is undefined.
    invalid = ((num2 > 0 && num1 < std::numeric_limits<long>::min() + num2) ||
               (num2 < 0 && num1 > std::numeric_limits<long>::max() + num2));
    if (!invalid)
    {
        BOOST_CHECK(scriptnum1 - scriptnum2 == CScriptNum(num1 - num2));
        BOOST_CHECK(scriptnum1 - num2 == CScriptNum(num1 - num2));
    }

    invalid = ((num1 > 0 && num2 < std::numeric_limits<long>::min() + num1) ||
               (num1 < 0 && num2 > std::numeric_limits<long>::max() + num1));
    if (!invalid)
    {
        BOOST_CHECK(scriptnum2 - scriptnum1 == CScriptNum(num2 - num1));
        BOOST_CHECK(scriptnum2 - num1 == CScriptNum(num2 - num1));
    }
}

static void CheckCompare(const long& num1, const long& num2)
{
    const CScriptNum scriptnum1(num1);
    const CScriptNum scriptnum2(num2);

    BOOST_CHECK((scriptnum1 == scriptnum1) == (num1 == num1));
    BOOST_CHECK((scriptnum1 != scriptnum1) ==  (num1 != num1));
    BOOST_CHECK((scriptnum1 < scriptnum1) ==  (num1 < num1));
    BOOST_CHECK((scriptnum1 > scriptnum1) ==  (num1 > num1));
    BOOST_CHECK((scriptnum1 >= scriptnum1) ==  (num1 >= num1));
    BOOST_CHECK((scriptnum1 <= scriptnum1) ==  (num1 <= num1));

    BOOST_CHECK((scriptnum1 == scriptnum1) == (num1 == num1));
    BOOST_CHECK((scriptnum1 != scriptnum1) ==  (num1 != num1));
    BOOST_CHECK((scriptnum1 < scriptnum1) ==  (num1 < num1));
    BOOST_CHECK((scriptnum1 > scriptnum1) ==  (num1 > num1));
    BOOST_CHECK((scriptnum1 >= scriptnum1) ==  (num1 >= num1));
    BOOST_CHECK((scriptnum1 <= scriptnum1) ==  (num1 <= num1));

    BOOST_CHECK((scriptnum1 == scriptnum2) ==  (num1 == num2));
    BOOST_CHECK((scriptnum1 != scriptnum2) ==  (num1 != num2));
    BOOST_CHECK((scriptnum1 < scriptnum2) ==  (num1 < num2));
    BOOST_CHECK((scriptnum1 > scriptnum2) ==  (num1 > num2));
    BOOST_CHECK((scriptnum1 >= scriptnum2) ==  (num1 >= num2));
    BOOST_CHECK((scriptnum1 <= scriptnum2) ==  (num1 <= num2));

    BOOST_CHECK((scriptnum1 == scriptnum2) ==  (num1 == num2));
    BOOST_CHECK((scriptnum1 != scriptnum2) ==  (num1 != num2));
    BOOST_CHECK((scriptnum1 < scriptnum2) ==  (num1 < num2));
    BOOST_CHECK((scriptnum1 > scriptnum2) ==  (num1 > num2));
    BOOST_CHECK((scriptnum1 >= scriptnum2) ==  (num1 >= num2));
    BOOST_CHECK((scriptnum1 <= scriptnum2) ==  (num1 <= num2));
}

static void RunCreate(const long& num)
{
    CheckCreateInt(num);
    CScriptNum scriptnum(num);
    if (scriptnum.getvch().size() <= CScriptNum::nDefaultMaxNumSize)
        CheckCreateVch(num);
    else
    {
        BOOST_CHECK_THROW (CheckCreateVch(num), scriptnum_error);
    }
}

static void RunOperators(const long& num1, const int64_t& num2)
{
    CheckAdd(num1, num2);
    CheckSubtract(num1, num2);
    CheckNegate(num1);
    CheckCompare(num1, num2);
}

BOOST_AUTO_TEST_CASE(creation)
{
    for(size_t i = 0; i < sizeof(values) / sizeof(values[0]); ++i)
    {
        for(size_t j = 0; j < sizeof(offsets) / sizeof(offsets[0]); ++j)
        {
            RunCreate(values[i]);
            RunCreate(values[i] + offsets[j]);
            RunCreate(values[i] - offsets[j]);
        }
    }
}

BOOST_AUTO_TEST_CASE(operators)
{
    for(size_t i = 0; i < sizeof(values) / sizeof(values[0]); ++i)
    {
        for(size_t j = 0; j < sizeof(offsets) / sizeof(offsets[0]); ++j)
        {
            RunOperators(values[i], values[i]);
            RunOperators(values[i], -values[i]);
            RunOperators(values[i], values[j]);
            RunOperators(values[i], -values[j]);
            RunOperators(values[i] + values[j], values[j]);
            RunOperators(values[i] + values[j], -values[j]);
            RunOperators(values[i] - values[j], values[j]);
            RunOperators(values[i] - values[j], -values[j]);
            RunOperators(values[i] + values[j], values[i] + values[j]);
            RunOperators(values[i] + values[j], values[i] - values[j]);
            RunOperators(values[i] - values[j], values[i] + values[j]);
            RunOperators(values[i] - values[j], values[i] - values[j]);
        }
    }
}

BOOST_AUTO_TEST_SUITE_END()
