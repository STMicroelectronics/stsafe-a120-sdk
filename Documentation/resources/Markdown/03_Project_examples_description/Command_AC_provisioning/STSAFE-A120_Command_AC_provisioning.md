# STSAFE-A120 Command Access Condition Provisioning {#STSAFE-A120_Command_AC_provisioning}

This example demonstrates how to configure command access conditions (AC) and encryption flags for STSAFE-A120 commands. Command access conditions define the security level required to execute specific commands, while encryption flags control whether command and response frames must be encrypted when using a host secure session.

Access condition provisioning is a critical security configuration that determines which commands can be executed freely, which require host authentication (via host session), and which are forbidden. This configuration is typically performed during device personalization and cannot be changed afterward once the change right is disabled.

## Example Flowchart

The following flowchart illustrates the command AC provisioning process:

@startuml{Command_AC_provisioning_flowchart.png} "Command AC Provisioning flowchart" width=5cm
	:MAIN;
	:Initialize application terminal (baudrate = 115200);
	:Display example title and instructions;
	:ret = <b>stse_init</b>;
	if(ret == STSE_OK) then (No)
	:Display error message;
	:Terminate application;
	-[hidden]->
	detach
	else (Yes)
	:Query current command AC records;
	:Display current access conditions;
	:Display WARNING - permanent operation;
	:Wait for user confirmation;
	:Provision command access conditions from table;
	:Provision encryption flags from table;
	:Query updated command AC records;
	:Display updated access conditions;
	while (while(1))
	end while
	-[hidden]->
	detach
	endif
@enduml

## Host Terminal Output

Upon execution, the host terminal displays the current and updated command access conditions:

```
----------------------------------------------------------------------------------------------------------------
-                            STSAFE-A120 Command Access Condition Provisioning                                  -
----------------------------------------------------------------------------------------------------------------

 - Initialize target STSAFE-A120

 - Query current command AC records

  HEADER | EXT-HEADER |    AC   | CMDEnc | RSPEnc  
   0x00  |     -     |  FREE  |  NO  |  NO  
   0x02  |     -     |  FREE  |  NO  |  NO  
   0x0E  |     -     | HOST  |  YES |  NO  
   0x0F  |     -     | HOST  |  NO  | YES  

+--------------------------------------------------------------------------------------------------------------+
|                                                                                                              |
|                                           /!\  W A R N I N G  /!\                                            |
|                                                                                                              |
| This operation will permanently configure command access conditions.                                         |
| Incorrect settings may lock device functions.                                                                |
|                                                                                                              |
| Press any key to continue and provision access conditions:                                                   |
|                                                                                                              |
+--------------------------------------------------------------------------------------------------------------+

 - Provisioning command access conditions...
 - Provisioning encryption flags...
 - Query updated command AC records

  HEADER | EXT-HEADER |    AC   | CMDEnc | RSPEnc  
   0x00  |     -     |  FREE  |  NO  |  NO  
   0x02  |     -     |  FREE  |  NO  |  NO  
   0x0E  |     -     | HOST  |  YES |  NO  
   0x0F  |     -     | HOST  |  NO  | YES  

 - Command AC provisioning completed successfully
```

## Access Condition Levels

The STSAFE-A120 supports several access condition levels:

- **FREE (0x01):** Command can be executed without any authentication. Suitable for read operations and non-sensitive commands.
- **HOST (0x03):** Command requires host authentication via a secure session (C-MAC). Protects sensitive operations.
- **ADMIN:** Command requires administrative password authentication. Used for critical configuration operations.
- **NEVER:** Command is permanently disabled and cannot be executed.

## Encryption Flags

Encryption flags control data protection during command execution:

- **ENCRYPT_NO (0x00):** No encryption required for command or response.
- **ENCRYPT_CMD (0x02):** Command frame must be encrypted (C-ENC).
- **ENCRYPT_RSP (0x01):** Response frame must be encrypted (R-ENC).
- **ENCRYPT_BOTH (0x03):** Both command and response must be encrypted.

## Applicative Scenarios

Command access condition provisioning is essential for:

- **Production Device Personalization:** Configure security policies during device manufacturing to enforce access controls.
- **Security Hardening:** Lock down sensitive commands to prevent unauthorized access in deployed devices.
- **Compliance Requirements:** Meet regulatory requirements by enforcing authentication for specific operations.
- **Multi-Tenant Systems:** Configure different access levels for different users or applications sharing the same device.
- **Secure Communication:** Enforce encryption for commands that handle sensitive data (e.g., key wrapping, unwrapping).

**WARNING:** This is a permanent operation. Once the change right is disabled, command access conditions cannot be modified. Carefully plan and test your AC configuration before deploying to production devices.
