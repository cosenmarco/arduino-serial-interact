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

#ifndef __INTERACT_H__
#define __INTERACT_H__

#include <inttypes.h>
#include <string.h>

#include "Arduino.h"

#define INTERACT_KEY_BACKSPACE 8
#define INTERACT_KEY_ENTER 13
#define INTERACT_KEY_SPACE 32

#define INTERACT_BUFFER_SIZE 40  // Not larger than 255
#define INTERACT_MAX_COMAND_COUNT 10 // Not larger than 255

typedef void (*itr_handler)(char *);

/**
 * Command definition
 */
struct Command {
    const char *command;
    itr_handler handler;
    size_t commandLength;
};

class InteractionManager
{

  public:
    /**
     * Construct a new InteractionManager instance
     *
     * @param stream Stream to use
     * @param prompt Command line prompt (e.g. "> ") used only in human mode
     * @param isHuman Set to true if you want to start in human mode
     * @param modeSwitchCommand Text for the command that toggles between human and machine mode
     */
    InteractionManager(Stream &stream, const char *prompt, bool isHuman, 
        const char * modeSwitchCommand);

    /**
     * This should be called after the tream is initialized
     */
    void init(void);

    /**
     * Read the stream and evaluate commands.
     */
    void loop(void);

    /**
     * Add a new command.
     *
     * @param The command as it should be interpreted
     * @param handler The function called to handle the command
     */
    void registerCommand(const char * command, itr_handler handler);

    /**
     * This handler can be provided to handle a command that couldn't be matched
     */
    void registerUnknownHandler(itr_handler handler);

  private:
    Stream &stream;
    const char *prompt;
    bool isHuman;
    const char *modeSwitchCommand;
    byte modeSwitchCommandLength;
    Command commands[INTERACT_MAX_COMAND_COUNT];
    byte registeredCommandsCount;

    itr_handler unknownHandler;

    size_t bufferIndex;
    char buffer[INTERACT_BUFFER_SIZE + 1];

    void acceptChar(char c);
    void parseCommand(void);
};

#endif
