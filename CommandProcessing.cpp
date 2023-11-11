///////////////////////////////////////////////////////////////
//
// CommandProcessing.cpp
//      implementation file for CommandProcessing.h
//
///////////////////////////////////////////////////////////////

// INCLUDES ///////////////////////////////////////////////////
#include "CommandProcessing.h"
#include <string.h>
#include <cstring>
#include <fstream>

// GLOBAL VARIABLES ///////////////////////////////////////////
std::string G_GameState;
std::vector<std::string> T_GameState;

// COMMAND CLASS //////////////////////////////////////////////

// Default Constructor
Command::Command()
{

};

// Copy Constructor
Command::Command(const Command& acmd) : cmd(acmd.cmd), flags(acmd.flags), fn(acmd.fn), gamestate(acmd.gamestate), effect(acmd.effect)
{

}

// Copy Constructor
Command::Command(std::string cmd, std::string flg, std::string fn, std::string gs)
{
        this->cmd = cmd;
        this->flags = flg;
        this->fn = fn;
        this->gamestate = gs;

    if (cmd=="loadmap") { this->effect ="load the map file"; }
    if (cmd=="validatemap") { this->effect ="validate the map file"; }

    if (cmd=="addplayer") { this->effect ="add a player"; }

    if (cmd=="gamestart") { this->effect ="starts the game"; }
    if (cmd=="quit") { this->effect ="quit the game"; }
    if (cmd=="loadcmd") { this->effect ="load a list of valid commands from file"; }
    if (cmd=="runcmd") { this->effect ="runs commands from list in FCPA returns to console in resulting state"; }
    if (cmd=="changestate") { this->effect ="manually change the state to test code"; }
}

// Assignment Operator
Command& Command::operator=(const Command& acmd)
{
    if (this==&acmd)
    {
        return *this;
    }
    cmd = acmd.cmd;
    flags = acmd.flags;
    fn = acmd.fn;
    gamestate = acmd.gamestate;
    effect = acmd.effect;
    return *this;
};

// Stream Operator
std::ostream& operator<<(std::ostream& stream, const Command& acmd)
{
    stream << "| command = " << acmd.cmd << " | flag = " << acmd.flags << " | filename = " << acmd.fn << " | gamestate = " << acmd.gamestate << " | effect = " << acmd.effect;
    return stream;
}

//std::string Command::getCmd() {
//    return std::string();
//}

// Getters
std::string Command::getCmd() { return cmd; };
std::string Command::getFlags() { return flags; };
std::string Command::getFn()  { return fn; };
std::string Command::getGs()  { return gamestate; };
std::string Command::getEffect() { return effect; };

///////////////////////////////////////////////////////////////



// COMMANDPROCESSING CLASS ////////////////////////////////////

// Default constructor
CommandProcessor::CommandProcessor()
{
}

// Copy Constructor
CommandProcessor::CommandProcessor(const CommandProcessor& acmdP)
{
    // this->listCmd = new std::vector<Command*>(acmdP->listCmd);

}

// Assignment Operator
CommandProcessor& CommandProcessor::operator=(const CommandProcessor& acmdP)
{
    if (this == &acmdP)
    {
        return *this;
    }
    listCmd = acmdP.listCmd;    // Copy the list of acmdP into listCmd of this object
    return *this;
}

// Stream Operator : CHECK THIS
std::ostream& operator<<(std::ostream& stream, const CommandProcessor& acmdP)
{
    stream << "List of valid commands : ";
    for (auto elm : acmdP.listCmd)
    {
        stream << elm << " ";
    }
    return stream;
}



// Returns command only if input contains at the start
// one of {quit,loadmap,validatemap,gamestart,replay,loadcmd}
// valid commands, and "error" otherwise.
std::string CommandProcessor::getOnlyCmd(std::string input)
{
    // Check if commands {quit,loadmap,validatemap,addplayer,gamestart,replay}
    // exist at position 0. If not, error.
    //std::cout<<"Input passed is : "<<input<<std::endl;
    if (input.find("quit",0)!=std::string::npos)
    {
        return "quit";
    };
    if (input.find("loadmap",0)!=std::string::npos) { return "loadmap"; };
    if (input.find("validatemap",0)!=std::string::npos) { return "validatemap"; };
    if (input.find("addplayer",0)!=std::string::npos) { return "addplayer"; };
    if (input.find("gamestart",0)!=std::string::npos) { return "gamestart"; };
    if (input.find("replay",0)!=std::string::npos) { return "replay"; };

    if (input.find("loadcmd",0)!=std::string::npos) { return "loadcmd"; };
    if (input.find("runcmd",0)!=std::string::npos) { return "runcmd"; }

    if (input.find("changestate",0)!=std::string::npos) { return "changestate"; };

    std::cout<<"cmd not found"<<std::endl;
    return "error";
}

// return only flag
std::string CommandProcessor::getOnlyFlag(std::string input)
{
    if (input.find("-file") != std::string::npos)
    {
        //std::cout<<"Loads -file"<<std::endl;
        return "-file";
    }
    if (input.find("-console") != std::string::npos)
    {
        return "-console";
    }

    //std::cout<<"flag not found"<<std::endl;

    return "error";
}

// return thing from <thing> in input
std::string CommandProcessor::getOnlyThing(std::string input)
{
    int sp = input.find("<");
    int i = sp;
    int ep = input.find(">");

    // Shimmy the indices
    sp+=1;
    ep=ep-1;

    // Adds some redundency that our commends are correct.
    // loadmap <thing>, return thing
    if ( (getOnlyCmd(input)=="loadmap") || (getOnlyCmd(input)=="addplayer") || (getOnlyCmd(input)=="loadcmd") ) {
        // // This code peels off <thing>
        if (sp != std::string::npos && ep != std::string::npos) {
            std::string t_fn = input.substr(sp, ep - sp + 1);
            // std::cout<<"Filename starts at i="<<sp<<" ends at j="<<ep<<" and has name "<<t_fn<<std::endl;
            return t_fn;
        }else
        {
            return "error";
        }
    }
    return "error";
}



// return filename if there is one, "" if not.
// here <filename> could also be <mapfile> or <playername>
std::string CommandProcessor::getOnlyFn(std::string input)
{
    std::string the_fn;

    // Ensure input has the -file flag
    if (input.find("-file") != std::string::npos)
    {
        int sp = input.find("<");
        int i = sp;
        int ep = input.find(">");

        // Ensure that the filename is wrapped in < and > remove these and return filename.txt
        if (sp!=std::string::npos && ep!=std::string::npos)
        {
            sp+=1;
            ep=ep-1;

            std::string t_fn = input.substr(sp, ep-sp+1);

            // std::cout<<"Filename starts at i="<<sp<<" ends at j="<<ep<<" and has name "<<t_fn<<std::endl;

            return t_fn;

            // Finally check if the last 4 values are .txt to make
            // sure its a valid text file.
        }
        std::cout<<"fn not found"<<std::endl;
    }
    if (input.find("-console") != std::string::npos)
    {
        return "";
    }
    return "error";
};

// Validates if command is valid
// we need to fix validate so that flags are optional
// and loadmap <mapfile>, addplayer <playername> are
// accounted for. The updating game state doesn't work either.
// quit should be a signular command. Just more conditions.
bool CommandProcessor::validate(const std::string& inp)
{
    // Valid commands :
    // loadmap <mapfile>
    // addplayer <playername>
    // validatemap
    // gamestart
    // replay
    // quit

    // AND, i've added
    // loadcmd
    // changestate

    // which has flags   loadcmd -file <filename>
    // and               loadcmd -console
    // does nothing, allows you to add by console
    // (command line) which we were doing anyway.

    // Check if the first word in inp is a valid command
    bool isvalidcmd = false;

    // Check if first word is a command.
    if (getOnlyCmd(inp)!="error")
    {
        //std::cout<<"First word is command : "<<getOnlyCmd(inp)<<std::endl;
        if (getOnlyCmd(inp)=="quit")
        {
            return true;
        }

        if (getOnlyCmd(inp)=="changestate")
        {
            return true;
        }

        if (getOnlyCmd(inp)=="loadcmd")
        {
            return true;
        }

        if (getOnlyCmd(inp)=="runcmd")
        {
            return true;
        }

        // These ones are just a command, so if its just this, return true.
        if (getOnlyCmd(inp)=="validatemap" || getOnlyCmd(inp)=="gamestart" || getOnlyCmd(inp)=="replay" || getOnlyCmd(inp)=="quit")
        {
            // Check if the GS is in a state permissible to allow command
            if (updateGs(getOnlyCmd(inp))==true)
            {
                return true;
            }
            return false;
        }

        if (getOnlyCmd(inp)=="loadmap" || getOnlyCmd(inp)=="addplayer")
        {
            // Check for <something>
            if ( (getOnlyThing(inp)!="error") && (updateGs(getOnlyCmd(inp))==true) )
            {
                return true;
            }
            return false;
        }

        // Only for loadfile do we not need to do a check, since the
        // contained commands will be validated using the earlier checks.
        if (getOnlyCmd(inp)=="loadfile")
        {
            // loadfile -console            // Do nothing, we're ALREADY IN the console
            // loadfile -file <filename>    // Load from file.
            if ( (getOnlyFlag(inp)!="error") && (getOnlyThing(inp)!="error") )
            {
                if (getOnlyFlag(inp)=="-console")
                {
                    return true;
                }
                if (getOnlyFlag(inp)=="-file")
                {
                    if (getOnlyThing(inp)!="error")
                    {
                        return true;
                    }
                }
            }
        }

    }
    return false;
}
/////////////////






// read cmd as console string, return the string without qualifying it.
std::string CommandProcessor::readCommand()
{
    std::string input;
    std::string updatestate;
    std::cout<<"WarZone ["<<G_GameState<<"] > ";
    getline(std::cin, input);

    // loadcmd -file <commands.txt>

    // This will read and run the files from <filename> sequentially, showing output
    // and landing in the desired state.

    // && (getOnlyFlag(input)=="-file")
    if ( (getOnlyCmd(input)=="loadcmd") && (getOnlyFlag(input)=="-file") )
    {
        std::string filename = getOnlyFn(input);
        //std::cout << "   This loads the file "<<filename<<" which contains commands and places them in listCmd vector IF VALID."<<std::endl;

        // Now we can supercede CommandProcessor using the
        // readCommand() in the FileCommandProcessorAdapter class
        // which will load commands line by line from the given file,
        // then validate them, and if they are valid it will add the
        // commands in FileCommandProcessorAdapters private vector<command*> listCmd
        // object. The command 'runlist' will now run these sequentially leaving
        // the game in the resulting state, and returning the command prompt
        // to the user.

        FileCommandProcessorAdapter* fCPA = new FileCommandProcessorAdapter();

        std::string currentS = G_GameState.c_str();

        fCPA->readCommand(filename, currentS);


        //FileCommandProcessorAdapter FCPA = new FileCommandProcessorAdapter()
    }

    // Lets add a command to manually change the state, since
    // progressing from assignorder to win needs to be manually
    // changed. Makes testing easier.
    if (input=="changestate")
    {
        std::cout<<"Change state to what? > ";
        getline(std::cin, updatestate);

        if (updatestate=="start") { G_GameState = "start"; }
        if (updatestate=="maploaded") { G_GameState = "maploaded"; }
        if (updatestate=="mapvalidated") { G_GameState = "mapvalidated"; }
        if (updatestate=="playersadded") { G_GameState = "playersadded"; }
        if (updatestate=="assignreinforcement") { G_GameState = "assignreinforement"; }
        if (updatestate=="win") { G_GameState = "win"; }
        if (updatestate=="exit program") { G_GameState = "exit program"; }
    }

    // You can only quit when you're ahead.
    if ( (input=="quit") && (G_GameState=="win") )
    {
        G_GameState="exit program";
        std::cout<<"Exiting Warzone. Goodbye."<<std::endl;
        exit(1);
    }


    //std::cout<<"the inputted line is : "<<input;
    return input;
}

// saves a valid command as command object in listCmd vector<command*> list
void CommandProcessor::saveCommand()
{
    // Save command will readCommand
    std::string input = readCommand();

    //std::cout<<"The complete line is : "<<input<<std::endl;

    // Then its a valid command. Make it a command
    if (validate(input)==true)
    {
        std::cout<<"OK. | State is now "<< G_GameState <<std::endl;
        // (cmd, flag, filename, current gamestate, effect)
        Command* valid_command = new Command(getOnlyCmd(input), getOnlyFlag(input), getOnlyFn(input), G_GameState);

        //std::cout<<getOnlyCmd(input)<<std::endl;
        //std::cout<<getOnlyFlag(input)<<std::endl;
        //std::cout<<getOnlyFn(input)<<std::endl;
        //std::cout<<G_GameState<<std::endl;
        //std::cout<<""<<std::endl;

        //return valid_command;
    }else
    {
        std::cout<< input << "not a valid command."<<std::endl;
    }
}

// returns current command at front of cmdList
Command* CommandProcessor::getCommand()
{
    if (!listCmd.empty()) {
        Command *next_cmd = listCmd.back();
        listCmd.pop_back();
        return next_cmd;
    };
    return new Command("error","error","error","error");
}

// Initialize the game state
void CommandProcessor::initGs()
{
    // Initialize T_GameState;
    T_GameState.push_back("start");
    T_GameState.push_back("maploaded");
    T_GameState.push_back("mapvalidated");
    T_GameState.push_back("playersadded");
    T_GameState.push_back("win");
    T_GameState.push_back("exit program");

    // Initialize the start
    G_GameState = "start";

    // Ceremonially pop the start at the start.
    //if (T_GameState.front()=="start")
    //{
        //std::cout<<"POP GOES THE START."<<std::endl;
     //   T_GameState.pop_back();
    //}
    // Now the G_GameState = "maploaded"
    //std::cout<<"Initialize T_GameState "<< T_GameState.size() <<std::endl;
}

bool CommandProcessor::updateGs(std::string cmd)
{
    if (G_GameState=="exit program")
    {
        std::cout<<"EXITING PROGRAM."<<std::endl;
        exit(1);
    }

    if (cmd=="loadcmd" || cmd=="runcmd")
    {
        return true;
    }


    if (G_GameState=="start" && cmd=="loadmap") { G_GameState="maploaded";  return true; }
    if (G_GameState=="maploaded" && cmd=="loadmap") { G_GameState="maploaded"; return true; }

    if (G_GameState=="maploaded" && cmd=="validatemap") { G_GameState="mapvalidated";  return true; }

    if (G_GameState=="mapvalidated" && cmd=="addplayer") { G_GameState="playersadded"; return true; }
    if (G_GameState=="playersadded" && cmd=="addplayer") { G_GameState="playersadded"; return true; }

    if (G_GameState=="playersadded" && cmd=="gamestart") { G_GameState = "assignreinforcement"; return true; }

    if (G_GameState=="win" && cmd=="replay") { G_GameState = "start"; return true; }

    if (G_GameState=="win" && cmd=="quit")
    {
        G_GameState ="exit program";
        return true;
    }

    return false;
}
///////////////////////////////////////////////////////////////



// FILECOMMANDPROCESSINGADAPTER CLASS /////////////////////////
FileCommandProcessorAdapter::FileCommandProcessorAdapter()                                                                          // Default Constructor
{

}

FileCommandProcessorAdapter::FileCommandProcessorAdapter(const FileCommandProcessorAdapter& aFCPA)                                  // Copy constructor
{

}

FileCommandProcessorAdapter& FileCommandProcessorAdapter::operator = (const FileCommandProcessorAdapter& aFCPA)                     // Assignment Operator
{
    if (this == &aFCPA)
    {
        return *this;
    }
    listC = aFCPA.listC;    // Copy the list of acmdP into listCmd of this object
    return *this;
}

std::ostream& operator<<(std::ostream& stream, const FileCommandProcessorAdapter& aFCPA)
{
    stream << "| command = " ;
    return stream;
}

// Outputs
std::string FileCommandProcessorAdapter::next_updateGs(std::string cmd, std::string existingstate)
{
    if (existingstate=="start" && cmd=="loadmap") { return "maploaded";  }

    if (existingstate=="maploaded" && cmd=="loadmap") { return "maploaded";  }
    if (existingstate=="maploaded" && cmd=="validatemap") { return "mapvalidated";  }

    if (existingstate=="mapvalidated" && cmd=="addplayer") { return "playersadded";  }
    if (existingstate=="playersadded" && cmd=="addplayer") { return "playersadded";  }

    if (existingstate=="win" && cmd=="replay") { return "start";  }
    if (existingstate=="win" && cmd=="quit") { return "exit program";  }

    return "error";
}

void FileCommandProcessorAdapter::readCommand(std::string filename, std::string init_Gs)
{
    // This will read all the files, line by line into the listCmd vector
    // ONLY IF THEY ARE VALID. Note that their validity depends on G_GameState
    // too, which only changes if the commands are actually executed.
    // Check G_GameState for the first read command. Then, for subsequent commands,
    // a presupposed P_GameState becomes the measure for validity.

    std::string line;
    std::string full_fn = "C:\\Users\\CHARLIE\\CLionProjects\\A2_345_v2\\"+filename;
    std::ifstream file(full_fn);

    std::string gSchange(init_Gs);

    if (file.is_open())
    {
        std::cout<<" file loaded "<<std::endl;
        while (getline(file, line))
        {
            std::string projected_next_state = next_updateGs(getOnlyCmd(line), gSchange);

            std::cout<<"   line :  "<< line << " projected state :  "<< projected_next_state<<"  current state : "<< gSchange <<std::endl;

            if ( (validate(line)==true) && projected_next_state!="error")
            {
                // Command is valid. Create a command and push to FCPA's own cmdline vector.
                Command* valid_command = new Command(getOnlyCmd(line), getOnlyFlag(line), getOnlyThing(line), gSchange);
                listC.push_back(valid_command);
                //std::cout<<"New valid command created for "<<line<<" and saved to listCmd "<<std::endl;

                // We need to update the state locally since this isn't changing G_GameState, but rather
                // it WILL change the gamestate when its run. We compare first to current state such that
                // it can be run in order.
                gSchange = projected_next_state;
            }
        }
        file.close();
    }else
    {
        std::cout<<" Error : Unable to open "<<filename<<"."<<std::endl;
    }
}
///////////////////////////////////////////////////////////////