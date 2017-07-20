# :rocket: Nozzle

Scriptable launcher for Linux

**Obviously NOT ready for use**

## Build

```sh
sudo apt install qt4-qmake libqt4-dev
git clone https://github.com/Schlipak/nozzle.git && cd nozzle/
qmake nozzle.pro && make
./nozzle
```

## Configuration

Nozzle will read configuration from a file. As of today this file is not created automatically, but it eventually will. Typically on Linux systems it will be `$HOME/.config/Nozzle/NozzleLauncher.conf`.

Only a few options are available yet, but more will follow, including extensive styling capabilities.

```ini
[backend]
exec=python
params=/home/myself/myscript.py --some-option

[panel]
animation-speed=350
```

## Backend scripts

Nozzle can run any script (or program for that matter) as a backend. Scripts must behave as a REPL, waiting for user input and outputting the results to STDOUT. Input and output must be done on a single line. The script will get the typed text as an input, and must give properly formatted and escaped JSON as an output. Backend scripts must terminate when the user input is EOF (aka. Ctrl-D)

Example of output

```plain
[{\"name\":\"something\",\"exec\":\"my-binary --with param\"},{\"name\":\"other\",\"exec\":\"run --this \'with a string param\'\"}]
```

Typically, if you copy the output and run it in JavaScript's `JSON.parse` method and it works, it's all good.

Properties planned to be used:

* `name`: Name of the entry to display
* `exec`: Command to run when the entry is selected. Optional if you only want to display data (eg. a backend that performs math calculations)
* `icon`: An icon to display next to the entry. Can be an absolute path, or an icon name, in which case Nozzle will attempt to grab the icon from the system icon theme
* `description`: A textual description of the entry to be displayed next to the name, to provide more information on the action
* `terminal`: true|false, should exec in a terminal or as a detached process
