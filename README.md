# :rocket: Nozzle

<p align="center">
  <span>Scriptable launcher for Linux</span><br/><br/>
  <img src="http://imgur.com/Qm1SZD9.gif" alt="Nozzle">
</p>

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
[backends]                                 ; Nozzle uses an extension of the INI format for arrays
script\1\exec=ruby                         ; The backend program to be run
script\1\params=/path/to/some-script.rb    ; Script to be run or params to send to the program
script\1\name="SomeScript"                 ; Used for identification in case of backend error
script\2\exec=python                       ; Another backend...
script\2\params=/path/to/another-script.py
script\2\name="Another Script"
script\size=2                              ; Size is needed internally, don't forget to update it!

[env]                           ; Environment variables to pass to backend scripts
GEM_HOME="/path/to/gems_home"   ; Gem HOME and PATH can be required if using RVM
GEM_PATH="/path/to/gems/2.3.0"  ; Variables that already exist are REPLACED in the environment

[style]
background-color="#F2F2F8"                 ; Background color of the panel
selection-background-color="#B3E5FC"       ; Background color of the selected entry
text-color="#212121"                       ; Main text color
text-secondary-color="rgba(33,33,33,0.75)" ; Secondary text color (used for description)
width=900                                  ; Width of the panel
border-radius=5px                          ; Panel border radius
blur-radius=15                             ; Radius of the panel blur
animation-duration=350                     ; Duration of the panel animation (ms)
```

## Backend scripts

[Official backends repository](https://github.com/Schlipak/nozzle-backends)

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


### Entry properties

- [x] `name`: Name of the entry to display. Can contain rich text, which means tags such as `<b>`, `<i>` or `<u>` will be replaced with the corresponding styling. Please refer to the [Qt documentation for a full list of rich text tags](http://doc.qt.io/qt-5/richtext-html-subset.html). Note that some of these tags will not work well with Nozzle and may break the UI.
- [x] `exec`: Command to run when the entry is selected. Optional if you only want to display data (eg. a backend that performs math calculations)
- [x] `icon`: An icon to display next to the entry. Can be an absolute path, or an icon name, in which case Nozzle will attempt to grab the icon from the system icon theme
- [x] `description`: A textual description of the entry to be displayed next to the name, to provide more information on the action. Can contain rich text.
- [ ] `terminal`: true|false, should exec in a terminal or as a detached process
