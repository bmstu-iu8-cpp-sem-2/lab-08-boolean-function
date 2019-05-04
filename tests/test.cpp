// Copyright 2018 Your Name <your_email>

#include <gtest/gtest.h>

#include <boolean_function.hpp>

TEST(BooleanFunction, Construct)
{
    std::string s = "00001110";
    std::vector<bool> vec = {0, 0, 0, 0, 1, 1, 1, 0};
    std::initializer_list<bool> list = {0, 0, 0, 0, 1, 1, 1, 0};
    boolean_function bf1(vec);
    boolean_function bf2(s);
    EXPECT_TRUE(bf1 == bf2);
    EXPECT_TRUE(bf1 == boolean_function(list));
    EXPECT_TRUE(bf1 == boolean_function(bf2));

    std::string s1 = "11110000";
    boolean_function bf3(s1);
    bf2 = bf3;
    EXPECT_FALSE(bf1 == bf2);
    EXPECT_TRUE(bf1 == boolean_function(14, 3));
    vec = {0, 0, 0, 0, 0, 0, 0, 0};
    EXPECT_TRUE(boolean_function(vec) == bf1.zero(3));
    vec = {1, 1, 1, 1, 1, 1, 1, 1};
    EXPECT_TRUE(boolean_function(vec) == bf1.one(3));
    vec = {0, 1, 0, 1, 0, 1, 0, 1};
    EXPECT_TRUE(bf1.var(0, 3) == boolean_function(vec));
}

TEST(BooleanFunction, Functions)
{
    std::vector<bool> vec = {0, 0, 0, 0, 1, 1, 1, 0};
    boolean_function bf1(vec);
    EXPECT_TRUE((bf1 + bf1) == bf1.zero(3));
    EXPECT_TRUE((bf1 | bf1) == bf1);

    vec = {1, 1, 0, 0, 1, 1, 1, 0};
    boolean_function bf2(vec);
    vec = {1, 1, 0, 0, 0, 0, 0, 0};
    bf1 += bf2;
    EXPECT_TRUE((bf1 * bf2) == boolean_function(vec));
    EXPECT_TRUE(bf1 == boolean_function(vec));
    EXPECT_TRUE((bf1 * bf1) == bf1);

    vec = {1, 1, 0, 0, 1, 1, 1, 0};
    EXPECT_TRUE((bf1 | bf2) == boolean_function(vec));

    vec = {1, 1, 0, 0, 0, 0, 0};
    EXPECT_THROW((bf1 | boolean_function(vec)), std::invalid_argument);
    EXPECT_THROW(bf1 + boolean_function(vec), std::invalid_argument);
    EXPECT_THROW(bf1 * boolean_function(vec), std::invalid_argument);
}

TEST(BooleanFunction, Functions2)
{
    std::vector<bool> vec = {0, 0, 0, 0, 1, 1, 1, 0};
    boolean_function bf1(vec);
    EXPECT_TRUE(bf1.weight() == 3);
    EXPECT_TRUE(bf1.dimension() == 3);
    EXPECT_FALSE(bf1.is_balanced());

    vec = {1, 1, 1, 1, 0, 0, 0, 1};
    EXPECT_TRUE(~bf1 == boolean_function(vec));

    vec = {0, 0, 0, 1, 1, 1, 1, 0};
    EXPECT_TRUE(boolean_function(vec).weight() == 4);
    EXPECT_TRUE(boolean_function(vec).is_balanced());
}

TEST(BooleanFunction, Operators)
{
    std::vector<bool> vec = {0, 0, 0, 0, 1, 1, 1, 0};
    std::vector<bool> vec1 = {1, 1, 0};
    EXPECT_TRUE(boolean_function (vec)(vec1) == 1);
    EXPECT_TRUE(boolean_function (vec)({1, 1, 0}) == 1);

    std::vector<boolean_function> vecOfBF;
    vecOfBF.push_back(boolean_function(vec));
    vecOfBF.push_back(boolean_function(vec1));
    vecOfBF.push_back(boolean_function(vec));
    vec1 = {1, 0, 1, 1, 0, 0, 1, 1};
    std::vector<bool> vec2 = {0, 1, 1, 1, 1, 1, 1, 1};
    EXPECT_TRUE(boolean_function (vec1)(vecOfBF) == boolean_function(vec2));
}

TEST(BooleanFunction, Anf)
{
    std::vector<bool> table = {1, 0, 1, 0, 0, 1, 1, 0};
    boolean_function bf(table);
    std::vector<bool> vec = {1, 1, 0, 0, 1, 0, 1, 0};
    EXPECT_TRUE(bf.anf() == vec);
    EXPECT_TRUE(bf.from_anf(bf.anf()) == table);

    table = {1, 0, 0, 0, 1, 1, 0, 1};
    vec = {1 , 1, 1, 1, 0, 1, 0, 0};
    bf = boolean_function(table);
    EXPECT_TRUE(boolean_function(table).anf() == vec);
    EXPECT_TRUE(bf.from_anf(bf.anf()) == table);

    table = {0, 1, 1, 1, 0, 0, 0, 0};
    vec = {0 , 1, 1, 1, 0, 1, 1, 1};
    bf = boolean_function(table);
    EXPECT_TRUE(boolean_function(table).anf() == vec);
    EXPECT_TRUE(bf.from_anf(bf.anf()) == table);
}

TEST(BooleanFunction, Monotone)
{
    std::vector<bool> table = {1, 0, 1, 0, 0, 1, 1, 0};
    std::vector<bool> vec = {1, 1, 0, 0, 1, 0, 1, 0};
    EXPECT_THROW(boolean_function(table) >= boolean_function(vec), std::logic_error);
    EXPECT_TRUE(boolean_function(table).one(3) >= boolean_function(table).zero(3));

    table = {0, 1, 0, 0, 0, 0, 0, 0};
    EXPECT_FALSE(boolean_function(table) >= boolean_function(vec));
    boolean_function(table).is_monotone();
}
