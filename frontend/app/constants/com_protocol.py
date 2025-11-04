## Current Command Assignments


START_BYTES = [0xAA, 0xAA, 0xAA]
END_BYTES = [0x0D, 0x0A]  # CR,LF


# Global Control 0x01-0x0F (Start,Stop,Idle,Pause,Resume,etc.)
NOP = 0x00  # no operation
CMD_START = 0x01
CMD_STOP = 0x02
CMD_IDLE = 0x03
CMD_SETUP = 0x04  # only used for errors while processing setup data


# Movement 0x10-0x4F(Jog,MTP,MR,etc.)
CMD_JOG = 0x10
CMD_MOVE_TO_POS = 0x11

# Program Control 0x50-0x7F(Load, etc.)
CMD_LOAD = 0x50
# CMD_SELECT_PRG = 0x51
# CMD_RUN_PRG = 0x52


# Program Selection (CMD_LOAD Modifiers)** 0x80-0x8F (Information for CMD_LOAD - which predefined program should be loaded)
PRG_PING = 0x80
PRG_PONG = 0x81
PRG_TEST_SWITCHES = 0x82
PRG_HOME = 0x83
PRG_MAIN = 0x84


# Fixed Payload System/Debug 0x90-0x9F ( Diagnostics, DataRequest,etc.)
SYS_DIAGNOSTICS = 0x90
SYS_DATA_REQUEST = 0x91

# Reserved 0xA0-0xDF & 0xE2-0xFF


# *******************Response******************

# Status Codes
STATUS_OK = 0xE0
STATUS_ERROR = 0xE1

# Detail Codes when STATUS_OK
# Data range: 0x00–0x1F
DATA_NONE = 0x00
DATA_STEPS = 0x01

# Info range: 0x20–0x3F
INFO_LOADED_PROGRAM = 0x20
INFO_RELOADED_PROGRAM = 0x21
INFO_ALREADY_RUNNING = 0x22
INFO_HOMING_ALREADY_DONE = 0x23
INFO_PING = 0x24

# Warn range: 0x40–0x5F
WARN_NOP_IGNORED = 0x40
WARN_NO_PROGRAM_LOADED = 0x41
WARN_LIMIT_HIT = 0x42
WARN_ARM_NOT_HOMED = 0x43

# Detail Codes when STATUS_ERROR
# Error range: 0x60–0x7F
ERR_UNKNOWN = 0x60
ERR_INVALID_PACKET = 0x61
ERR_TIMEOUT = 0x62
ERR_CHECKSUM = 0x63

ERR_NO_PROCESSOR = 0x64
ERR_BUFFER_OVERFLOW = 0x65
ERR_INVALID_CMD = 0x66
ERR_INVALID_TELEMETRY_FLAG = 0x67

ERR_UNKNOWN_PROGRAM = 0x68
ERR_INDEX_OOB = 0x69
ERR_INVALID_DIR = 0x6A
ERR_JSON_PARSE = 0x6B
ERR_ALLOC_FAIL = 0x6C
ERR_MISSING_FIELD = 0x6D
