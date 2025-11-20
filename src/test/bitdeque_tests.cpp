// Copyright (c) 2025 The Bitcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <util/bitdeque.h>
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_SUITE(bitdeque_tests)

BOOST_AUTO_TEST_CASE(bitdeque_constructor)
{
    bitdeque<> b;
    BOOST_CHECK(b.empty());
    BOOST_CHECK_EQUAL(b.size(), 0U);

    bitdeque<> b1(100);
    BOOST_CHECK(!b1.empty());
    BOOST_CHECK_EQUAL(b1.size(), 100U);
    for (size_t i = 0; i < 100; ++i) {
        BOOST_CHECK_EQUAL(b1[i], false);
    }

    bitdeque<> b2(100, true);
    BOOST_CHECK_EQUAL(b2.size(), 100U);
    for (size_t i = 0; i < 100; ++i) {
        BOOST_CHECK_EQUAL(b2[i], true);
    }

    bitdeque<> b3{true, false, true};
    BOOST_CHECK_EQUAL(b3.size(), 3U);
    BOOST_CHECK_EQUAL(b3[0], true);
    BOOST_CHECK_EQUAL(b3[1], false);
    BOOST_CHECK_EQUAL(b3[2], true);
}

BOOST_AUTO_TEST_CASE(bitdeque_push_pop)
{
    bitdeque<> b;
    b.push_back(true);
    BOOST_CHECK_EQUAL(b.size(), 1U);
    BOOST_CHECK_EQUAL(b.back(), true);
    BOOST_CHECK_EQUAL(b.front(), true);

    b.push_front(false);
    BOOST_CHECK_EQUAL(b.size(), 2U);
    BOOST_CHECK_EQUAL(b.front(), false);
    BOOST_CHECK_EQUAL(b.back(), true);

    b.pop_back();
    BOOST_CHECK_EQUAL(b.size(), 1U);
    BOOST_CHECK_EQUAL(b.front(), false);

    b.pop_front();
    BOOST_CHECK(b.empty());

    // Emplace
    b.emplace_back(true);
    BOOST_CHECK_EQUAL(b.back(), true);
    b.emplace_front(false);
    BOOST_CHECK_EQUAL(b.front(), false);
}

BOOST_AUTO_TEST_CASE(bitdeque_assign)
{
    bitdeque<> b;
    b.assign(10, true);
    BOOST_CHECK_EQUAL(b.size(), 10U);
    for (auto x : b) {
        BOOST_CHECK_EQUAL(x, true);
    }

    b.assign({false, true});
    BOOST_CHECK_EQUAL(b.size(), 2U);
    BOOST_CHECK_EQUAL(b[0], false);
    BOOST_CHECK_EQUAL(b[1], true);

    std::vector<bool> v{true, false, true};
    b.assign(v.begin(), v.end());
    BOOST_CHECK_EQUAL(b.size(), 3U);
    BOOST_CHECK_EQUAL(b[0], true);
    BOOST_CHECK_EQUAL(b[1], false);
    BOOST_CHECK_EQUAL(b[2], true);
}

BOOST_AUTO_TEST_CASE(bitdeque_resize)
{
    bitdeque<> b(5, true);
    b.resize(3);
    BOOST_CHECK_EQUAL(b.size(), 3U);
    BOOST_CHECK_EQUAL(b[0], true);

    b.resize(5);
    BOOST_CHECK_EQUAL(b.size(), 5U);
    BOOST_CHECK_EQUAL(b[3], false); // Default value for new elements is false
}

BOOST_AUTO_TEST_CASE(bitdeque_iterator)
{
    bitdeque<> b{true, false, true};
    auto it = b.begin();
    BOOST_CHECK_EQUAL(*it, true);
    ++it;
    BOOST_CHECK_EQUAL(*it, false);
    it++;
    BOOST_CHECK_EQUAL(*it, true);
    ++it;
    BOOST_CHECK(it == b.end());

    auto rit = b.rbegin();
    BOOST_CHECK_EQUAL(*rit, true);
    ++rit;
    BOOST_CHECK_EQUAL(*rit, false);
    rit++;
    BOOST_CHECK_EQUAL(*rit, true);
    ++rit;
    BOOST_CHECK(rit == b.rend());
}

BOOST_AUTO_TEST_CASE(bitdeque_iterator_random_access)
{
    bitdeque<> b{false, true, false, true, false};
    auto it = b.begin();

    // Arithmetic
    BOOST_CHECK_EQUAL(*(it + 1), true);
    BOOST_CHECK_EQUAL(*(1 + it), true);
    BOOST_CHECK_EQUAL(*(b.end() - 1), false);
    BOOST_CHECK_EQUAL((b.end() - b.begin()), 5);

    it += 3;
    BOOST_CHECK_EQUAL(*it, true);
    it -= 2;
    BOOST_CHECK_EQUAL(*it, true);

    // Comparisons
    auto it2 = b.begin() + 2; // it is at begin()+1, make it2 be begin()+2
    BOOST_CHECK(it < it2);
    BOOST_CHECK(it2 > it);
    BOOST_CHECK(it <= it2);
    BOOST_CHECK(it2 >= it);
    BOOST_CHECK(it != it2);
    BOOST_CHECK(it == b.begin() + 1);

    // Offset dereference
    BOOST_CHECK_EQUAL(it[2], true); // b[1+2] = b[3] = true
}

BOOST_AUTO_TEST_CASE(bitdeque_erase_insert)
{
    bitdeque<> b{true, true, true};
    b.insert(b.begin() + 1, false);
    BOOST_CHECK_EQUAL(b.size(), 4U);
    BOOST_CHECK_EQUAL(b[0], true);
    BOOST_CHECK_EQUAL(b[1], false);
    BOOST_CHECK_EQUAL(b[2], true);
    BOOST_CHECK_EQUAL(b[3], true);

    b.erase(b.begin() + 1);
    BOOST_CHECK_EQUAL(b.size(), 3U);
    BOOST_CHECK_EQUAL(b[0], true);
    BOOST_CHECK_EQUAL(b[1], true);
    BOOST_CHECK_EQUAL(b[2], true);

    b.insert(b.begin(), 2, false);
    BOOST_CHECK_EQUAL(b.size(), 5U);
    BOOST_CHECK_EQUAL(b[0], false);
    BOOST_CHECK_EQUAL(b[1], false);
    BOOST_CHECK_EQUAL(b[2], true);

    std::vector<bool> v{true, false};
    b.insert(b.end(), v.begin(), v.end());
    BOOST_CHECK_EQUAL(b.size(), 7U);
    BOOST_CHECK_EQUAL(b[5], true);
    BOOST_CHECK_EQUAL(b[6], false);

    b.erase(b.begin(), b.begin() + 2);
    BOOST_CHECK_EQUAL(b.size(), 5U);
    BOOST_CHECK_EQUAL(b[0], true);
}

BOOST_AUTO_TEST_CASE(bitdeque_swap)
{
    bitdeque<> b1{true, false};
    bitdeque<> b2{false, true, false};

    b1.swap(b2);
    BOOST_CHECK_EQUAL(b1.size(), 3U);
    BOOST_CHECK_EQUAL(b1[0], false);
    BOOST_CHECK_EQUAL(b2.size(), 2U);
    BOOST_CHECK_EQUAL(b2[0], true);

    swap(b1, b2);
    BOOST_CHECK_EQUAL(b1.size(), 2U);
    BOOST_CHECK_EQUAL(b1[0], true);
    BOOST_CHECK_EQUAL(b2.size(), 3U);
    BOOST_CHECK_EQUAL(b2[0], false);
}

BOOST_AUTO_TEST_CASE(bitdeque_clear)
{
    bitdeque<> b(100, true);
    b.clear();
    BOOST_CHECK(b.empty());
    BOOST_CHECK_EQUAL(b.size(), 0U);
}

BOOST_AUTO_TEST_CASE(bitdeque_access)
{
    bitdeque<> b{true, false, true};
    BOOST_CHECK_EQUAL(b.at(0), true);
    BOOST_CHECK_EQUAL(b.at(1), false);
    BOOST_CHECK_EQUAL(b.at(2), true);
    BOOST_CHECK_THROW(b.at(3), std::out_of_range);
}

BOOST_AUTO_TEST_CASE(bitdeque_small_word_boundary)
{
    // Test with a small BITS_PER_WORD to trigger word boundary logic
    bitdeque<64> b;
    // Fill up more than one word (64 bits)
    for (int i = 0; i < 70; ++i) {
        b.push_back(i % 2 == 0);
    }
    BOOST_CHECK_EQUAL(b.size(), 70U);
    for (int i = 0; i < 70; ++i) {
        BOOST_CHECK_EQUAL(b[i], i % 2 == 0);
    }

    // Remove from front to shift pad_begin
    for (int i = 0; i < 10; ++i) {
        b.pop_front();
    }
    BOOST_CHECK_EQUAL(b.size(), 60U);
    for (int i = 0; i < 60; ++i) {
        // Original index was i + 10
        BOOST_CHECK_EQUAL(b[i], (i + 10) % 2 == 0);
    }

    // Add to front, potentially crossing boundary backwards or filling up pad_begin
    for (int i = 0; i < 10; ++i) {
        b.push_front(true);
    }
    BOOST_CHECK_EQUAL(b.size(), 70U);
    for (int i = 0; i < 10; ++i) {
        BOOST_CHECK_EQUAL(b[i], true);
    }
}

BOOST_AUTO_TEST_SUITE_END()
