/**
 * @file ProgramLoader.h
 * @brief Declares the ProgramLoader class responsible for loading and executing predefined robot programs.
 */

#ifndef PROGRAMLOADER_H
#define PROGRAMLOADER_H

#include "ComProtocoll.h"
#include "Homing.h"
#include "JogCommand.h"
#include "JogController.h"
#include "JogState.h"
#include "LimitSwitches.h"
#include "MotorConfig.h"
#include "ProcessedData.h"
#include "RobotDataSender.h"
#include "Utils.h"
#include "teensystep4.h"
#include <Arduino.h>
#include <optional>

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
    MAIN           ///< Main program for executing commands like JOG or MOVE.
};

enum ExecutionState
{
    EXEC_IDLE,    ///< No program is currently executing.
    EXEC_RUNNING, ///< A program is currently running.
    EXEC_STOPPED  ///< The program execution has been stopped.

};

/**
 * @brief Handles logic for loading and executing robot programs based on input commands.
 */
class ProgramLoader
{
public:
    //  ******************************PUBLIC FUNCTIONS********************************
    /**
     * @brief Constructs the ProgramLoader with a reference to LimitSwitches.
     * @param motorConfigs Reference to a vector of MotorConfig's for the robot's motors.
     * @param limitSwitches Reference to a LimitSwitches instance for diagnostic testing.
     */
    ProgramLoader(Homing* homingManager, std::vector<MotorConfig*>& motorConfigs, LimitSwitches& limitSwitches);

    ~ProgramLoader(); ///< Declaration of deconstructor.

    /**
     * @brief Handles incoming command parts and loads a matching program.
     * @param processedDta Struct which contains cmdId and its needed data.
     */
    void handleCommand(const ProcessedData& processedDta);

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
    //  ******************************PRIVATE FUNCTIONS********************************
    /**
     * @brief Loads and activates a program by name.
     * @param program The name of the program to load (e.
     * @internal
     */
    void _loadProgram(const uint8_t program);

    /**
     * @brief Executes the current loaded program.
     * @internal
     */
    void _start();

    /**
     * @brief Stops the currently running program and resets the state to IDLE.
     * @internal
     */
    void _stop();

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

    //  ******************************HELPER FUNCTIONS********************************

    /**
     * @brief Stops all motors if they are currently moving.(smooth stop)
     * @internal
     */
    void _stopMotors();

    /**
     * @brief Stops all motors if they are currently moving.(emergency stop)
     * @internal
     */
    void _eStopMotors();

    // ******************************MEMBER VARIABLES********************************
    Homing*                             _homingManager;                   ///< Pointer to the Homing manager for homing routines. @internal
    std::vector<MotorConfig*>&          _motorConfigs;                    ///< Vector of motor configurations for the robot. Note: Setup class has full ownership of this vector. @internal
    ProgramState                        _currentProgramState = IDLE;      ///< Current active program state. @internal
    ExecutionState                      _executionState      = EXEC_IDLE; ///< Current execution state of the program. @internal
    LimitSwitches&                      _limitSwitches;                   ///< Reference to limit switches for diagnostics. @internal
    ProcessedData                       _processedDta;                    ///< Struct which contains processed data @internal
    uint8_t                             _cmdId;                           ///< Current command @internal
    bool                                _isHomingDone = false;            ///< Flag to check if homing is done. @internal
    RobotDataSender                     _rbtDtaSender;                    ///< Instance of data sender class for the robot. @internal
    JogController*                      _jogCtrl;                         ///< Pointer of the jog controller class which handles jog of the robot. @internal
    std::optional<std::vector<int32_t>> _jogSpeeds;
};

#endif // PROGRAMLOADER_H
