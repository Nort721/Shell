# Shell
An interactive shell, supports two input methods, direct CLI-based input (when set in LOCAL mode) or network-based input (when set in REMOTE mode).

## Usage
Double-click shell.exe to launch in locale mode.
to launch in remote mode use winkey + r to execute the following command

```shell.exe <address>```

## Specifications

#### commands
* ls (opens the directory specified (relative or absolute) (or the current directory if none is provided) and then iterates through its entries, printing out each file or directory name to the standard output.)
* cd (updates the shell's current working directory to the specified path.)
* cat (opens the specified file (relative or absolute), reads its contents, and then prints the content to the standard output.)

### server
A server that provides remote control capability over the shell when it's set to REMOTE mode.
