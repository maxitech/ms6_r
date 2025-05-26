/**
 * @file ProgramLoader.h
 * @brief Declares the ProgramLoader class responsible for loading and executing predefined robot programs.
 */

#ifndef PROGRAMLOADER_H
#define PROGRAMLOADER_H

#include "LimitSwitches.h"
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
    MAIN
};

/**
 * @brief Handles logic for loading and executing robot programs based on input commands.
 */
class ProgramLoader
{
public:
    /**
     * @brief Constructs the ProgramLoader with a reference to LimitSwitches.
     * @param limitSwitches Reference to a LimitSwitches instance for diagnostic testing.
     */
    explicit ProgramLoader(LimitSwitches& limitSwitches);

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
     * @brief Executes the Main program, run commands like JOG or MOVE.
     * @internal
     */
    void _main();

    ProgramState        _currentProgramState = IDLE; ///< Current active program state. @internal
    LimitSwitches&      _limitSwitches;              ///< Reference to limit switches for diagnostics. @internal
    std::vector<String> _arguments = {};
    String              _cmd       = ""; ///< Current command @internal
};

#endif // PROGRAMLOADER_H
