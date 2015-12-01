
#include "ctrl_network.h"
#include "ctrl_gamepad.h"
#include "ctrl_keyboard.h"

#include "metabot_control.h"

#include "minitraces.h"

#include <chrono>
#include <thread>

//#define LATENCY_TIMER
#define ENABLE_BOT 1
#define ENABLE_NET 1
#define ENABLE_KEY 1
#define ENABLE_PAD 1

void main_infos()
{
    TRACE_INFO(MAIN, "Metabot " BLD_WHITE "SERVER" CLR_RESET " launched!\n");
    TRACE_INFO(MAIN, "Built on '%s, %s'\n", __DATE__ , __TIME__);
    TRACE_INFO(MAIN, "\n");

#if ENABLE_BOT == 1
    TRACE_INFO(MAIN, "Metabot hardware is " BLD_GREEN "ENABLED" CLR_RESET "\n");
#else
    TRACE_INFO(MAIN, "Metabot hardware is " BLD_RED "DISABLED" CLR_RESET "\n");
#endif

#if ENABLE_NET == 1
    TRACE_INFO(MAIN, "Network control is " BLD_GREEN "ENABLED" CLR_RESET "\n");
#else
    TRACE_INFO(MAIN, "Network control is " BLD_RED "DISABLED" CLR_RESET "\n");
#endif

#if ENABLE_KEY == 1
    TRACE_INFO(MAIN, "Keyboard control is " BLD_GREEN "ENABLED" CLR_RESET "\n");
#else
    TRACE_INFO(MAIN, "Keyboard control is " BLD_RED "DISABLED" CLR_RESET "\n");
#endif

#if ENABLE_PAD == 1
    TRACE_INFO(MAIN, "Gamepad control is " BLD_GREEN "ENABLED" CLR_RESET "\n");
#else
    TRACE_INFO(MAIN, "Gamepad control is " BLD_RED "DISABLED" CLR_RESET "\n");
#endif

    TRACE_INFO(MAIN, "\n");
}

int main(int argc, char *argv[])
{
    main_infos();

    bool status = true;

    // Init network server
    ////////////////////////////////////////////////////////////////////////////

    networkControl *net = NULL;

#if ENABLE_NET == 1
    net = new networkControl();
    if (net)
    {
        if (net->setup() == true)
        {
            net->autodetect();
        }
        else
        {
            TRACE_ERROR(MAIN, "networkControl setup FATAL ERROR!\n");
            return EXIT_FAILURE;
        }
    }
    else
    {
        TRACE_ERROR(MAIN, "networkControl allocation FATAL ERROR!\n");
        return EXIT_FAILURE;
    }
#endif

    // Init KEYBOARD
    ////////////////////////////////////////////////////////////////////////////

    keyboardControl *key = NULL;

#if ENABLE_KEY == 1
    key = new keyboardControl();
    if (key)
    {
        key->autodetect();
    }
    else
    {
        TRACE_WARNING(MAIN, "keyboardControl allocation error!\n");
    }
#endif

    // Init GAMEPAD
    ////////////////////////////////////////////////////////////////////////////

    gamepadControl *pad = NULL;

#if ENABLE_PAD == 1
    pad = new gamepadControl();
    if (pad)
    {
        pad->autodetect();
    }
    else
    {
        TRACE_WARNING(MAIN, "gamepadControl allocation error!\n");
    }
#endif

    // Init Metabot & serial link
    ////////////////////////////////////////////////////////////////////////////

    Metabot *bot = NULL;

#if ENABLE_BOT == 1
    bot = new Metabot();
    if (bot)
    {
        if (bot->setup() == true)
        {
            TRACE_INFO(MAIN, ">> Metabot is connected!\n");
        }
        else
        {
            TRACE_ERROR(MAIN, "Metabot connection FATAL ERROR!\n");
            return EXIT_FAILURE;
        }
    }
    else
    {
        TRACE_ERROR(MAIN, "Metabot allocation FATAL ERROR!\n");
        return EXIT_FAILURE;
    }
#endif

    // Go for it MOTHAFUCKA
    ////////////////////////////////////////////////////////////////////////////

    MoveitMoveit move;
    double loopFrequency = 30.0;
    double loopDuration  = 1000.0 / loopFrequency;
    std::chrono::time_point<std::chrono::system_clock> start, end;

    while (true)
    {
        start = std::chrono::system_clock::now(); // Loop timer
        bool exit = false;

        // Metabot control
        {
            // Reset direct input, not states (walk, crab and inverted)
            move.dx = 0;
            move.dy = 0;
            move.turn = 0;
            move.height = 0;

            if (net)
            {
                // Read input from network client
                net->run(move, exit);
            }

            if (key)
            {
                // Read input from keyboard (overwrite network control)
                key->run(move, exit);
            }

            if (pad)
            {
                // Read input from gamepads (overwrite keayboar & network control)
                pad->run(move, exit);
            }

            if (exit)
            {
                break;
            }

            if (bot)
            {
                // Forward orders to the metabot
                bot->move(move);

                // Execute movments
                bot->run();
            }
        }

        // Loop timer
        end = std::chrono::system_clock::now();
        double loopd = std::chrono::duration_cast<std::chrono::microseconds>(end-start).count();
        double waitd = (loopDuration * 1000.0) - loopd;

#ifdef LATENCY_TIMER
        if ((loopd / 1000.0) > loopDuration)
            TRACE_WARNING(MAIN, "Control loop duration: %f ms of the %f ms budget.\n", (loopd / 1000.0), loopDuration);
        else
            TRACE_1(MAIN, "Control loop duration: %f ms of the %f ms budget.\n", (loopd / 1000.0), loopDuration);
#endif

        // Loop control
        if (waitd > 0.0)
        {
            std::chrono::microseconds waittime(static_cast<int>(waitd));
            std::this_thread::sleep_for(waittime);
        }
    }

    TRACE_INFO(MAIN, "MetaBotServer exiting normally!\n");
    return EXIT_SUCCESS;
}
