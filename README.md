# :rocket: Nozzle

Scriptable launcher for Linux

**Obviously NOT ready for use**

## Build

```sh
sudo apt install qt5-qmake qt5-default
git clone https://github.com/Schlipak/nozzle.git && cd nozzle/
qmake && make
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

Nozzle can run any script (or program for that matter) as a backend. Scripts must behave as a REPL, waiting for user input and outputting the results to STDOUT. Input and output must be done on a single line. The script will get the typed text as an input, and must give properly formatted JSON as an output. Backend scripts must terminate when the input is EOF (aka. Ctrl-D)

Example of output

```json
{"backend":"myBackendName","version":"1.0.0","priority":2,"results":[{"name":"My Entry Name","exec":"some-command --with params","icon":"myEntryIcon","description":"The description of My Entry Name"}]}
```

Typically, if you copy the output and run it in JavaScript's `JSON.parse` method (after escaping quotes) and it works, it's all good.

### Backend properties

- [ ] `backend`: Name or ID identifying the backend script
- [ ] `version`: Version number, can be used to further identify the script
- [ ] `priority`: Integer > 0. Determines how high in the list the results of the script will be displayed. This is useful for scripts that output very specific data, that should be shown before general data (eg. results of a math expression before a more generic applications list)
- [x] `results`: A JSON array of the query results (see below)

### Entry properties planned to be used

- [x] `name`: Name of the entry to display. Can contain rich text, which means tags such as `<b>`, `<i>` or `<u>` will be replaced with the corresponding styling. Please refer to the [Qt documentation for a full list of rich text tags](http://doc.qt.io/qt-5/richtext-html-subset.html). Note that some of these tags will not work well with Nozzle and may break the UI.
- [x] `exec`: Command to run when the entry is selected. Optional if you only want to display data (eg. a backend that performs math calculations)
- [x] `icon`: An icon to display next to the entry. Can be an absolute path, or an icon name, in which case Nozzle will attempt to grab the icon from the system icon theme
- [x] `description`: A textual description of the entry to be displayed next to the name, to provide more information on the action. Can contain rich text.
- [ ] `terminal`: true|false, should exec in a terminal or as a detached process
