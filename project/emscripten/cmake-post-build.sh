#!/bin/bash

# Copy our 'template.html' into the output folder as 'index.html'.
cp template.html out/index.html

# Navigate into the output folder then start a simple server and open it.
pushd out
#  http-server


#python3  -m http.server 1233 & open http://localhost:1233

lsof -nti:8001 | xargs kill -9
python -m SimpleHTTPServer 8001 & open http://localhost:8001

popd

