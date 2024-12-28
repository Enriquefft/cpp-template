#include "utils.hpp"
#include <crow/app.h>
#include <crow/common.h>
#include <crow/multipart.h>

using TypeParam = double;

constexpr uint16_t PORT = 443;

auto main(int argc, char *argv[]) -> int {

  crow::SimpleApp app;

  CROW_ROUTE(app, "/").methods("POST"_method)([](const crow::request &req) {
    crow::response response;
    crow::json::wvalue response_body;

    response.write(response_body.dump());
    return response;
  });

  CROW_ROUTE(app, "/health")([]() { return "Hello world"; });

  app.port(PORT).multithreaded().run();
}
