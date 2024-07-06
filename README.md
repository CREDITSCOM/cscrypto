<img src="https://raw.githubusercontent.com/CREDITSCOM/Documentation/master/Src/Logo_Credits_horizontal_black.png" align="center">

[Documentation](https://developers.credits.com/en/Articles/Platform) \|
[Guides](https://developers.credits.com/en/Articles/Guides) \|
[News](https://credits.com/en/Home/News)

[![Twitter](https://img.shields.io/twitter/follow/creditscom.svg?label=Follow&style=social)](https://twitter.com/intent/follow?screen_name=creditscom)
[![AGPL License](https://img.shields.io/github/license/CREDITSCOM/node.svg?color=green&style=plastic)](LICENSE)

# CSCRYPTO
Cscrypto is credits company's crypto library supporting:
 - digital signatures
 - hashing and HMAC
 - random data generation
 - memory protection
 - private data ciphering
 - deterministic keys derivation
 - keys recovery from mnemonic phrase
 - key exchange

## Building from source

### Basic requirements :
 - cmake 3.10 or newer
 - compiler with C++17 support
 
To build with tests add the following argument when run cmake:
```sh
 -Dbuild_cscrypto_tests=ON
```

### Windows:
```sh
 git clone https://github.com/CREDITSCOM/cscrypto.git
 cd cscrypto
 mkdir build
 cd build
 cmake -DCMAKE_BUILD_TYPE=Release -A x64 ..
 cmake --build . --target ALL_BUILD --config Release
```
If Libsodium issues arise please try to open MSVS project and build for x64 located : cscrypto\third-party\libsodium\builds\msvc 
### Unix:
```sh
 $ git clone https://github.com/CREDITSCOM/cscrypto.git
 $ cd cscrypto
 $ mkdir build && cd build
 $ cmake -DCMAKE_BUILD_TYPE=Release ..
 $ make
```

#### Ubuntu & Debian

Build requirements:
```sh
 $ sudo apt-get install build-essential libtool autoconf
```

To build with gui:
```sh
 $ sudo apt-get install libqt5gui5 libqt5core5a libqt5dbus5 qttools5-dev qttools5-dev-tools libprotobuf-dev protobuf-compiler
```

#### Fedora

Build requirements:
```sh
 $ sudo dnf install gcc-c++ libtool make autoconf
```

To build with gui:
```sh
 $ sudo dnf install qt5-qttools-devel qt5-qtbase-devel protobuf-devel
```

<h2>Contribution</h2>
<p>Thank you for considering to help out with the source code! We welcome contributions from anyone on the internet, and are grateful for even the smallest of fixes!
If you'd like to contribute to Cscrypto, please fork, fix, commit and send a pull request for the maintainers to review and merge into the main code base. If you wish to submit more complex changes though, please check up with the core devs first on our <a href="https://developers.credits.com/">Developers portal</a> and <a href="https://github.com/CREDITSCOM/Documentation/blob/master/Contribution.md"> Contribution file</a> to ensure those changes are in line with the general philosophy of the project and/or get some early feedback which can make both your efforts much lighter as well as our review and merge procedures quick and simple.
Please make sure your contributions adhere to our coding guidelines:</p>
<ul>
<li>Code must adhere to the <a href="https://github.com/CREDITSCOM/node/blob/master/codingstyle.md">Credits coding style</a></li>

<li>Pull requests need to be based on and opened against the master branch</li>
<li>Commit messages should be prefixed with the package(s) they modify</li>
</ul>
<h3>Resources</h3>

<a href="https://credits.com//">CREDITS Website</a>

<a href="https://github.com/CREDITSCOM/DOCUMENTATION">Documentation</a>

<a href="https://credits.com/Content/Docs/TechnicalWhitePaperCREDITSEng.pdf">Whitepaper</a>

<a href="https://credits.com/Content/Docs/TechnicalPaperENG.pdf">Technical paper</a>

<a href="https://developers.credits.com/">Developers portal</a>

<a href="http://forum.credits.com/">Credits forum</a>
<h3>Community links</h3>
   <a href="https://t.me/creditscom"><img src ="https://simpleicons.org/icons/telegram.svg" height=40 widht=40 ></a>
   <a href="https://twitter.com/creditscom"><img src ="https://simpleicons.org/icons/twitter.svg" height=40 widht=40 ></a>
   <a href="https://www.reddit.com/r/CreditsOfficial/"><img src ="https://simpleicons.org/icons/reddit.svg" height=40 widht=40></a> 
   <a href="https://medium.com/@credits"><img src="https://simpleicons.org/icons/medium.svg" height=40 widht=40></a>
   <a href="https://www.instagram.com/credits_com/"><img src="https://simpleicons.org/icons/facebook.svg" height=40 widht=40></a>
   <a href="https://www.facebook.com/creditscom"><img src="https://simpleicons.org/icons/instagram.svg" height=40 widht=40></a>
   <a href="https://www.youtube.com/channel/UC7kjX_jgauCqmf_a4fqLGOQ"><img src="https://simpleicons.org/icons/youtube.svg" height=40 widht=40></a>
