# Command Protocol Specification

## Command Categories and ID Ranges

### Primary Commands

**Definition**: Main instructions that trigger movement, program execution, or global state changes.  
**CMD_ID Range**: `0x01–0x7F` (split into fixed functional ranges)

| Range     | Category        | Description / Examples                    |
| --------- | --------------- | ----------------------------------------- |
| 0x01–0x0F | Global Control  | Start, Stop, Idle, Pause, Resume          |
| 0x10–0x4F | Movement        | Jog, Move To Position, Move Relative      |
| 0x50–0x7F | Program Control | Load Program, Select Program, Run Program |

---

### Modifier Commands

**Definition**: Parameters that extend primary commands with additional data.  
**Range**: Not tied to a fixed range — depends on the calling primary command.  
**Examples**: Joint positions, joint speeds, load parameters, execution options.

| Range     | Category                               | Description / Examples                |
| --------- | -------------------------------------- | ------------------------------------- |
| 0x80–0x8F | Program Selection (CMD_LOAD Modifiers) | Ping, Pong, Test Switches, Home, Main |

---

### Fixed Payload Commands

**Range**: `0x90–0x9F`  
**Examples**: System/Debug requests like Diagnostics or Data Request

| Range     | Category       | Description / Examples    |
| --------- | -------------- | ------------------------- |
| 0x90–0x9F | System / Debug | Diagnostics, Data Request |

---

### Reserved

**Range**: `0xA0–0xFF`  
**Purpose**: Future expansion, vendor-specific features.

---

## Packet Structure

**MAX_PACKAGE_SIZE** = 256 bytes

| Field                         | Size    | Description                                            |
| ----------------------------- | ------- | ------------------------------------------------------ |
| Start                         | 3 bytes | Message start identifier (`0xAA 0xAA 0xAA`)            |
| Payload Length                | 1 byte  | Number of bytes in payload                             |
| Payload                       | n bytes | Content of the message (see details below)             |
| ├─ CMD_ID                     | 1 byte  | Primary Command ID                                     |
| ├─ Command Payload            | n bytes | Command-specific data (may include modifiers)          |
| ├─ Fixed Payload              | n bytes | System/Debug requests                                  |
| \*\*\*└─ Fixed Payload Length | 1 byte  | Length of fixed payload (must be last byte in payload) |
| CRC16                         | 2 bytes | CRC-16 checksum (LSB first)                            |
| End                           | 2 bytes | Message end identifier (`0x0D 0x0A`)                   |

---

## Current Command Assignments

```python
START_BYTES = [0xAA, 0xAA, 0xAA]
END_BYTES = [0x0D, 0x0A]  # CR,LF

# Global Control 0x01-0x0F
NOP = 0x00
CMD_START = 0x01
CMD_STOP = 0x02
CMD_IDLE = 0x03

# Movement 0x10-0x4F
CMD_JOG = 0x10
CMD_MOVE_TO_POS = 0x11

# Program Control 0x50-0x7F
CMD_LOAD = 0x50

# Program Selection (CMD_LOAD Modifiers) 0x80-0x8F
PRG_PING = 0x80
PRG_PONG = 0x81
PRG_TEST_SWITCHES = 0x82
PRG_HOME = 0x83
PRG_MAIN = 0x84

# Fixed Payload System/Debug 0x90-0x9F
SYS_DIAGNOSTICS = 0x90
SYS_DATA_REQUEST = 0x91

# Reserve 0xA0-0xFF
```
