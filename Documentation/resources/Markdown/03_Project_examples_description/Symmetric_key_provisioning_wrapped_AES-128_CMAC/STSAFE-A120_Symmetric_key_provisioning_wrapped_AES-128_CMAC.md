# STSAFE-A120 Wrapped Symmetric Key Provisioning - AES-128 CMAC {#STSAFE-A120_Symmetric_key_provisioning_wrapped_AES-128_CMAC}

This example demonstrates how to securely provision an AES-128 symmetric key into the STSAFE-A120 secure element using the wrapped envelope method, and then use that key to compute CMAC (Cipher-based Message Authentication Code) on a test message.

Wrapped key provisioning provides a secure method for transferring symmetric keys to the STSAFE-A120 without exposing them in plaintext on the communication bus. The key is encrypted using a Key Encryption Key (KEK) derived from the host secure session, ensuring end-to-end protection during the provisioning process.

## Example Flowchart

The following flowchart illustrates the wrapped key provisioning and CMAC operation:

@startuml{Symmetric_key_provisioning_wrapped_AES-128_CMAC_flowchart.png} "Wrapped AES-128 Key Provisioning CMAC flowchart" width=5cm
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
	:Establish host secure session;
	:Wrap AES-128 key using KEK from session;
	:Provision wrapped key to symmetric key slot;
	:Configure key for CMAC mode of operation;
	:Generate test message;
	:Compute CMAC using provisioned key;
	:Verify CMAC using platform crypto library;
	if(CMAC valid?) then (Yes)
	:Display success message;
	else (No)
	:Display error message;
	endif
	while (while(1))
	end while
	-[hidden]->
	detach
	endif
@enduml

## Host Terminal Output

Upon execution, the host terminal displays the provisioning and CMAC computation:

```
----------------------------------------------------------------------------------------------------------------
-              STSAFE-A120 Symmetric key cryptography and key provisioning wrapped example                     -
-                                              AES 128 CMAC                                                    -
----------------------------------------------------------------------------------------------------------------

 - Initialize target STSAFE-A120
 - Establish host secure session

 - Wrap AES-128 key using session KEK
 - Provision wrapped key to slot 0

 - Key provisioned successfully

 - Test message:
  0x00 0x01 0x02 0x03 0x04 0x05 0x06 0x07 0x08 0x09 0x0A 0x0B 0x0C 0x0D 0x0E 0x0F

 - Compute CMAC using STSAFE-A120
 - CMAC (4 bytes):
  0xA1 0xB2 0xC3 0xD4

 - Verify CMAC using platform crypto: SUCCESS
```

## CMAC Overview

CMAC (Cipher-based Message Authentication Code) is a block cipher-based MAC algorithm that provides message authentication and integrity verification. Key characteristics:

- Based on AES block cipher in CBC mode
- Provides strong authentication guarantees
- Fixed output size (configurable between 4-16 bytes)
- Suitable for messages of any length
- Widely used in security protocols and standards

## Applicative Scenarios

Wrapped symmetric key provisioning and CMAC operations serve several use cases:

- **Secure Key Distribution:** Distribute symmetric keys to field devices without exposing them during transfer.
- **Message Authentication:** Verify integrity and authenticity of firmware updates, configuration files, or data messages.
- **Secure Firmware Update:** Authenticate firmware images before installation using pre-provisioned keys.
- **API Request Authentication:** Protect API calls and commands with CMAC-based authentication.
- **Data Integrity Protection:** Ensure data hasn't been tampered with during storage or transmission.
- **Multi-Device Key Management:** Provision shared keys across multiple devices for secure inter-device communication.

## Security Advantages

Wrapped key provisioning offers several security benefits over plaintext provisioning:

- **No Plaintext Exposure:** The symmetric key is never transmitted in plaintext on the communication bus.
- **Secure Channel:** Uses the host secure session's KEK to protect the key during transfer.
- **Production-Ready:** Suitable for deployment in production environments where key security is critical.
- **Side-Channel Resistance:** Reduces risk of key extraction through bus monitoring or side-channel attacks.
- **Compliance:** Meets security requirements for key management in regulated industries.

**Recommended Practice:** Always use wrapped or ECDHE-based key provisioning for production deployments. Plaintext key provisioning should only be used during development and testing.
