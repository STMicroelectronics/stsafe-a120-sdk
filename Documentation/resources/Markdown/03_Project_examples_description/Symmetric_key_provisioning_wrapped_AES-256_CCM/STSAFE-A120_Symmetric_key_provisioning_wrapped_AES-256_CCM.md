# STSAFE-A120 Wrapped Symmetric Key Provisioning - AES-256 CCM* {#STSAFE-A120_Symmetric_key_provisioning_wrapped_AES-256_CCM}

This example demonstrates how to securely provision an AES-256 symmetric key into the STSAFE-A120 secure element using the wrapped envelope method, and then use that key to perform authenticated encryption using CCM* (Counter with CBC-MAC) mode.

Wrapped key provisioning protects the symmetric key during transfer by encrypting it with a Key Encryption Key (KEK) derived from the host secure session. This ensures the key is never exposed in plaintext on the communication bus, providing production-grade security for key management.

## Example Flowchart

The following flowchart illustrates the wrapped key provisioning and CCM* operation:

@startuml{Symmetric_key_provisioning_wrapped_AES-256_CCM_flowchart.png} "Wrapped AES-256 Key Provisioning CCM flowchart" width=5cm
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
	:Wrap AES-256 key using KEK from session;
	:Provision wrapped key to symmetric key slot;
	:Configure key for CCM* mode of operation;
	:Generate test message and nonce;
	:Encrypt message using CCM* (AEAD);
	:Decrypt encrypted message using CCM*;
	:Compare decrypted with original message;
	if(Messages match?) then (Yes)
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

Upon execution, the host terminal displays the provisioning and CCM* encryption/decryption:

```
----------------------------------------------------------------------------------------------------------------
-              STSAFE-A120 Symmetric key cryptography and key provisioning wrapped example                     -
-                                              AES 256 CCM*                                                    -
----------------------------------------------------------------------------------------------------------------

 - Initialize target STSAFE-A120
 - Establish host secure session

 - Wrap AES-256 key using session KEK
 - Provision wrapped key to slot 0

 - Key provisioned successfully

 - Test message (plaintext):
  0x00 0x01 0x02 0x03 0x04 0x05 0x06 0x07 0x08 0x09 0x0A 0x0B 0x0C 0x0D 0x0E 0x0F

 - Encrypt using CCM* mode
 - Encrypted message:
  0xA1 0xB2 0xC3 0xD4 0xE5 0xF6 0x07 0x18 0x29 0x3A 0x4B 0x5C 0x6D 0x7E 0x8F 0x90

 - Authentication tag (4 bytes):
  0x11 0x22 0x33 0x44

 - Decrypt using CCM* mode
 - Decrypted message:
  0x00 0x01 0x02 0x03 0x04 0x05 0x06 0x07 0x08 0x09 0x0A 0x0B 0x0C 0x0D 0x0E 0x0F

 - Decryption and authentication verification: SUCCESS
```

## CCM* Overview

CCM* (Counter with CBC-MAC) is an authenticated encryption with associated data (AEAD) mode that provides both confidentiality and authenticity:

- **Encryption:** Uses CTR (Counter) mode for confidentiality
- **Authentication:** Uses CBC-MAC for message authentication
- **AEAD:** Authenticates both the ciphertext and optional additional data
- **Nonce-based:** Requires a unique nonce for each encryption operation
- **Authentication Tag:** Produces a MAC tag to verify data integrity and authenticity

Key features:
- Single-pass operation for both encryption and authentication
- Configurable authentication tag length (4-16 bytes)
- Suitable for IoT and resource-constrained devices
- Widely used in wireless protocols (IEEE 802.15.4, Zigbee, Thread)

## Applicative Scenarios

Wrapped symmetric key provisioning with CCM* encryption serves several use cases:

- **Secure Firmware Updates:** Encrypt and authenticate firmware images for secure over-the-air updates.
- **IoT Data Protection:** Protect sensor data with encryption and authentication in IoT devices.
- **Secure Communication Protocols:** Implement secure channels for device-to-device or device-to-cloud communication.
- **Configuration Protection:** Encrypt and authenticate device configuration files to prevent tampering.
- **Secure Storage:** Protect sensitive data stored in external memory with authenticated encryption.
- **Key Distribution:** Securely distribute additional keys or credentials in encrypted and authenticated form.

## Security Advantages

Wrapped key provisioning with CCM* offers comprehensive security:

- **Key Protection:** Symmetric key never exposed in plaintext during provisioning.
- **Confidentiality:** CCM* encryption protects data from eavesdropping.
- **Authenticity:** Authentication tag ensures data hasn't been tampered with.
- **Integrity:** Any modification to ciphertext or associated data is detected.
- **Replay Protection:** Nonce-based operation prevents replay attacks when managed properly.
- **Production-Ready:** Suitable for deployment in security-critical production environments.

**Recommended Practice:** 
- Always use wrapped or ECDHE-based key provisioning for production
- Ensure unique nonces for each CCM* encryption operation
- Use adequate authentication tag length (at least 8 bytes for high-security applications)
- Protect nonce generation to prevent nonce reuse
