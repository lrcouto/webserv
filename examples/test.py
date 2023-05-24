#!/usr/bin/env python

import os

method = os.environ.get('REQUEST_METHOD')
query = os.environ.get('QUERY_STRING')
con_type = os.environ.get('CONTENT_TYPE')
length = os.environ.get('CONTENT_LENGTH')
cookie = os.environ.get('HTTP_COOKIE')
agent = os.environ.get('HTTP_USER_AGENT')

print("<html>")
print("<head>")
print("<title>Request Details</title>")
print("</head>")
print("<body>")
print("<h1>Request Details in Python</h1>")
print("<pre>{}</pre>".format(method))
print("<pre>{}</pre>".format(query))
print("<pre>{}</pre>".format(con_type))
print("<pre>{}</pre>".format(length))
print("<pre>{}</pre>".format(cookie))
print("<pre>{}</pre>".format(agent))
print("</body>")
print("</html>")