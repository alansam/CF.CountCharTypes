//
//  countchar.cpp
//  CF.CountCharTypes
//  Count characters matching character classifications as
//  identified by the isxxxx() functions in the <cctype> system header.
//
//  Created by Alan Sampson on 4/7/21.
//
//  MARK: - Reference
//  @see: https://en.cppreference.com/w/cpp/string/byte
//

#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
#include <string_view>
#include <vector>
#include <algorithm>
#include <numeric>
#include <utility>
#include <cctype>

using namespace std::literals::string_literals;
using namespace std::literals::string_view_literals;

#if (__cplusplus > 201707L)
#endif  /* (__cplusplus > 201703L) */

auto what_is_it(void) -> void;

//  MARK: - Implementation
//  ....+....!....+....!....+....!....+....!....+....!....+....!....+....!....+....!
//  ================================================================================
//  ....+....!....+....!....+....!....+....!....+....!....+....!....+....!....+....!
/*
 *  MARK: main()
 */
int main(int argc, const char * argv[]) {
  // insert code here...
  std::cout << "CF.CountCharTypes\n"sv;
  std::cout << "C++ Version: "s << __cplusplus << std::endl;

  what_is_it();

  return 0;
}


//  ....+....!....+....!....+....!....+....!....+....!....+....!....+....!....+....!
//  ================================================================================
//  ....+....!....+....!....+....!....+....!....+....!....+....!....+....!....+....!
/*
 *  MARK: what_is_it()
 */
auto what_is_it(void) -> void {
  std::cout << "In: " << __func__ << "\n" << std::endl;

  typedef int (* isfn)(int);  //  function signature

  //  table of character classification functions
  std::vector<std::pair<std::string_view const, isfn const>> const fv {
    std::make_pair("iscntrl"sv,  iscntrl),
    std::make_pair("isprint"sv,  isprint),
    std::make_pair("isspace"sv,  isspace),
    std::make_pair("isblank"sv,  isblank),
    std::make_pair("ispunct"sv,  ispunct),
    std::make_pair("isalnum"sv,  isalnum),
    std::make_pair("isalpha"sv,  isalpha),
    std::make_pair("islower"sv,  islower),
    std::make_pair("isupper"sv,  isupper),
    std::make_pair("isdigit"sv,  isdigit),
    std::make_pair("isxdigit"sv, isxdigit),
  };

  /*
   *  MARK: lambda - Count character classifications
   */
  auto isit = [](std::string_view str, isfn fn) {

#if (__cplusplus >= 201707L)
    //  TODO: Implement with std::transform_reduce()
    auto ct = std::transform_reduce(str.begin(), str.end(), 0,
                                    [](auto cv, auto nr) {
      return nr + cv; //  accumulate
    },
                                    [&](auto ch) {
      auto vv = fn(ch) == 0 ? 0 : 1;  //  call isxxxx() function
      return vv;
    });
#else
    //  TODO: Implement with std::transform() & std::reduce()
    std::vector<uint16_t> uv(str.size());
    std::transform(str.cbegin(), str.cend(), uv.begin(), [&](auto ch) {
      auto vv = fn(ch) == 0 ? 0 : 1;  //  call isxxxx() function
      return vv;
    });
    auto ct = std::reduce(uv.begin(), uv.end(), 0, std::plus<>());  // accumulate
#endif  /* (__cplusplus > 201703L) */

    return ct;
  };

  /*
   *  MARK: lambda - Collect classified characters together
   */
  auto found = [](std::string_view str, isfn fn) {
    std::ostringstream os;
    auto result = ""s;
    std::for_each(str.cbegin(), str.cend(), [&](auto ch) {
      if (fn(ch)) {
        if (fn == iscntrl) {
          os << "\\x"sv << std::setw(2) << std::setfill('0') << std::hex << (0xff & ch)
             << std::setfill(' ') << std::dec;
        }
        else {
          os << ch;
        }
      }
    });
    result = os.str();
    return result;
  };

  //  sample texts
  auto const s1 = "What hAve wE hEre?\t310-555-2500 \x07"s;
  auto constexpr s2 = "ChEaTiNg LoWlIfE!\t127.0.0.1"sv;

  //  select for each function
  for (auto const & [fname, fun] : fv) {
    std::cout << "Demonstrate "sv << fname << '\n';
    //  std::string
    auto ct1 = isit(s1, fun);
    std::cout << std::setw(4) << s1.size() << ' ' << std::quoted(s1) << '\n'
              << std::setw(10) << fname << ' '
              << std::setw(4) << ct1 << ' '
              << std::quoted(found(s1, fun))
              << std::endl;

    //  std::string_view
    auto ct2 = isit(s2, fun);
    std::cout << std::setw(4) << s2.size() << ' ' << std::quoted(s2) << '\n'
              << std::setw(10) << fname << ' '
              << std::setw(4) << ct2 << ' '
              << std::quoted(found(s2, fun))
              << '\n'
              << std::endl;
  }

  return;
}
