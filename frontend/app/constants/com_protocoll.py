# Protocol defines three categories of commands:
## → Primary
## → System
## → Modifier

# Primary
## Represent primary command instructions.
## Internally encoded as 'CMD_ID' (1 byte). Limit: 255 unique commands. 0x01-0xFF(1-255)

# Modifier
## Augment motion commands with additional parameters (e.g., joint positions, joint speeds).

# System
## Represent non-movement operations such as diagnostics or status updates.
## Always included in payload. No movement execution.


# Package Structure:
# MAX_PACKAGE_SIZE = 256 bytes

# Field: uint24 Start         → 3 bytes         // Message start identifier
# Field: uint8 Payload Length → 1 byte          // Length of payload in bytes
# Field: Payload              → n bytes
### Field: uint8 CMD_ID    → 1 byte          // Command identifier
#### Payload Details based on CMD_ID:
##### Field: Joint Position    → 18 bytes        // 6 joints × 3 bytes each, MSB first
##### [Extend with additional payload types as needed]
##### Field Fixed Payload -> n bytes
#### Field: Fixed Payload Length -> 1byte // must be last byte in payload


# Field: uint16 CRC16         → 2 bytes         // Data integrity check LSB first
# Field: uint16 End           → 2 bytes         // Message end identifier


START_BYTES = [0xAA, 0xAA, 0xAA]
END_BYTES = [0x0D, 0x0A]  # CR,LF
NOP = 0x00  # no operation


CMD_JOG = 0x01
CMD_MOVE_TO_POS = 0x02
CMD_STOP = 0x03
CMD_PING = 0x04
