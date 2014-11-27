#pragma once

struct GameSettings;

void print_usage(char *programName);
GameSettings* parseConsoleParameters(int argc, char *argv[]);       //creates a settings-object according to the program parameters. returns nullptr if it fails. settings need to be deleted()