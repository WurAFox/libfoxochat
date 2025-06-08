#define FOXOCHAT_URL "-dev.foxochat.app"

#include <foxochat/Me.h>
#include <foxochat/Logger.h>
#include <random>
#include <string>
#include <gtest/gtest.h>
#include <foxochat/exceptions.h>

std::string generateRandomString(int length) {
    const std::string CHARACTERS
            = "abcdefghijklmnopqrstuvwxyz0123456789";

    std::random_device rd;
    std::mt19937 generator(rd());

    std::uniform_int_distribution<> distribution(
            0, CHARACTERS.size() - 1);

    std::string random_string;
    for (int i = 0; i < length; ++i) {
        random_string
                += CHARACTERS[distribution(generator)];
    }

    return random_string;
}

TEST(add_test, Me) {
    foxochat::Logger::setLogLevel(foxochat::LOG_DEBUG);
    std::string username = generateRandomString(12);
    std::string email = username+"@foxochat.app";
    std::string password = generateRandomString(12);
    auto u = foxochat::Me(username, email, password);
    EXPECT_EQ(u.getUsername(), username);
    EXPECT_TRUE(u.resendEmail());
    // EXPECT_TRUE(u.verifyEmail("000000"));
    EXPECT_TRUE(u.deleteUser(password));
    // EXPECT_TRUE(u.confirmDeleteUser("000000"));
}

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);

    return RUN_ALL_TESTS();
}