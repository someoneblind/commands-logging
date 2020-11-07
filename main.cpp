#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <stdio.h>
#include <termios.h>
#include <unistd.h>

#define COMMAND_QUIT "quit"
#define SAVE_FILE_PATH "history.log"

#define KEY_UP 65
#define KEY_DOWN 66
#define KEY_RIGHT 67
#define KEY_LEFT 68

using namespace std;

int main()
{
    string command;
    string lastCommand;
    char symbol;

    vector<string> commands;
    unsigned int commandsIt = 0;

    ifstream iSaveFile;
    ofstream oSaveFile;

    struct termios oldt, newt;

    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);

    iSaveFile.open(SAVE_FILE_PATH, fstream::out);

    if (iSaveFile.is_open()) {

        string s;
        while (getline(iSaveFile, s)) {
            commands.push_back(s);
        }
        iSaveFile.close();

        commandsIt = static_cast<unsigned int>(commands.size());
    }

    while (1) {

        symbol = static_cast<char>(getchar());

        if (symbol == '\n') {

            if (command.size() > 0) {

                if (commandsIt < commands.size())
                    commands.erase(commands.begin() + commandsIt);

                command += '\n';
                commands.push_back(command.substr(0, command.size() - 1));
            }

            if (!command.find(COMMAND_QUIT)) {

                oSaveFile.open(SAVE_FILE_PATH);
                for (string command : commands)
                    oSaveFile.write(string(command + '\n').c_str(), static_cast<streamsize>(command.size() + 1));
                oSaveFile.close();

                command.clear();
                cout << symbol;
                break;
            }
            else {
                command.clear();
                cout << symbol;
            }

            commandsIt = static_cast<unsigned int>(commands.size());

        }
        else if ((symbol == '\b') || (symbol == 127)) {
            if (command.length() > 0) {
                command.pop_back();
                cout << '\b' << ' ' << '\b';
            }
        }
        else if (symbol == 0x1b) {
            symbol = static_cast<char>(getchar());

            if (symbol == 0x5b) {
                symbol = static_cast<char>(getchar());

                switch((symbol)) {
                case KEY_UP: {
                    if (commandsIt == commands.size())
                        lastCommand = command;
                    if (commandsIt > 0)
                        commandsIt--;
                    break;
                }
                case KEY_DOWN: {
                    if (commandsIt < commands.size())
                        commandsIt++;
                    break;
                }
                case KEY_RIGHT: {

                    break;
                }
                case KEY_LEFT: {

                    break;
                }
                default: {
                    break;
                }
                }

                if ((symbol == KEY_UP) || (symbol == KEY_DOWN)) {
                    for (unsigned int i = 0; i < command.size(); ++i)
                        cout << '\b' << ' ' << '\b';
                    if (commandsIt < commands.size()) {
                        command = commands[commandsIt];
                    }
                    else {
                        command.clear();
                        command = lastCommand;
                    }
                    cout << command;
                }

            }
        }
        else if (symbol >= 0x20) {
            command += symbol;
            cout << static_cast<char>(symbol);
        }
    }

    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    return 0;
}
