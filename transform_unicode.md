how to transform unicode

# main.cpp #

```
#include <brig/unicode/transform.hpp>
#include <brig/unicode/upper_case.hpp>
#include <iostream>
#include <locale>
#include <string>

int main(int, char**)
{
  using namespace brig::unicode;
  using namespace std;
  char16_t u16[] = { 0x68, 0x65, 0x6c, 0x6c, 0x6f, 0x20, 0x43f, 0x440, 0x438, 0x432, 0x435, 0x442, 0 }; // UTF-16
  string u8(transform<char>(u16)); // UTF-8
  wstring w(transform<wchar_t>(u8, upper_case)); // UTF-16 or UTF-32
  //locale::global(locale("rus"));
  wcout << w << endl;
  return 0;
}
```