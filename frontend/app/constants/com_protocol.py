## Current Command Assignments


START_BYTES = [0xAA, 0xAA, 0xAA]
END_BYTES = [0x0D, 0x0A]  # CR,LF


# Global Control 0x01-0x0F (Start,Stop,Idle,Pause,Resume,etc.)
NOP = 0x00  # no operation
CMD_START = 0x01
CMD_STOP = 0x02
CMD_IDLE = 0x03
# CMD_PAUSE = 0x04
# CMD_RESUME = 0x05


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

# Reserve 0xA0-0xFF
