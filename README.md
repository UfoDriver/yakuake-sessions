# Yakuake sessions

This small program adds session support for yakuake. This allows you to save some annoying type for changing needed directory, splitting terminal, activating virtual environment etc. Under the hood it utilizes d-bus interface. 


## Installation

Before building you need to make sure that qt5-base-devel or qt-dev package installed. Fedora users can innstall that package issuing

`# dnf install qt5-qtbase-devel`

in command prompt. Ubuntu users can run

`$ sudo apt-get install libqt4-dev`

(Not sure about Ubuntu, don't have it now).

### Building

Clone the project:

`$ git clone git://github.com/UfoDriver/yakuake-sessions.git`

Create makefile:

`$ qmake-qt5`

Build program:

`$ make`

### Post building

Copy `yakuake-sessions` binary to any place that's in your PATH. Then run:

`$ yakuake-sessions`
