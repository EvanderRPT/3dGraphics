#include "gtest/gtest.h"
#include "pch.h"
#include <algorithm>
// Function to test
int Add(int a, int b) {
    return a + b;
}
void swap(int& a, int& b) {
    int tmp = a;
    a = b;
    b = a;
}
int test1() {
    int a = 10;
    int b = 2;
    std::swap(a, b);
    if (a == 2 && b == 10) {
        return 0;
    }
    return 1;
    
}
// Google Test
TEST(MathTest, AddFunction) {
 
    EXPECT_EQ(test1(), 0);
    //EXPECT_EQ(test1(), 1);

   /* EXPECT_EQ(Add(2, 3), 5);
    EXPECT_EQ(Add(-1, 1), 0);*/
    //EXPECT_EQ(Add(-1, 1), 1);
}

// Main function for running all tests
int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}