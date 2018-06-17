#include "../metabot_server/src/minitraces.h"
#include "../metabot_server/src/ctrl_gamepad.h"
#include "../metabot_server/src/ctrl_keyboard.h"

#include "network.h"
#include "metabotgui.h"

#include <chrono>
#include <thread>

#include <QApplication>

//#define LATENCY_TIMER
#define ENABLE_KEY 0
#define ENABLE_PAD 1

void main_infos()
{
    TRACE_INFO(MAIN, "Metabot " BLD_WHITE "CLIENT" CLR_RESET " launched!");
    TRACE_INFO(MAIN, "Built on '%s, %s'", __DATE__ , __TIME__);
    TRACE_INFO(MAIN, "");

#if ENABLE_KEY == 1
    TRACE_INFO(MAIN, "Keyboard control is " BLD_GREEN "ENABLED" CLR_RESET);
#else
    TRACE_INFO(MAIN, "Keyboard control is " BLD_RED "DISABLED" CLR_RESET);
#endif

#if ENABLE_PAD == 1
    TRACE_INFO(MAIN, "Gamepad control is " BLD_GREEN "ENABLED" CLR_RESET);
#else
    TRACE_INFO(MAIN, "Gamepad control is " BLD_RED "DISABLED" CLR_RESET);
#endif

    TRACE_INFO(MAIN, "");
}

int main(int argc, char *argv[])
{
    bool status = false;

    main_infos();
/*
    QApplication a(argc, argv);
    MetaBotGui w;
    w.show();

    return a.exec();
*/

    // Init KEYBOARD
    ////////////////////////////////////////////////////////////////////////////

    keyboardControl *key = nullptr;

#if ENABLE_KEY == 1
    key = new keyboardControl();
    if (key)
    {
        key->autodetect();
    }
#endif

    // Init GAMEPAD
    ////////////////////////////////////////////////////////////////////////////

    gamepadControl *pad = nullptr;

#if ENABLE_PAD == 1
    pad = new gamepadControl();
    if (pad)
    {
        pad->autodetect();
    }
#endif

    // Init NETWORK
    ////////////////////////////////////////////////////////////////////////////

    TRACE_INFO(MAIN, "MetaBotClient trying connection");

    networkClient *net = new networkClient();
    if (net)
    {
        bool conn = false;
        int timeout = 30;

        while (status == false && timeout > 0)
        {
            status = net->autodetect();
            timeout--;

            MiniTraces_flush();

            if (status == false)
            {
                std::chrono::seconds waittime(1);
                std::this_thread::sleep_for(waittime);
            }
        }
    }


    // Start moving around!
    ////////////////////////////////////////////////////////////////////////////

    RobotStatus move;

    TRACE_INFO(MAIN, "MetaBotClient control loop starting");

    double syncloopFrequency = 60.0;
    double syncloopDuration = 1000.0 / syncloopFrequency;
    std::chrono::time_point<std::chrono::system_clock> start, end;

    while (status)
    {
        bool exit = false;
        start = std::chrono::system_clock::now(); // Loop timer

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

            if (net)
            {
                net->send(move);
            }
        }

        MiniTraces_flush();

        // Loop timer
        end = std::chrono::system_clock::now();
        double loopd = std::chrono::duration_cast<std::chrono::microseconds>(end-start).count();
        double waitd = (syncloopDuration * 1000.0) - loopd;

#ifdef LATENCY_TIMER
        if ((loopd / 1000.0) > syncloopDuration)
            TRACE_WARNING(MAIN, "Control loop duration: %f ms of the %f ms budget.", (loopd / 1000.0), loopDuration);
        else
            TRACE_1(MAIN, "Control loop duration: %f ms of the %f ms budget.", (loopd / 1000.0), loopDuration);
#endif

        if (waitd > 0.0)
        {
            std::chrono::microseconds waittime(static_cast<int>(waitd));
            std::this_thread::sleep_for(waittime);
        }
    }

    TRACE_INFO(MAIN, "MetaBotClient exiting normaly!");
    return EXIT_SUCCESS;
}
