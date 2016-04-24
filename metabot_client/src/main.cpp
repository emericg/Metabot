
#include "ctrl_gamepad.h"
#include "ctrl_keyboard.h"
#include "network.h"
#include "minitraces.h"

#include "metabotgui.h"
#include <QApplication>

#include <chrono>
#include <thread>

//#define LATENCY_TIMER
#define ENABLE_KEY 1
#define ENABLE_PAD 1

void main_infos()
{
    TRACE_INFO(MAIN, "Metabot " BLD_WHITE "CLIENT" CLR_RESET " launched!\n");
    TRACE_INFO(MAIN, "Built on '%s, %s'\n", __DATE__ , __TIME__);
    TRACE_INFO(MAIN, "\n");

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
/*
    QApplication a(argc, argv);
    MetaBotGui w;
    w.show();

    return a.exec();
*/
    bool status = false;
    MoveitMoveit move;

    // Init KEYBOARD
    ////////////////////////////////////////////////////////////////////////////

    keyboardControl *key = NULL;

#if ENABLE_KEY == 1
    key = new keyboardControl();
    if (key)
    {
        key->autodetect();
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
#endif

    // Init NETWORK
    ////////////////////////////////////////////////////////////////////////////

    networkClient *com = NULL;

    com = new networkClient();
    if (com)
    {
        com->autodetect();
    }

    // Go for it MOTHAFUCKA
    ////////////////////////////////////////////////////////////////////////////

    double syncloopFrequency = 30.0;
    double syncloopDuration = 1000.0 / syncloopFrequency;
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

            if (key)
            {
                // Read input from keyboard
                key->run(move, exit);
            }

            if (pad)
            {
                // Read input from gamepads
                pad->run(move, exit);
            }

            if (exit)
            {
                break;
            }

            // Network stuff
            if (com)
            {
                com->send(move);
            }
        }

        // Loop timer
        end = std::chrono::system_clock::now();
        double loopd = std::chrono::duration_cast<std::chrono::microseconds>(end-start).count();
        double waitd = (syncloopDuration * 1000.0) - loopd;

#ifdef LATENCY_TIMER
        if ((loopd / 1000.0) > syncloopDuration)
            TRACE_WARNING(MAIN, "Control loop duration: %f ms of the %f ms budget.\n", (loopd / 1000.0), loopDuration);
        else
            TRACE_1(MAIN, "Control loop duration: %f ms of the %f ms budget.\n", (loopd / 1000.0), loopDuration);
#endif

        if (waitd > 0.0)
        {
            std::chrono::microseconds waittime(static_cast<int>(waitd));
            std::this_thread::sleep_for(waittime);
        }
    }

    TRACE_INFO(MAIN, "MetaBotClient exiting normaly!\n");
    return EXIT_SUCCESS;
}