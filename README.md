# Yakuake sessions

This small program adds session support for yakuake. This allows you to save some annoying type for changing needed directory, splitting terminal, activating virtual environment etc. Under the hood it utilizes d-bus interface. 


## Installation

Before building you need to make sure that qt-devel or qt-dev package installed. Fedora users can innstall that package issuing

`# yum install qt-devel`

in command prompt. Ubuntu users can run

`$ sudo apt-get install libqt4-dev`

### Building

Clone the project:

`$ git clone git://github.com/UfoDriver/yakuake-sessions.git`

Create makefile:

`$ qmake-qt4`

Build program:

`$ make`

### Post building

Copy `yakuake-sessions` binary to any place that's in your PATH. Then run:

`$ yakuake-sessions`