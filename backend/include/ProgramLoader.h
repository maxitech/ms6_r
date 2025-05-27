/**
 * @file ProgramLoader.h
 * @brief Declares the ProgramLoader class responsible for loading and executing predefined robot programs.
 */

#ifndef PROGRAMLOADER_H
#define PROGRAMLOADER_H

#include "Homing.h"
#include "LimitSwitches.h"
#include "MotorConfig.h"
#include "teensystep4.h"
#include <Arduino.h>

/**
 * @brief Enum representing the different states/programs that can be run.
 */
enum ProgramState
{
    IDLE,          ///< Default state when no program is active.
    PING,          ///< Sends "PONG" every second.
    PONG,          ///< Sends "PING" every second.
    TEST_SWITCHES, ///< Runs limit switch diagnostic.
    HOME,          ///< Executes homing routine for all axes.
    MAIN
};

enum JogState
{
    IDLE_JOG, ///< No jog command is active.
    JOGGING,  ///< Currently executing a jog command.
};

enum JogCommand
{
    JOG_START,  ///< Start jogging in the specified direction.
    JOG_STOP,   ///< Stop the current jog operation.
    JOG_UNKNOWN ///< Unknown jog command state.
};

/**
 * @brief Handles logic for loading and executing robot programs based on input commands.
 */
class ProgramLoader
{
public:
    /**
     * @brief Constructs the ProgramLoader with a reference to LimitSwitches.
     * @param motorConfigs Reference to a vector of MotorConfig's for the robot's motors.
     * @param limitSwitches Reference to a LimitSwitches instance for diagnostic testing.
     */
    ProgramLoader(Homing* homingManager, std::vector<MotorConfig>& motorConfigs, LimitSwitches& limitSwitches);

    /**
     * @brief Handles incoming command parts and loads a matching program.
     * @param cmd A string which is the command.
     * @param args A vector of strings arguments related to the command.
     */
    void handleCommand(const String& cmd, const std::vector<String>& args);

    /**
     * @brief Executes the current program logic. Should be called repeatedly inside the main loop.
     */
    void run();

    /**
     * @brief Returns the current program state.
     * @return The current ProgramState.
     */
    ProgramState getState() const;

private:
    /**
     * @brief Loads and activates a program by name.
     * @param program The name of the program to load (e.g., "PING", "PONG").
     * @internal
     */
    void _loadProgram(const String& program);

    /**
     * @brief Changes the current state to a new program.
     * @param newState The new program state to activate.
     * @return The updated ProgramState.
     * @internal
     */
    ProgramState _setState(ProgramState newState);

    /**
     * @brief Sends "PONG" every second (part of the PING program).
     * @internal
     */
    void _executePing();

    /**
     * @brief Sends "PING" every second (part of the PONG program).
     * @internal
     */
    void _executePong();

    /**
     * @brief Executes the TEST_SWITCHES program, triggering limit switch checks.
     * @internal
     */
    void _testSwitches();

    /**
     * @brief Executes the HOME program, running the homing routine for all axes.
     * @internal
     */
    void _home();

    /**
     * @brief Executes the Main program, run commands like JOG or MOVE.
     * @internal
     */
    void _main();

    /**
     * @brief Parses the jog command from the input string.
     * @param str The jog command string.
     * @return The JogCommand enum value representing the jog command.
     * @internal
     */
    JogCommand _getJogCommand(const String& str);

    Homing*                   _homingManager;              ///< Pointer to the Homing manager for homing routines.
    std::vector<MotorConfig>& _motorConfigs;               ///< Vector of motor configurations for the robot.
    ProgramState              _currentProgramState = IDLE; ///< Current active program state. @internal
    LimitSwitches&            _limitSwitches;              ///< Reference to limit switches for diagnostics. @internal
    std::vector<String>       _arguments = {};
    String                    _cmd       = ""; ///< Current command @internal
};

#endif // PROGRAMLOADER_H
