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
        log.info() << "App Exit success";
    }
    catch (std::exception& e)
    {
        log.critic() << e.what();
        log.info() << "App Exit after exception";
    }

    float executionTime = appClock.restart().asSeconds();

    if (executionTime >= 60.f)
    {
        int minutes = static_cast<int>(executionTime) / 60;
        executionTime -= static_cast<float>(minutes)*60.f;
        log.info() << "Execution time: " << minutes << " min " << executionTime << " seconds";
    }
    else
    {
        log.info() << "Execution time: " << executionTime << " seconds";
    }

    return 0;
}

