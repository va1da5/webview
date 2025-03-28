//bin/echo; [ $(uname) = "Darwin" ] && FLAGS="-framework Webkit" || FLAGS="$(pkg-config --cflags --libs gtk+-3.0 webkit2gtk-4.0)" ; c++ "$0" $FLAGS -std=c++11 -g -o webview && ./webview ; exit
// +build ignore

#include "webview.h"

#include <iostream>

#ifdef _WIN32
int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
                     LPSTR lpCmdLine, int nCmdShow)
#else
int main()
#endif
{
  webview::webview w(true, nullptr);
  w.set_title("Example");
  w.set_size(480, 320, WEBVIEW_HINT_NONE);
  w.set_size(180, 120, WEBVIEW_HINT_MIN);
  w.bind("noop", [](const std::string &s) -> std::string {
    std::cout << s << std::endl;
    return s;
  });
  w.bind("add", [](const std::string &s) -> std::string {
    auto a = std::stoi(webview::detail::json_parse(s, "", 0));
    auto b = std::stoi(webview::detail::json_parse(s, "", 1));
    return std::to_string(a + b);
  });
  w.set_html(R"V0G0N(
    <!doctype html>
    <html>
      <body>hello</body>
      <script>
        window.onload = function() {
          document.body.innerText = `hello, ${navigator.userAgent}`;
          noop('hello').then(function(res) {
            console.log('noop res', res);
          });
          add(1, 2).then(function(res) {
            console.log('add res', res);
          });
        };
      </script>
    </html>
  )V0G0N");
  w.run();
  return 0;
}
