# STSAFE-A120 Symmetric Key Provisioning Control Fields {#STSAFE-A120_Symmetric_key_provisioning_control_fields}

This example demonstrates how to configure the provisioning control fields for symmetric key slots in the STSAFE-A120 secure element. These control fields determine the methods and security levels allowed for provisioning symmetric keys into each slot.

Control fields provide fine-grained control over how symmetric keys can be installed into the STSAFE-A120, including whether keys can be written in plaintext, wrapped (encrypted), or derived via ECDHE (Elliptic Curve Diffie-Hellman Ephemeral). This configuration is critical for establishing the security posture of the device.

## Example Flowchart

The following flowchart illustrates the control field configuration process:

@startuml{Symmetric_key_provisioning_control_fields_flowchart.png} "Symmetric Key Provisioning Control Fields flowchart" width=5cm
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
	:Query current control field settings for slot 0;
	:Display current settings;
	:Configure control fields to enable provisioning methods;
	:Display WARNING - permanent configuration;
	:Wait for user confirmation;
	:Write updated control fields to device;
	:Verify changes by querying again;
	:Display updated settings;
	while (while(1))
	end while
	-[hidden]->
	detach
	endif
@enduml

## Host Terminal Output

Upon execution, the host terminal displays the control field configuration:

```
----------------------------------------------------------------------------------------------------------------
-              STSAFE-A120 Configure Symmetric key slot provisioning control fields example                    -
----------------------------------------------------------------------------------------------------------------

 - Initialize target STSAFE-A120

 - Query current control fields for slot 0

 ------+----------------------------------------------------------------
       |                    PROVISIONNING CTRL FIELDS                   
 ------+--------------+---------+-----------+---------+---------+-------
  SLOT | CHANGE RIGHT | DERIVED | PLAINTEXT | PUT KEY | WRAPPED | ECDHE 
 ------+--------------+---------+-----------+---------+---------+-------
   00  |       x      |    .    |     .     |    .    |    .    |   .   

+--------------------------------------------------------------------------------------------------------------+
|                                                                                                              |
|                                           /!\  W A R N I N G  /!\                                            |
|                                                                                                              |
| This operation will permanently configure symmetric key provisioning control fields.                         |
| Once change_right is disabled, these settings cannot be modified again.                                      |
|                                                                                                              |
| Press any key to continue and configure control fields:                                                      |
|                                                                                                              |
+--------------------------------------------------------------------------------------------------------------+

 - Configuring control fields...
 - Control fields updated successfully

 - Query updated control fields for slot 0

 ------+----------------------------------------------------------------
       |                    PROVISIONNING CTRL FIELDS                   
 ------+--------------+---------+-----------+---------+---------+-------
  SLOT | CHANGE RIGHT | DERIVED | PLAINTEXT | PUT KEY | WRAPPED | ECDHE 
 ------+--------------+---------+-----------+---------+---------+-------
   00  |       x      |    .    |     .     |    x    |    x    |   x   
```

## Control Field Options

The symmetric key slot provisioning control fields include:

- **Change Right:** Indicates whether control fields can still be modified. Once disabled, the configuration becomes permanent.
- **Derived:** Allows key derivation using key derivation functions (e.g., HKDF).
- **Plaintext:** Allows keys to be written in plaintext (not recommended for production).
- **Put Key:** Allows keys to be installed using the put key command.
- **Wrapped (Anonymous):** Allows keys to be provisioned using wrapped envelope without authentication.
- **Wrapped (Authenticated):** Allows keys to be provisioned using wrapped envelope with authentication via specified key slot.
- **ECDHE (Anonymous):** Allows keys to be provisioned using ECDHE key agreement without authentication.
- **ECDHE (Authenticated):** Allows keys to be provisioned using ECDHE key agreement with authentication via specified key slot.

## Applicative Scenarios

Symmetric key provisioning control field configuration is essential for:

- **Production Security Policy:** Define secure key provisioning methods during device personalization.
- **Key Lifecycle Management:** Control how symmetric keys can be updated or replaced throughout device lifetime.
- **Zero-Touch Provisioning:** Enable secure remote key provisioning using ECDHE or wrapped methods.
- **Multi-Stage Provisioning:** Allow different provisioning methods at different stages of device lifecycle (manufacturing, field deployment, updates).
- **Compliance Requirements:** Meet security standards by enforcing encrypted key transfer methods.

**Recommended Settings for Production:**
- Disable plaintext provisioning
- Enable wrapped provisioning with authentication
- Enable ECDHE provisioning for zero-touch scenarios
- Disable change right after configuration to lock settings permanently

**WARNING:** This is a permanent configuration operation. Once the change right is disabled, control fields cannot be modified. Carefully plan and test your provisioning strategy before deploying to production devices.
