/**
      This file is part of arduino-serial-interact.

    arduino-serial-interact is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    arduino-serial-interact is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with arduino-serial-interact.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <string.h>
#include "interact.h"

InteractionManager::InteractionManager(Stream &stream, const char *prompt, 
    bool isHuman, const char *modeSwitchCommand) : 
        stream(stream),
        prompt(prompt),
        isHuman(isHuman),
        modeSwitchCommand(modeSwitchCommand)
{
    bufferIndex = 0;
    unknownHandler = NULL;
    modeSwitchCommandLength = strlen(modeSwitchCommand);
    registeredCommandsCount = 0;   
}

void InteractionManager::init() {
    if(isHuman) {
        stream.print(prompt);
    } 
}

void InteractionManager::loop() {
    if (stream.available()) {
        acceptChar(stream.read());
    }
}

// Accepts a new char into the buffer and triggers appropriate action
void InteractionManager::acceptChar(char c) {
    switch(c) {
        case INTERACT_KEY_ENTER:
            buffer[bufferIndex] = '\0';
            bufferIndex = 0;
            if(isHuman) {
                stream.println("");
            }

            parseCommand();

            // Echo prompt in human mode
            if(isHuman) {
                stream.println("");
                stream.print(prompt);
            }
            break;
        case INTERACT_KEY_BACKSPACE:
            if(bufferIndex > 0) {
                bufferIndex--;
                // Echo backspace sequence in human mode
                if(isHuman) {
                    stream.write(INTERACT_KEY_BACKSPACE);
                    stream.write(INTERACT_KEY_SPACE);
                    stream.write(INTERACT_KEY_BACKSPACE);
                }
            }
            break;
        default:
            if (c > 31 && c < 127 && bufferIndex <= INTERACT_BUFFER_SIZE) {
                buffer[bufferIndex] = c;
                bufferIndex ++;

                // Echo character in human mode
                if(isHuman) {
                    stream.write(c);
                }
            }
            break;
    }
}

void InteractionManager::parseCommand() {
    // See if it's the mode switching command
    if(strncmp(buffer, modeSwitchCommand, modeSwitchCommandLength) == 0) {
        isHuman ^= true;
        stream.print(F("Setting human mode to: "));
        stream.println(isHuman);
        return;
    }

    // Try to find matching command
    byte i;
    for(i = 0; i < registeredCommandsCount; i++) {
        if(strncmp(buffer, commands[i].command, commands[i].commandLength) == 0) {
            // Found the command
            commands[i].handler(buffer + commands[i].commandLength + 1);
            break;
        }
    }

    if(i == registeredCommandsCount && unknownHandler) {
        unknownHandler(buffer);
    }
}

void InteractionManager::registerCommand(const char *command, itr_handler handler) {
    if(registeredCommandsCount < INTERACT_MAX_COMAND_COUNT) {
        commands[registeredCommandsCount].command = command;
        commands[registeredCommandsCount].handler = handler;
        commands[registeredCommandsCount].commandLength = strlen(command);
        registeredCommandsCount++;
    }
}

void InteractionManager::registerUnknownHandler(itr_handler handler) {
    unknownHandler = handler;
}
