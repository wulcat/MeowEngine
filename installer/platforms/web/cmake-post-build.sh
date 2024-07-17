#!/bin/bash

echo "Building HTML files..."

# Copy our 'template.html' into the output folder as 'index.html'.
cp template.html ../../../builds/web/index.html

# Navigate into the output folder then start a simple server and open it.
pushd ../../../builds/web
#  http-server


#python3  -m http.server 1233 & open http://localhost:1233

# For mac (kill's the old server)
lsof -nti:8001 | xargs kill -9

python -m SimpleHTTPServer 8001 #& open http://localhost:8001
#sudo nc -lk -p 8001 -c 'nc localhost 8000 | sed "s|application/octet-stream|application/wasm|g"'
#popd


#  python -m SimpleHTTPServer 8000
#  sudo lsof -t -i:8000 | xargs kill -9
#  python -m SimpleHTTPServer 8000 &
#  sudo nc -lk -p 8001 -c 'nc localhost 8000 | sed "s|application/octet-stream|application/wasm|g"'
popd