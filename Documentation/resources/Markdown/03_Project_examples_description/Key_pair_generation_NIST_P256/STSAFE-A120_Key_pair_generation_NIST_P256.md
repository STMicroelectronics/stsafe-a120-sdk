# STSAFE-A120 NIST P-256 Key Pair Generation {#STSAFE-A120_Key_pair_generation_NIST_P256}

This example demonstrates how to generate an ECDSA (Elliptic curve Digital Signature Algorithm) key pair using the NIST P-256 curve on the STSAFE-A120 secure element.  
Elliptic curve (ECC) is widely used for digital signatures, key exchange, and other cryptographic operations in modern security applications.

The NIST P-256 curve (also known as secp256r1 or prime256v1) is a standardized elliptic curve recommended by NIST for cryptographic use.  
It provides 128-bit security with relatively small key sizes (256 bits), making it suitable for resource-constrained embedded systems.

## Example Flowchart

The following flowchart illustrates the NIST P-256 key pair generation and signature verification process:

@startuml{Key_pair_generation_NIST_P256_flowchart.png} "NIST P-256 Key Pair Generation flowchart" width=5cm
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
	:Generate NIST P-256 key pair in selected slot;
	:Retrieve the generated public key;
	:Create random message and compute SHA-256 hash;
	:Generate ECDSA signature using private key;
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
-                              STSAFE-A120 NIST-P256 key pair generation                                       -
----------------------------------------------------------------------------------------------------------------
- This example demonstrates how to generate and use a NIST-P256 key pair on a target STSAFE-A120 device        -
----------------------------------------------------------------------------------------------------------------

 - Initialize target STSAFE-A120

 - Query asymmetric key table
 - Generate NIST-P256 key pair in slot 0x00

 - Public Key (X coordinate):
  0x1A 0x2B 0x3C 0x4D 0x5E 0x6F 0x7A 0x8B 0x9C 0xAD 0xBE 0xCF 0xD0 0xE1 0xF2 0x03
  0x14 0x25 0x36 0x47 0x58 0x69 0x7A 0x8B 0x9C 0xAD 0xBE 0xCF 0xD0 0xE1 0xF2 0x03

 - Public Key (Y coordinate):
  0xA1 0xB2 0xC3 0xD4 0xE5 0xF6 0x07 0x18 0x29 0x3A 0x4B 0x5C 0x6D 0x7E 0x8F 0x90
  0xA1 0xB2 0xC3 0xD4 0xE5 0xF6 0x07 0x18 0x29 0x3A 0x4B 0x5C 0x6D 0x7E 0x8F 0x90

 - STSAFE-A120 verify signature : success
```

## Applicative Scenarios

The NIST P-256 key pair generation capability serves several important use cases:

- **Device Identity and Authentication:** Generate unique device identity keys for secure authentication in IoT networks and cloud services.
- **Digital Signatures:** Sign firmware updates, configuration files, or messages to ensure integrity and authenticity.
- **Key Exchange:** Perform ECDH (Elliptic Curve Diffie-Hellman) key exchange to establish secure communication channels.
- **Certificate Generation:** Create device certificates for TLS/SSL or other PKI-based security protocols.
- **Secure Boot:** Sign and verify bootloader and firmware images during the secure boot process.

The STSAFE-A120's hardware-based key generation ensures that private keys are created and stored securely within the device, never exposed to the host system, providing strong protection against key extraction attacks.
