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
**Exception**: `CMD_LOAD` modifiers — **See below**:
| Range | Category | Description / Examples |
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

**Range**: `0xA0–0xDF` & `0xE2-0xFF`
**Purpose**: Future expansion, vendor-specific features.

---

## Status Codes

### Status Code Range

| Range     | Category       | Description                                  |
| --------- | -------------- | -------------------------------------------- |
| 0xE0–0xE1 | Status Codes   | Top-level execution result (OK, ERROR)       |
| 0x00–0x1F | Success – Data | Returned when STATUS_OK → data results       |
| 0x20–0x3F | Success – Info | Returned when STATUS_OK → informational only |
| 0x40–0x5F | Success – Warn | Returned when STATUS_OK → warning messages   |
| 0x60–0x7F | Error Codes    | Returned when STATUS_ERROR → error details   |

---

## Packet Structure

**MAX_PACKAGE_SIZE** = 256 bytes

### Request Packet (Frontend → Backend)

| Field                   | Size    | Description                                            |
| ----------------------- | ------- | ------------------------------------------------------ |
| Start                   | 3 bytes | Message start identifier (`0xAA 0xAA 0xAA`)            |
| Payload Length          | 1 byte  | Number of bytes in payload                             |
| Payload                 | n bytes | Content of the message (see below)                     |
| ├─ CMD_ID               | 1 byte  | Primary Command ID                                     |
| ├─ Command Payload      | n bytes | Command-specific data (may include modifiers)          |
| ├─ Fixed Payload        | n bytes | System/Debug requests only                             |
| └─ Fixed Payload Length | 1 byte  | Length of fixed payload (must be last byte in payload) |
| CRC16                   | 2 bytes | CRC-16 checksum (LSB first)                            |
| End                     | 2 bytes | Message end identifier (`0x0D 0x0A`)                   |

---

### Response Packet (Backend → Frontend)

| Field            | Size    | Description                                                                     |
| ---------------- | ------- | ------------------------------------------------------------------------------- |
| Start            | 3 bytes | Message start identifier (`0xAA 0xAA 0xAA`)                                     |
| Payload Length   | 1 byte  | Number of bytes in payload                                                      |
| Payload          | n bytes | Response content                                                                |
| ├─ CMD_ID        | 1 byte  | Echoed Command ID from request                                                  |
| ├─ Status Code   | 1 byte  | Response status (see Status Codes)                                              |
| ├─ Detail Code   | 1 byte  | Success or error detail (depends on Status)                                     |
| └─ Response Data | n bytes | Optional data depends on Status and Detail Code (e.g. steps, diagnostics, etc.) |
| CRC16            | 2 bytes | CRC-16 checksum (LSB first)                                                     |
| End              | 2 bytes | Message end identifier (`0x0D 0x0A`)                                            |

---

## Example Packets

### Request – Jog Joint

    Start: AA AA AA
    Length: 21
    Payload:
    CMD_ID: 10 (CMD_JOG)
    Data: joint speeds 18bytes (3bytes for each joint)
    Fixed Payload: 00 (NOP)
    Fixed Payload Length: 01
    CRC16: xx xx
    End: 0D 0A

---

### Response – OK

    Start: AA AA AA
    Length: 03
    Payload:
    CMD_ID: 10 (CMD_JOG)
    Status: E0 (STATUS_OK)
    Sucess Detail: 00 (DATA_NONE)
    CRC16: xx xx
    End: 0D 0A

---

### Data Packet

    Start: AA AA AA
    Length: 21
    Payload:
    CMD_ID: 10 (CMD_JOG)
    Status: E0 (STATUS_OK)
    Success Detail: 01 DATA_STEPS
    Response Data: joint steps (18 bytes, 3 per joint)
    CRC16: xx xx
    End: 0D 0A

---

### Response – Error (Not Homed)

    Start: AA AA AA
    Length: 03
    Payload:
    CMD_ID: 10 (CMD_JOG)
    Status: E1 (STATUS_ERROR)
    Error Detail: 08 (ERR_ARM_NOT_HOMED)
    CRC16: xx xx
    End: 0D 0A

---

## Current Command and Status Assignments

```python
# *******************Request******************

START_BYTES = [0xAA, 0xAA, 0xAA]
END_BYTES = [0x0D, 0x0A]  # CR,LF

# Global Control 0x01-0x0F
NOP = 0x00
CMD_START = 0x01
CMD_STOP = 0x02
CMD_IDLE = 0x03
CMD_SETUP = 0x04; # only used for errors while processing setup data

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

# Reserved 0xA0-0xDF & 0xE2-0xFF



# *******************Response******************

# Status Codes
STATUS_OK    = 0xE0
STATUS_ERROR = 0xE1

# Detail Codes when STATUS_OK
# Data range: 0x00–0x1F
DATA_NONE  = 0x00
DATA_STEPS = 0x01

# Info range: 0x20–0x3F
INFO_LOADED_PROGRAM      = 0x20
INFO_RELOADED_PROGRAM    = 0x21
INFO_ALREADY_RUNNING     = 0x22
INFO_HOMING_ALREADY_DONE = 0x23
INFO_PING = 0x24

# Warn range: 0x40–0x5F
WARN_NOP_IGNORED       = 0x40
WARN_NO_PROGRAM_LOADED = 0x41
WARN_LIMIT_HIT         = 0x42
WARN_ARM_NOT_HOMED     = 0x43

# Detail Codes when STATUS_ERROR
# Error range: 0x60–0x7F
ERR_UNKNOWN                = 0x60
ERR_INVALID_PACKET         = 0x61
ERR_TIMEOUT                = 0x62
ERR_CHECKSUM               = 0x63
ERR_NO_PROCESSOR           = 0x64
ERR_BUFFER_OVERFLOW        = 0x65
ERR_INVALID_CMD            = 0x66
ERR_INVALID_TELEMETRY_FLAG = 0x67
ERR_UNKNOWN_PROGRAM        = 0x68
ERR_INDEX_OOB              = 0x69
ERR_INVALID_DIR            = 0x6A
ERR_JSON_PARSE             = 0x6B
ERR_ALLOC_FAIL             = 0x6C
ERR_MISSING_FIELD = 0x6D;
```
