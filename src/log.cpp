#include <iostream>
#include <string>

#include "cxxopts.hpp"

using std::cout;
using std::endl;
using std::string;

using cxxopts::OptionAdder;
using cxxopts::Options;
using cxxopts::ParseResult;
using cxxopts::value;

string set_handler(const ParseResult &result) {
  cout << "got set command, level is " << result["l"].as<size_t>()
       << "module is " << result["module"].as<size_t>() << endl;

  return "";
}

string reset_handler(const ParseResult &result) {
  cout << "got reset command, module is " << result["module"].as<size_t>()
       << endl;
  return "";
}

string list_handler(const ParseResult &result) {
  (void)result;
  cout << "got list command" << endl;
  return "";
}

ParseResult parse(int argc, char *argv[]) {
  try {
    Options options(argv[0], " - log configuration");

    // first create a option adder
    OptionAdder op_adder = options.add_options();

    // add options
    op_adder("l, level", "level", value<size_t>());
    op_adder("m,module", "module", value<size_t>());
    op_adder("h,help", "Print help");

    // add command and specify required options if needed
    options.cmd("set", {"l", "m"}, "set log level to `l` for module `m`");
    options.cmd("reset", {"m"}, "rest log level to default for module `m`");
    options.cmd("list", {}, "print all log info");

    auto result = options.parse(argc, argv);

    if (result.count("help")) {
      std::cout << options.help() << std::endl;
      exit(0);
    }

    return result;

  } catch (const cxxopts::OptionException &e) {
    std::cout << "error parsing options: " << e.what() << std::endl;
    exit(1);
  }
}

int main(int argc, char *argv[]) {
  auto result = parse(argc, argv);

  if (result.GotCommand("set")) {
    cout << set_handler(result) << endl;
  }

  if (result.GotCommand("reset")) {
    cout << reset_handler(result) << endl;
  }

  if (result.GotCommand("list")) {
    cout << list_handler(result) << endl;
  }

  return 0;
}
