#!/usr/bin/env python

import os

cookie_value = os.environ.get('HTTP_COOKIE')
name = ""

if cookie_value:
    cookies = cookie_value.split(';')
    for cookie in cookies:
        cookie_parts = cookie.split('=')
        if len(cookie_parts) == 2 and cookie_parts[0].strip() == 'name':
            name = cookie_parts[1]

print("Content-type: text/html")
print()
print("<!DOCTYPE html>")
print("<html>")
print("<head>")
print("  <meta charset='UTF-8'>")
print("  <title>Uma sessão só pra você</title>")
print("  <style>")
print("""\
* { overflow: hidden; margin: 0; }
body { background: #000; }
.content {
  max-width: 1000px;
  margin: 0 auto;
  text-align: justify;
  color: #fff;
  font-family: "Lucida Console", monospace;
  position: relative;
  z-index: 1;
  background-color: rgba(0, 0, 0, 0.8);
  padding: 20px;
  border-radius: 15px;
  box-shadow: 0 0 10px rgba(255, 255, 255, 0.2);
  position: absolute;
  top: 40%;
  left: 50%;
  transform: translate(-50%, -50%);
}
h1 { margin-bottom: 20px; }
p { line-height: 1.5; margin-bottom: 20px; }
.button-container { display: flex; justify-content: space-between; }
.button {
  display: inline-block;
  padding: 10px 20px;
  background-color: rgb(0, 104, 136);
  color: #fff;
  font-size: 20px;
  text-decoration: none;
  border-radius: 4px;
}
.button:hover { background-color: rgb(0, 186, 188); }
""")
print("  </style>")
print("</head>")
print("<body>")
print("  <div class='content'>")
print("    <h1>Uma sessão só pra você</h1>")
print("    <p>")
print("      Olá, {}!".format(name))
print("    </p>")
print("    <p>")
print("      Como você pode ver, agora nós podemos usar um valor retirado de um cookie para alterar")
print("      dinamicamente a informação apresentada em uma página.")
print("    </p>")
print("    <p>")
print("      Repare, no entanto, que você acabou de abrir um script em Python. Isso é porque nosso")
print("      Webserv suporta apenas páginas estáticas, e não funciona bem com o código em Javascript")
print("      que seria necessário para utilizar a informação contida em um cookie. Mas pra tudo se dá um jeitinho,")
print("      e nesse caso pudemos usar o nosso CGI para demonstrar essa funcionalidade!")
print("    </p>")
print("    <br>")
print("    <a href='http://localhost:3007/goodbye.html' class='button'>➡️➡️➡️</a>")
print("  </div>")
print("</body>")
print("</html>")
