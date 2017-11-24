initial release 24.11.2017

preconditions:
1) Linux OS
2) C++ compiler installed with C++11 support
3) Cmake 3.x or higher installed
4) Autotools installed
5) Git installed

download steps:
1) git clone https://github.com/piotrek24061988/AutomaticTesting.git
2) cd AutomaticTesting
3) git submodule update --init --recursive

configuration steps:
1) to build unit tests, just build
2) to build integration tests, enable AutomaticTesting/module1/CMakeLists.txt IntegrationTests flag and then build 

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
3) ./out/usr/local/bin/smsDevice_test1  - execute tests for smsDevice with googletest or
   ./out/usr/local/bin/smsDevice_test2  - execute tests for smsDevice with cppunit
4) ./out/usr/local/bin/smsSender_test1  - execute tests for smsSender with googletest or
   ./out/usr/local/bin/smsSender_test2  - execute tests for smsSender with cppunit
5) ./out/usr/local/bin/smsPlanner_test1  - exeute tests for smsPlanner with googletest or
   ./out/usr/local/bin/smsPlanner_test2  - exeute tests for smsPlanner with cppunit
6) lcov --rc lcov_branch_coverage=1 --capture --directory $PWD --output-file coverage.info  - generate code coverage output file
7) genhtml --rc lcov_branch_coverage=1 coverage.info --output-directory outhtml  - convert code coverage output file to html file
8) firefox outhtml/index.html  - open html code coverage output file in web browser
