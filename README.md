# Tester In A Terminal

A simple tool written entirely in C++ that makes sure your code is behaving correctly.




## How to install/use?

It's simple; just download the binaries for your OS from the Releases tab, add them to the PATH variable, and you are good to go. For a tutorial on how to use this tool, scroll down a bit further.

> [!WARNING]
> On Linux and MacOS you are not able to make HTTPS requests.

## How to build from source?

<sup>It might take a while...</sup>




Each OS has different requirements for building this tool from source:<br>

Windows: MSYS2 and MinGW64<br>

Linux: build-essential and the source code of libcurl<br>

Mac OS: brew, GCC, make and the source code of libcurl<br>




#### Instructions for Windows:

1. Open the MinGW MSYS2 shell

2. Update and upgrade with `pacman -Syu && pacman -Su`

3. Install all required libs `pacman -S --noconfirm --needed mingw-w64-x86_64-toolchain mingw-w64-x86_64-curl mingw-w64-x86_64-openssl mingw-w64-x86_64-zlib mingw-w64-x86_64-libidn2 mingw-w64-x86_64-libssh2 mingw-w64-x86_64-libpsl mingw-w64-x86_64-brotli mingw-w64-x86_64-zstd mingw-w64-x86_64-nghttp2 mingw-w64-x86_64-nghttp3 mingw-w64-x86_64-ngtcp2 mingw-w64-x86_64-libiconv mingw-w64-x86_64-libunistring`

4. Get args required for compilation with `pkg-config --define-prefix --static --libs libcurl`

5. Compile main.cpp (add the output from the previous command to the end of this one)  `g++ main.cpp util.c -o tt.exe -static -DCURL_STATICLIB` ...

6. Compile setup.cpp `g++ setup.cpp -o tt-setup.exe`




If you've done everything correctly, there should only be some warnings, no errors, and an executable tt file in the working dir.




#### Instructions for Linux and MacOS:

> [!IMPORTANT]
> (MacOS) Install gcc and make through brew `brew install gcc make`




> [!IMPORTANT]
> (Linux) Install buildtools and make with `sudo apt install build-essential make`

1. Create a folder where the compiled lib will go (for example `mkdir ~/curl`)

2. Navigate to the folder containing the source code of libcurl (not the newly created one)

3. Configure libcurl for compilation (replace ~/curl with your dir) `./configure --prefix=~/curl --disable-shared --enable-static --without-ssl --without-zlib --disable-ftp --disable-file --disable-ldap --disable-ldaps --disable-rtsp --disable-dict --disable-telnet --disable-tftp --disable-pop3 --disable-imap --disable-smtp --disable-gopher --disable-smb --disable-unix-sockets --disable-verbose --disable-ntlm-wb --disable-tls-srp --disable-threaded-resolver --disable-libcurl-option --disable-alt-svc --disable-doh --disable-hsts --disable-http-proxy --disable-metalink --disable-mqtt --disable-proxy --disable-rtmp --disable-sspi --disable-websockets --disable-pthreads --disable-manual --disable-ipv6 --without-brotli --without-zstd --without-nghttp2 --without-libidn2 --without-libpsl --without-libssh2 --without-gnutls --without-librtmp --without-mbedtls --without-nss --without-openssl --without-winldap --without-wincrypt --without-schannel --without-ca-bundle --without-ca-path --disable-netrc`

4. Make `make`

5. Install `make install`

6. Go back to the TesterInATerminal source code dir

7. * Compile main.cpp for Linux (replace ~/curl with your dir) `g++ main.cpp util.c -o tt -L ~/curl/lib -I ~/curl/include -lcurl -DCURL_STATICLIB -static -static-libstdc++ -static-libgcc -std=c++17`

   * Compile main.cpp for MacOS (replace ~/curl with your dir) `g++ main.cpp util.c -o tt -L ~/curl/lib -I ~/curl/include -lcurl -DCURL_STATICLIB -std=c++17`

8. Compile setup.cpp `g++ setup.cpp -o tt-setup`




## Tutorial: How to get started with your first test

1. Download your respective platform's test executable (you can find them in the test-exes folder)

* Windows: test.exe

* Linux: test.out

* MacOS: test.app

2. Run tt-setup in a dir that contains the test executable.

### How the test executable works

When given two words, it prints them back to STDOUT. When the first word is file, it will write a file called test.txt with its contents being the second word.




3. Input that you want to set up 1 test and that the file tested is (test.exe/test.out/test.app)




    To move around in the form, you can press tab. To select and change the data inside of an input, press space. To press a button, press space twice.

4. Select the first multi-choice input and press space until STDOUT is highlighted.

5. In the second input, type the command to run (for Windows, it is test.exe, for Linux it is ./test.out and for MacOS it is ./test.app) (press space when the input is underlined to start typing).

6. Select the Done button and press space twice to continue.

7. At the multi-choice for STDOUT, you are free to choose either full or portion.

8. In the expected STDOUT box, type any two words separated by a space (ex. hello world)

9. In the input provided to STDIN box, type out the two words separated by a space you chose earlier and add an enter (ex. Input provided to STDIN [hello world




    ])

10. For reverse condition, press space until No is selected.

11. Now that you have finished with the setup, try to run tt. If it says that all tests were completed successfully, then everything went well. If not, try and run tt-setup again and go back to step 3.




Congratulations, you have made your first test! From now on, it should be relatively straightforward to make your own tests. If there is an input that you don't know what it does, you can look it up in the tt-setup inputs table.




## tt-setup inputs table

| Input text | What it does|
|------------|-------------|
| Output is read from       | What the source of the output is |
| Command to run            | The command used to run the executable for testing |
| Name/path of output file  | The name/path of the file produced by your program |
| Check for file content    | If the content of the file should be exactly the text in the Expected file contents input or if it should contain the text in the Expected file contents input|
| Expected file contents    | Expected content of the file |
| Input provided to STDIN   | Text that is served as input to prompts through STDIN |
| Reverse condition?        | If the condition should be reversed (e.g., file contents do not contain the word error) |
| URL | URL of the request  |
| Check for response content| If the content of the response should be exactly the text in the Expected response input or if it should contain the text in the Expected response input|
| Expected response         | Expected content of the HTTP response |
| Check for STDOUT content| If the content of STDOUT should be exactly the text in the Expected STDOUT output input or if it should contain the text in the Expected STDOUT output input|
| Expected STDOUT output | Output expected from STDOUT |
