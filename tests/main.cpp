#include <foxogram/Me.h>
#include <dotenv.h>
#include <gtest/gtest.h>

TEST(add_test, Me) {
    dotenv::init();
    std::string emailCode, deleteCode;
//    auto u = foxogram::Me(dotenv::getenv("USERNAME"), dotenv::getenv("EMAIL"),
//                          dotenv::getenv("PASSWORD"));
//    EXPECT_EQ(foxogram::Me(dotenv::getenv("USERNAME"), dotenv::getenv("EMAIL"),
//                           dotenv::getenv("PASSWORD")).getUsername(), std::string(dotenv::getenv("USERNAME")));
//    u.deleteUser(dotenv::getenv("PASSWORD"));
//    EXPECT_TRUE(u.confirmDeleteUser())
}

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);

    return RUN_ALL_TESTS();
}