/*

Copyright (c) 2014 Jarryd Beck

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.

*/

#include <iostream>

#include "cxxopts.hpp"

cxxopts::ParseResult parse(int argc, char *argv[]) {
  try {
    cxxopts::Options options(argv[0], " - example command line options");
    options.positional_help("[optional args]").show_positional_help();

    bool apple = false;

    // clang-format off
    options
//      .allow_unrecognised_options()
      .add_options()
      ("a,apple", "an apple", cxxopts::value<bool>(apple))
      ("b,bob", "Bob")
      ("l, level", "level", cxxopts::value<int>())
      ("m,module", "module", cxxopts::value<int>())
      ("t,true", "True", cxxopts::value<bool>()->default_value("true"))
      ("f, file", "File", cxxopts::value<std::vector<std::string>>(), "FILE")
      ("i,input", "Input", cxxopts::value<std::string>())
      ("o,output", "Output file", cxxopts::value<std::string>()
          ->default_value("a.out")->implicit_value("b.def"), "BIN")
      ("positional",
        "Positional arguments: these are the arguments that are entered "
        "without an option", cxxopts::value<std::vector<std::string>>())
      ("long-description",
        "thisisareallylongwordthattakesupthewholelineandcannotbebrokenataspace")
      ("h,help", "Print help")
      ("int", "An integer", cxxopts::value<uint8_t>(), "model level")
      ("float", "A floating point number", cxxopts::value<float>())
      ("option_that_is_too_long_for_the_help", "A very long option")
    #ifdef CXXOPTS_USE_UNICODE
      ("unicode", u8"A help option with non-ascii: à. Here the size of the"
        " string should be correct")
    #endif
    ;

    options.add_options("Group")
      ("c,compile", "compile")
      ("d,drop", "drop", cxxopts::value<std::vector<std::string>>());

    // clang-format on

    //    options.parse_positional({"input", "output", "positional"});

    options.cmd("set", {"l", "m"}, "set log level to `l` for module `m`");
    options.cmd("reset", {"m"}, "rest log level to default for module `m`");
    options.cmd("list", {}, "print all log info`");

    auto result = options.parse(argc, argv);

    if (result.count("help")) {
      std::cout << options.help({"", "Group"}) << std::endl;
      exit(0);
    }

    if (apple) {
      std::cout << "Saw option ‘a’ " << result.count("a") << " times "
                << std::endl;
    }

    if (result.count("b")) {
      std::cout << "Saw option ‘b’" << std::endl;
    }

    if (result.count("f")) {
      auto &ff = result["f"].as<std::vector<std::string>>();
      std::cout << "Files" << std::endl;
      for (const auto &f : ff) {
        std::cout << f << std::endl;
      }
    }

    if (result.count("input")) {
      std::cout << "Input = " << result["input"].as<std::string>() << std::endl;
    }

    if (result.count("output")) {
      std::cout << "Output = " << result["output"].as<std::string>()
                << std::endl;
    }

    if (result.count("positional")) {
      std::cout << "Positional = {";
      auto &v = result["positional"].as<std::vector<std::string>>();
      for (const auto &s : v) {
        std::cout << s << ", ";
      }
      std::cout << "}" << std::endl;
    }

    if (result.count("level")) {
      std::cout << "level = " << result["level"].as<int>() << std::endl;
    }

    if (result.count("m")) {
      std::cout << "module = " << result["m"].as<int>() << std::endl;
    }

    if (result.count("int")) {

      auto value = result["int"].as<uint8_t>();
      (void)value;

      std::cout << "int = " << value << std::endl;
    }

    if (result.count("float")) {
      std::cout << "float = " << result["float"].as<float>() << std::endl;
    }

    std::cout << "arguments: " << std::endl;

    for (const auto &kv : result.arguments()) {
      std::cout << "key: " << kv.key() << " value:" << kv.value() << std::endl;
    }
    std::cout << "Arguments remain = " << argc << std::endl;

    return result;

  } catch (const cxxopts::OptionException &e) {
    std::cout << "error parsing options: " << e.what() << std::endl;
    exit(1);
  }
}

int main(int argc, char *argv[]) {
  auto result = parse(argc, argv);
  auto arguments = result.arguments();
  std::cout << "Saw " << arguments.size() << " arguments" << std::endl;

  return 0;
}
