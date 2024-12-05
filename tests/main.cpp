#define FOXOGRAM_BASE_API_URL "https://api.dev." FOXOGRAM_URL FOXOGRAM_API_VERSION

#include <foxogram/Me.h>
#include <foxogram/Logger.h>
#include <random>
#include <string>
#include <gtest/gtest.h>

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
    foxogram::Payload i("GET", "/1");
    foxogram::Logger::setLogLevel(foxogram::LOG_DEBUG);
    std::string username = generateRandomString(12);
    std::string email = generateRandomString(12)+"@foxogram.su";
    std::string password = generateRandomString(12);
    auto u = foxogram::Me(username, email, password);
    EXPECT_EQ(u.getUsername(), username);
    EXPECT_EQ(foxogram::Me(email, password).getUsername(), username);
    EXPECT_TRUE(u.deleteUser(password));
}

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);

    return RUN_ALL_TESTS();
}