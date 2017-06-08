build steps:
1) mkdir out  - create cmake output directory
2) cd out  - go to this directory
3) cmake ../module1  - execute cmake to create project makefiles
4) make  - build project
5) make install DESTDIR=${PWD}/out  - install build output under out directory 

post build steps
6) export LD_LIBRARY_PATH=out/usr/local/lib/  - export dynamic libraries from project build output
7) ./out/usr/local/bin/timeKeeper_test1  - execute tests for timeKeeper
8) ./out/usr/local/bin/smsSender_test1  - execute tests for smsSender
9) ./out/usr/local/bin/smsPlanner_test1  - exeute tests for smsPlanner
10) lcov --capture --directory $PWD --output-file coverage.info  - generate code coverage output file
11) genhtml coverage.info --output-directory outhtml  - convert code coverage output file to html file
12) firefox outhtml/index.html  - open html code coverage output file in web browser

http://cppunit.sourceforge.net/doc/cvs/money_example.html
http://cppunit.sourceforge.net/doc/cvs/cppunit_cookbook.html
