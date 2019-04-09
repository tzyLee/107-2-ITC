Compiler:
    g++ (Ubuntu 7.3.0-27ubuntu1~18.04) 7.3.0

Compile:
    make

    or

    g++ -std=c++11 cipher.cpp pow.h   -o cipher
    g++ -std=c++11 decipher.cpp pow.h   -o decipher
    g++ -std=c++11 find.cpp   -o find
    g++ -std=c++11 bonus.cpp   -o bonus

secret.txt format:
    (\n(NUMBER))+

    One number on each line, prepend with a newline(\n)
    For example:
-----------

1306212233
2714996403
3144189642
991881486
4136870374
757097515
1493617138
1272776218
3901345899
1622147817
192602148
-----------

I finished the bonus part.