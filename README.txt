download steps:
1) git clone https://github.com/piotrek24061988/AutomaticTesting.git
2) cd AutomaticTesting
3) git submodule update --init --recursive

configuration steps:
1) to build integration tests, just build
2) to build unit tests, disable AutomaticTesting/module1/CMakeLists.txt IntegrationTests flag and then build 

build steps:
1) mkdir out  - create cmake output directory
2) cd out  - go to this directory
3) cmake ../module1  - execute cmake to create project makefiles
4) make DESTDIR=${PWD}/out - build project
5) make install DESTDIR=${PWD}/out  - install build output under out directory 

post build steps
1) export LD_LIBRARY_PATH=out/usr/local/lib/  - export dynamic libraries from project build output
2) ./out/usr/local/bin/timeKeeper_test1  - execute tests for timeKeeper with googletest or
   ./out/usr/local/bin/timeKeeper_test2  - execute tests for timeKeeper with cppunit
3) ./out/usr/local/bin/smsSender_test1  - execute tests for smsSender with googletest or
   ./out/usr/local/bin/smsSender_test2  - execute tests for smsSender with cppunit
4) ./out/usr/local/bin/smsPlanner_test1  - exeute tests for smsPlanner with googletest or
   ./out/usr/local/bin/smsPlanner_test2  - exeute tests for smsPlanner with cppunit
5) lcov --capture --directory $PWD --output-file coverage.info  - generate code coverage output file
6) genhtml coverage.info --output-directory outhtml  - convert code coverage output file to html file
7) firefox outhtml/index.html  - open html code coverage output file in web browser
