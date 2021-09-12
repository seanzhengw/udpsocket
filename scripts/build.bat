cmake ..
msbuild /property:Configuration=Release udpsocket.sln /t:udpsocket;client;server /m
