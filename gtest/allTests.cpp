#include "gtest/gtest.h"
#include "flat_tree.h"

#include "test_insert.cpp"
#include "test_erase.cpp"
#include "test_iterator.cpp"
#include "test_member.cpp"
#include "test_compare.cpp"


int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

