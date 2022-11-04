// Copyright (c) 2022 LucaWei. All rights reserved.
#include <gtest/gtest.h>
#include <omp.h>
#include <lcs/lcs.hpp>

class demo_test: public testing::Test {
 protected:
    static void SetUpTestCase() {
#ifdef _OPENMP
    std::cout << _OPENMP << std::endl;
    #pragma omp parallel
        #pragma omp single
    std::cout << "num_threads= " << omp_get_num_threads() << std::endl;
#endif
    }

    static void TearDownTestCase() {
    }
};

TEST_F(demo_test, test1) {
    std::cout << "Hello World!!" << std::endl;
}

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
