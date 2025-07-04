/**
 * @file ProgramLoader.h
 * @brief Declares the ProgramLoader class responsible for loading and executing predefined robot programs.
 */

#ifndef PROGRAMLOADER_H
#define PROGRAMLOADER_H

#include "Homing.h"
#include "LimitSwitches.h"
#include "MotorConfig.h"
#include "Utils.h"
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
    MAIN           ///< Main program for executing commands like JOG or MOVE.
};

enum ExecutionState
{
    EXEC_IDLE,    ///< No program is currently executing.
    EXEC_RUNNING, ///< A program is currently running.
    EXEC_STOPPED  ///< The program execution has been stopped.

};

enum JogState
{
    IDLE_JOG, ///< No jog command is active.
    JOGGING,  ///< Currently executing a jog command.
};

struct JogFlags
{
    bool   limitReached = false;
    String blockedDir   = "";
    bool   runOnce      = false;
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
    //  ******************************PUBLIC FUNCTIONS********************************
    /**
     * @brief Constructs the ProgramLoader with a reference to LimitSwitches.
     * @param motorConfigs Reference to a vector of MotorConfig's for the robot's motors.
     * @param limitSwitches Reference to a LimitSwitches instance for diagnostic testing.
     */
    ProgramLoader(Homing* homingManager, std::vector<MotorConfig*>& motorConfigs, LimitSwitches& limitSwitches);

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
    //  ******************************PRIVATE FUNCTIONS********************************
    /**
     * @brief Loads and activates a program by name.
     * @param program The name of the program to load (e.g., "PING", "PONG").
     * @internal
     */
    void _loadProgram(const String& program);

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
     * @brief Parses the jog command from the input string.
     * @param str The jog command string.
     * @return The JogCommand enum value representing the jog command.
     * @internal
     */
    JogCommand _getJogCommand(const String& str);

    /**
     * @brief Executes the jog command for a specific joint.
     * @param currJogState Reference to the current JogState.
     * @param motorIdx The index of the motor to jog.
     * @internal
     */
    void _jogJoint(JogState& currJogState, const int motorIdx);

    /**
     * @brief Stops all motors if they are currently moving.
     * @internal
     */
    void _stopMotors();

    /**
     * @brief Sends the forward kinematics pose and joint angles to the serial output.
     * @internal
     */
    void _sendFkPoseAndJointAngles();

    /**
     * @brief Sends the current motor position in steps to the serial output.
     * @param motorIdx The index of the motor to send the position for.
     * @internal
     */
    void _sendMotorPosInSteps(const int motorIdx);

    // ******************************MEMBER VARIABLES********************************
    Homing*                              _homingManager;                   ///< Pointer to the Homing manager for homing routines.
    std::vector<MotorConfig*>&           _motorConfigs;                    ///< Vector of motor configurations for the robot. Note: Setup class has full ownership of this vector.
    ProgramState                         _currentProgramState = IDLE;      ///< Current active program state. @internal
    ExecutionState                       _executionState      = EXEC_IDLE; ///< Current execution state of the program. @internal
    LimitSwitches&                       _limitSwitches;                   ///< Reference to limit switches for diagnostics. @internal
    std::vector<String>                  _arguments;                       ///< Arguments passed with the command. @internal
    String                               _cmd;                             ///< Current command @internal
    bool                                 _isHomingDone = false;            ///< Flag to check if homing is done. @internal
    std::array<JogFlags, Utils::NUM_DOF> _jogFlags;
};

#endif // PROGRAMLOADER_H
