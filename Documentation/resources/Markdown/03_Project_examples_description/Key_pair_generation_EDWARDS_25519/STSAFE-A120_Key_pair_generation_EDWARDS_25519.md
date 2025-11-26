# STSAFE-A120 Edwards-25519 Key Pair Generation {#STSAFE-A120_Key_pair_generation_EDWARDS_25519}

This example demonstrates how to generate an EdDSA (Edwards curve Digital Signature Algorithm) key pair using the Edwards-25519 curve on the STSAFE-A120 secure element.  
Ed25519 is a modern signature scheme that offers high security, excellent performance, and small key and signature sizes.

The Edwards-25519 curve (also known as Ed25519) is based on the Curve25519 elliptic curve and provides approximately 128-bit security.  
It is designed to be fast, secure against timing attacks, and requires no random number generation during the signing process, making it particularly suitable for embedded and IoT applications.

## Example Flowchart

The following flowchart illustrates the Ed25519 key pair generation and signature verification process:

@startuml{Key_pair_generation_EDWARDS_25519_flowchart.png} "Edwards-25519 Key Pair Generation flowchart" width=5cm
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
	:Query asymmetric key table to find available slot;
	:Generate Edwards-25519 key pair in selected slot;
	:Retrieve the generated public key;
	:Create random message and compute SHA-256 hash;
	:Generate EdDSA signature using private key;
	:Verify signature using platform crypto library;
	if(Signature valid?) then (Yes)
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

Upon execution, the host terminal displays the key generation process and signature verification results:

```
----------------------------------------------------------------------------------------------------------------
-                              STSAFE-A120 EDWARDS-25519 key pair generation                                   -
----------------------------------------------------------------------------------------------------------------
- This example demonstrates how to generate and use a EDWARDS-25519 key pair on a target STSAFE-A120 device    -
----------------------------------------------------------------------------------------------------------------

 - Initialize target STSAFE-A120

 - Query asymmetric key table
 - Generate Edwards-25519 key pair in slot 0x00

 - Public Key:
  0x1A 0x2B 0x3C 0x4D 0x5E 0x6F 0x7A 0x8B 0x9C 0xAD 0xBE 0xCF 0xD0 0xE1 0xF2 0x03
  0x14 0x25 0x36 0x47 0x58 0x69 0x7A 0x8B 0x9C 0xAD 0xBE 0xCF 0xD0 0xE1 0xF2 0x03

 - Generate random message and compute hash
 - Generate EdDSA signature
 - Signature:
  0xA1 0xB2 0xC3 0xD4 0xE5 0xF6 0x07 0x18 0x29 0x3A 0x4B 0x5C 0x6D 0x7E 0x8F 0x90
  0xA1 0xB2 0xC3 0xD4 0xE5 0xF6 0x07 0x18 0x29 0x3A 0x4B 0x5C 0x6D 0x7E 0x8F 0x90
  0xA1 0xB2 0xC3 0xD4 0xE5 0xF6 0x07 0x18 0x29 0x3A 0x4B 0x5C 0x6D 0x7E 0x8F 0x90
  0xA1 0xB2 0xC3 0xD4 0xE5 0xF6 0x07 0x18 0x29 0x3A 0x4B 0x5C 0x6D 0x7E 0x8F 0x90

 - STSAFE-A120 verify signature : success
```

## Applicative Scenarios

The Edwards-25519 key pair generation capability serves several important use cases:

- **Modern Authentication Protocols:** Ed25519 is increasingly used in modern authentication systems like SSH, TLS 1.3, and blockchain technologies.
- **Digital Signatures for IoT:** Small signature size (64 bytes) and fast verification make Ed25519 ideal for bandwidth-constrained IoT devices.
- **Secure Firmware Signing:** Sign firmware updates with compact signatures that can be verified quickly on resource-limited devices.
- **Cryptocurrency and Blockchain:** Ed25519 is commonly used in blockchain applications for transaction signing and wallet operations.
- **High-Performance Applications:** Ed25519's deterministic signature generation (no random number required) and fast verification make it suitable for high-throughput systems.

The STSAFE-A120's hardware-based Ed25519 key generation ensures that private keys are created and stored securely within the secure element, providing strong protection against side-channel attacks and key extraction attempts. The algorithm's resistance to timing attacks makes it particularly suitable for security-critical applications.
