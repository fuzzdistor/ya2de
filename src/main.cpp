#include <SFML/Audio/Music.hpp>
#include <SFML/Audio/Sound.hpp>
#include <SFML/Audio/SoundBuffer.hpp>
#include <SFML/System/Sleep.hpp>
#include <SFML/System/Time.hpp>
#include <sol/sol.hpp>

#include <init.hpp>
#include <game.hpp>

#include <exception>
#include <iostream>
#include <string>


int main()
{
    sf::Clock appClock;
    Init::loggerInit();

    Log::Logger log("Application");
    log.info() << "App Start";

    try
    {
        Game game;
        game.run();
    }
    catch (std::exception& e)
    {
        log.critic() << e.what();
    }
    log.info() << "App Exit";
    log.info() << "Execution time: " << appClock.restart().asSeconds() << " seconds";
    return 0;
}

