# Arduino Serial Interact

This is a minimalistic library for Arduino that offers the ability to interact using the serial link.

There are two modes of interacion: machine (default) and human. In machine mode there is no echo of the characters received from the serial link.

The handlers for the various commands will receive a C-like string (char *) which contains what was typed after the command (to use as arguments).

Before using you may want to tune the following constants in "interact.h":
```
#define INTERACT_BUFFER_SIZE 40  // Not larger than 255
#define INTERACT_MAX_COMAND_COUNT 10 // Not larger than 255
```

# Example

A sketch might look like this:

```cpp
// We want to use the Arduino hardware serial, 
// with prompt "> ", we want to start in human mode
// and the command to switch modes is "debug"
InteractionManager interaction(Serial, "> ", true, "debug");

void setup() {
    // initialize serial:
    Serial.begin(1000000);
    
    // Pre-defined commands
    interaction.registerCommand("reset", &handleReset);
    interaction.registerCommand("query", &handleQuery);
    interaction.registerCommand("cmd", &handleCommand);
    interaction.init();
}

void loop()
{
    interaction.loop();
}

void handleReset(char *buffer)
{
    Serial.println(F("RESET invoked"));
}
```

Example interaction from the previous sketch:

```
$ picocom -b 1000000 /dev/ttyUSB0
picocom v3.1

port is        : /dev/ttyUSB0
flowcontrol    : none
baudrate is    : 1000000
parity is      : none
databits are   : 8
stopbits are   : 1
escape is      : C-a
local echo is  : no
noinit is      : no
noreset is     : no
hangup is      : no
nolock is      : no
send_cmd is    : sz -vv
receive_cmd is : rz -vv -E
imap is        : 
omap is        : 
emap is        : crcrlf,delbs,
logfile is     : none
initstring     : none
exit_after is  : not set
exit is        : no

Type [C-a] [C-h] to see available commands
Terminal ready
> reset

RESET invoked

> debug
Setting human mode to: 0

```

# License

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
