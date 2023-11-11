////////////////////////////////////////////////////////////
//
//  CommandProcessingDriver.cpp
//
//
////////////////////////////////////////////////////////////

// INCLUDES ////////////////////////////////////////////////
#include "CommandProcessing.h"

// PROTOTYPE ///////////////////////////////////////////////

// Free function for testing all classes
void testCommandProcessor();

// MAIN ////////////////////////////////////////////////////
int main()
{
    testCommandProcessor();

    return 0;
}

// FUNCTION ////////////////////////////////////////////////
void testCommandProcessor()
{
    CommandProcessor *cp = new CommandProcessor();

    // Initialize the gamestate
    cp->initGs();

    //std::string checkcmd = "validatemap -file <abcdefgh.txt>";
    //std::string cmd = cp->getOnlyCmd(checkcmd);
    //std::string flg = cp->getOnlyFlag(checkcmd);
    //std::string fn = cp->getOnlyFn(checkcmd);

    cp->saveCommand();

    Command* a_command = cp->getCommand();

    while (a_command->getCmd()!="quit")
    {
        cp->saveCommand();
        Command* nextCmd = cp->getCommand();
        cp->updateGs(nextCmd->getCmd());
    }
    std::cout<<"GameState = "<<G_GameState<<std::endl;
}