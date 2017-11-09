preconditions:
sudo apt-get install sendmail

post build steps
1) export LD_LIBRARY_PATH=out/usr/local/lib/  - export dynamic libraries from project build output
2) ./out/usr/local/bin/module2_client piotrek24061988@gmail.com piotrek24061988@gmail.com Topic Content - execute application which is object of tests
3) ./out/usr/local/bin/connectionChecker_test1  - execute tests for connectionChecker with googletest
4) ./out/usr/local/bin/mailDevice_test1  - execute tests for mailDevice with googletest
5) ./out/usr/local/bin/mailSender_test1 - execute tests for mailSender with googletest
6) ./out/usr/local/bin/mailPlanner_test1 - execute tests for mailPlanner with googletest
