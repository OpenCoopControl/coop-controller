#ifndef VERSION_H
#define VERSION_H

#define FIRMWARE_VERSION "0.1.10"
#define FIRMWARE_NAME "OpenCoopControl"
#define FIRMWARE_AUTHOR "OpenCoopControl Team"

#ifndef BUILD_TIMESTAMP
#define BUILD_TIMESTAMP "Development Build"
#endif

#define VERSION_STRING "const FIRMWARE_VERSION = \"" FIRMWARE_VERSION "\";\n" \
                      "const BUILD_TIMESTAMP = \"" BUILD_TIMESTAMP "\";\n" \
                      "const FIRMWARE_NAME = \"" FIRMWARE_NAME "\";"

#endif