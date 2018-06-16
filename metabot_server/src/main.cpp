
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
    TRACE_INFO(MAIN, "Metabot " BLD_WHITE "SERVER" CLR_RESET " launched!");
    TRACE_INFO(MAIN, "Built on '%s, %s'", __DATE__ , __TIME__);
    TRACE_INFO(MAIN, "");

#if ENABLE_BOT == 1
    TRACE_INFO(MAIN, "Metabot hardware is " BLD_GREEN "ENABLED" CLR_RESET "");
#else
    TRACE_INFO(MAIN, "Metabot hardware is " BLD_RED "DISABLED" CLR_RESET "");
#endif

#if ENABLE_NET == 1
    TRACE_INFO(MAIN, "Network control is " BLD_GREEN "ENABLED" CLR_RESET "");
#else
    TRACE_INFO(MAIN, "Network control is " BLD_RED "DISABLED" CLR_RESET "");
#endif

#if ENABLE_KEY == 1
    TRACE_INFO(MAIN, "Keyboard control is " BLD_GREEN "ENABLED" CLR_RESET "");
#else
    TRACE_INFO(MAIN, "Keyboard control is " BLD_RED "DISABLED" CLR_RESET "");
#endif

#if ENABLE_PAD == 1
    TRACE_INFO(MAIN, "Gamepad control is " BLD_GREEN "ENABLED" CLR_RESET "");
#else
    TRACE_INFO(MAIN, "Gamepad control is " BLD_RED "DISABLED" CLR_RESET "");
#endif

    TRACE_INFO(MAIN, "");
}

int main(int argc, char *argv[])
{
    main_infos();

    // Init network server
    ////////////////////////////////////////////////////////////////////////////

    networkControl *net = nullptr;

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
            delete net;
            net = nullptr;
            TRACE_ERROR(MAIN, "networkControl setup FATAL ERROR!");
        }
    }
    else
    {
        TRACE_ERROR(MAIN, "networkControl allocation FATAL ERROR!");
    }

    if (!net)
        return EXIT_FAILURE;
#endif // ENABLE_NET

    // Init KEYBOARD
    ////////////////////////////////////////////////////////////////////////////

    keyboardControl *key = nullptr;

#if ENABLE_KEY == 1
    key = new keyboardControl();
    if (key)
    {
        key->autodetect();
    }
    else
    {
        TRACE_WARNING(MAIN, "keyboardControl allocation error!");
    }
#endif // ENABLE_KEY

    // Init GAMEPAD
    ////////////////////////////////////////////////////////////////////////////

    gamepadControl *pad = nullptr;

#if ENABLE_PAD == 1
    pad = new gamepadControl();
    if (pad)
    {
        pad->autodetect();
    }
    else
    {
        TRACE_WARNING(MAIN, "gamepadControl allocation error!");
    }
#endif // ENABLE_PAD

    // Init Metabot & serial link
    ////////////////////////////////////////////////////////////////////////////

    Metabot *bot = nullptr;

#if ENABLE_BOT == 1
    bot = new Metabot();
    if (bot)
    {
        if (bot->setup() == true)
        {
            TRACE_INFO(MAIN, ">> Metabot is connected!");
        }
        else
        {
            delete bot;
            bot = nullptr;
            TRACE_ERROR(MAIN, "Metabot connection FATAL ERROR!");
        }
    }
    else
    {
        TRACE_ERROR(MAIN, "Metabot allocation FATAL ERROR!");
    }
#endif // ENABLE_BOT

    MiniTraces_flush();

    // Start moving around!
    ////////////////////////////////////////////////////////////////////////////

    if (bot)
    {
        RobotStatus rs;

        double loopFrequency = 30.0;
        double loopDuration  = 1000.0 / loopFrequency;
        std::chrono::time_point<std::chrono::system_clock> start, end;

        bool exit = false;
        while (exit == false)
        {
            start = std::chrono::system_clock::now(); // Loop timer

            // Metabot control
            {
                // Reset direct inputs, not states (walk, crab and inverted)
                rs.dx = 0.0;
                rs.dy = 0.0;
                rs.turn = 0.0;
                rs.height = 0.0;

                if (net)
                {
                    // Read inputs from network client
                    net->run(rs, exit);
                }

                if (key)
                {
                    // Read inputs from keyboard (overwrite network control)
                    key->run(rs, exit);
                }

                if (pad)
                {
                    // Read inputs from gamepads (overwrite keyboard & network control)
                    pad->run(rs, exit);
                }

                if (bot)
                {
                    // Forward orders to the metabot
                    bot->forward(rs);

                    // Execute movements
                    bot->move();
                }
            }

            MiniTraces_flush();

            // Loop timer
            end = std::chrono::system_clock::now();
            double loopd = std::chrono::duration_cast<std::chrono::microseconds>(end-start).count();
            double waitd = (loopDuration * 1000.0) - loopd;

    #ifdef LATENCY_TIMER
            if ((loopd / 1000.0) > loopDuration)
                TRACE_WARNING(MAIN, "Control loop duration: %f ms of the %f ms budget.", (loopd / 1000.0), loopDuration);
            else
                TRACE_1(MAIN, "Control loop duration: %f ms of the %f ms budget.", (loopd / 1000.0), loopDuration);
    #endif

            // Loop control
            if (waitd > 0.0)
            {
                std::chrono::microseconds waittime(static_cast<int>(waitd));
                std::this_thread::sleep_for(waittime);
            }
        }
    }

    delete net;
    delete key;
    delete pad;
    delete bot;

    TRACE_INFO(MAIN, "MetaBotServer exiting normally!");
    return EXIT_SUCCESS;
}
