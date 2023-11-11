////////////////////////////////////////////////////////////
//
// COMMANDPROCESSING.H
//
//      Header file for Command, CommandProcessor,
//      FileCommandProcessorAdapter classes
//
////////////////////////////////////////////////////////////
#ifndef A2_345_V2_COMMANDPROCESSING_H
#define A2_345_V2_COMMANDPROCESSING_H
////////////////////////////////////////////////////////////

// INCLUDES ////////////////////////////////////////////////
#include <string.h>
#include <iostream>
#include <vector>

// GLOBAL //////////////////////////////////////////////////
extern std::string G_GameState;                 // This is the gamestate currently
extern std::vector<std::string> T_GameState;    // Transition gamestate

// CLASSES /////////////////////////////////////////////////


// Command Class /////////////////////////////////
class Command
{
    private:
        std::string cmd;              // stores the command itself :  {loadmap, validatemap, addplayer, gamestart, replay, quit}
        std::string flags;            // stores "-console" or "-file" flags : { -file, -console }
        std::string fn;               // stores the filename, if one is passed (without <>) : "something.txt"
        std::string gamestate;        // stores gamestate the command was run successfully in : {start,maploaded,mapvalidated,playersadded,win}
        std::string effect;           // stores the effect of a given command after execution : { }

    public:
        Command();                                                                   // Default Consructor
        Command(std::string cmd, std::string flg, std::string fn, std::string gs);   // Constructor/Init by values
        Command(const Command& acmd);                                                // Copy Constructor
        Command& operator = (const Command& acmd);                                   // Assignment Operator
        friend std::ostream& operator<<(std::ostream& stream, const Command& acmd);  // Stream Operator

        std::string getCmd();
        std::string getFlags();
        std::string getFn();
        std::string getGs();
        std::string getEffect();
};
//////////////////////////////////////////////////


// CommandProcessor //////////////////////////////
class CommandProcessor
{
    private:
        std::vector<Command*> listCmd;              // Stores all valid commands in vector
        std::string readCommand();                  // read cmd as console string

    public:
        CommandProcessor();                                      // Default constructor
        CommandProcessor(const CommandProcessor& acmdP);         // Copy Constructor
        CommandProcessor& operator = (const CommandProcessor& acmdP);        // Assignment Operator
        friend std::ostream& operator<<(std::ostream& stream, const CommandProcessor& acmdP);          // Stream Operator

        void saveCommand();                 // saves a valid command as command object
        Command* getCommand();                                   // returns current command at front of cmdList

        void initGs();                                           // Initialize the game state
        bool updateGs(std::string cmd);                          // Updates GS if command is OK returns false otherwise

        void saveEffect(Command* cmd);                           // Saves an effect in command obj
        bool validate(const std::string& inp);                   // Validates if command is valid

        std::string getOnlyCmd(std::string input);               // return only cmd part
        std::string getOnlyFlag(std::string input);              // return only flag
        std::string getOnlyFn(std::string input);                // return filename if there is one, "" if not.
        std::string getOnlyThing(std::string input);             // returns <playername> or <mapfile>
};
//////////////////////////////////////////////////


// FileCommandProcessorAdapter ///////////////////
class FileCommandProcessorAdapter : CommandProcessor
{
    private:
        std::vector<Command*> listC;                                          // listCmd just for FCPA class


    public:
        FileCommandProcessorAdapter();                                                                          // Default Constructor
        FileCommandProcessorAdapter(const FileCommandProcessorAdapter& aFCPA);                                  // Copy constructor
        FileCommandProcessorAdapter& operator = (const FileCommandProcessorAdapter& aFCPA);                     // Assignment Operator
        friend std::ostream& operator<<(std::ostream& stream, const FileCommandProcessorAdapter& aFCPA);        // Stream operator

        void readCommand(std::string filename, std::string init_Gs);
        std::string next_updateGs(std::string cmd, std::string existingstate);
};
//////////////////////////////////////////////////


////////////////////////////////////////////////////////////
#endif //A2_345_V2_COMMANDPROCESSING_H
////////////////////////////////////////////////////////////
