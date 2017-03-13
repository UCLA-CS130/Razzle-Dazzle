#include "gtest/gtest.h"
#include "server.hpp"
#include "config_parser.h"
#include <string>
#include <thread>
#include <stdlib.h> 


NginxConfig retGoodConfig()
{
    NginxConfigParser parser;
    NginxConfig test_config;
    parser.Parse("config_file", &test_config);
    return test_config;
}

void waitAndKill(http::server::server test)
{
    sleep(5000);
    test.kill();
}

TEST(ServerTest, GoodConfigs)
{
    NginxConfig test_config = retGoodConfig();
    EXPECT_NO_THROW(http::server::server test_server("127.0.0.1", test_config));
    http::server::server test("0.0.0.0", test_config);
    EXPECT_TRUE(test.get_config_info(test_config));
    
    std::thread t1(waitAndKill, test);
    test.run();
    t1.join();
}
