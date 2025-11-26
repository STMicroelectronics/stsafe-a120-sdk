# STSAFE-A120 Symmetric Key Management AES-256 CCM {#STSAFE-A120_Symmetric_key_management_AES-256_CCM}

This example demonstrates a symmetric key (AES-256 CCM) provisionning and usage in STSAFE-A1xx Secure Element.
It uses the STSAFE Middleware APIs to a write a key and encrypt then decrypt a message.

The example applicative flowchart is illustrated below :

@startuml{STSAFE-A120_Symmetric_key_management_AES-256_CCM.png} "STSAFE-STSAFE-A120_Symmetric_key_management_AES-256_CCM Example flowchart" width=5cm
	:MAIN;
	:Initialize UART (baudrate = 115200)]
	:Print example title and instructions]
	:Initialize STSAFE device]
    :Ask user to enter a slot number]
	:Write AES-256 CCM* key in selected slot]
	:Encrypt message with the STSAFE-A API]
	:Decrypt message with the STSAFE-A API]

	while (while(1) )
	end while
	-[hidden]->
    detach
@enduml

When executed following logs can be seen on the host attached computer terminal window \n

```
----------------------------------------------------------------------------------------------------------------
-                   STSAFE-A120 Symmetric key cryptography and key management example                          -
-                                              AES 256 CCM*                                                    -
----------------------------------------------------------------------------------------------------------------
-
- description :
- This examples illustrates how to provision symmetric keys in STSAFE-A120 secure element
- and perform AES cryptographic operations
-
- Note : Device with accessible symmetric key (i.e. STSAFE-A120 SPL04) is required to run this example
-
----------------------------------------------------------------------------------------------------------------

 - Initialize target device


 -------------------------------------+-----------------------------------------+---------------------------------------------------+---------------------------
                                      |                 KEY USAGE               |                     PARAMETERS                    | PROVISIONNING CTRL FIELDS
 ------+------+----------+------------+---------+-----------+---------+---------+-------------+--------------+------------+---------+---------+---------+-------
  SLOT | LOCK | KEY TYPE | MODE OF OP | MAC GEN | MAC VERIF | ENCRYPT | DECRYPT | MIN MAC LEN | AUTH TAG LEN | CNT OFFSET | CNT VAL | PUT KEY | WRAPPED | ECDHE
 ------+------+----------+------------+---------+-----------+---------+---------+-------------+--------------+------------+---------+---------+---------+-------
   00  |   -  |    -     |     -      |    -    |     -     |    -    |    -    |      -      |      -       |     -      |    -    |    -    |  0x00   | 0x00
   01  |   -  |    -     |     -      |    -    |     -     |    -    |    -    |      -      |      -       |     -      |    -    |    -    |  0x00   | 0x00
   02  |   -  |    -     |     -      |    -    |     -     |    -    |    -    |      -      |      -       |     -      |    -    |    -    |  0x00   | 0x00
   03  |   -  |    -     |     -      |    -    |     -     |    -    |    -    |      -      |      -       |     -      |    -    |    -    |  0x00   | 0x00
   04  |   -  |    -     |     -      |    -    |     -     |    -    |    -    |      -      |      -       |     -      |    -    |    -    |  0x00   | 0x00
   05  |   -  |    -     |     -      |    -    |     -     |    -    |    -    |      -      |      -       |     -      |    -    |    -    |  0x00   | 0x00
   06  |   -  |    -     |     -      |    -    |     -     |    -    |    -    |      -      |      -       |     -      |    -    |    -    |  0x00   | 0x00
   07  |   -  |    -     |     -      |    -    |     -     |    -    |    -    |      -      |      -       |     -      |    -    |    -    |  0x00   | 0x00
   08  |   -  |    -     |     -      |    -    |     -     |    -    |    -    |      -      |      -       |     -      |    -    |    -    |  0x00   | 0x00
   09  |   -  |    -     |     -      |    -    |     -     |    -    |    -    |      -      |      -       |     -      |    -    |    -    |  0x00   | 0x00
   10  |   -  |    -     |     -      |    -    |     -     |    -    |    -    |      -      |      -       |     -      |    -    |    -    |  0x00   | 0x00
   11  |   -  |    -     |     -      |    -    |     -     |    -    |    -    |      -      |      -       |     -      |    -    |    -    |  0x00   | 0x00
   12  |   -  |    -     |     -      |    -    |     -     |    -    |    -    |      -      |      -       |     -      |    -    |    -    |  0x00   | 0x00
   13  |   -  |    -     |     -      |    -    |     -     |    -    |    -    |      -      |      -       |     -      |    -    |    -    |  0x00   | 0x00
   14  |   -  |    -     |     -      |    -    |     -     |    -    |    -    |      -      |      -       |     -      |    -    |    -    |  0x00   | 0x00
   15  |   -  |    -     |     -      |    -    |     -     |    -    |    -    |      -      |      -       |     -      |    -    |    -    |  0x00   | 0x00

==> Enter the slot number to write :

 - Write symmetric key in slot 0

 - Encrypted Message :

  0x69 0x66 0x30 0xBB 0x08 0x0F 0xE4 0x1D 0x61 0x04 0x89 0xD3 0xDA 0x23 0x21 0x66

 - Authentication tag :

  0xDA 0xA9 0xB5 0x70

 - Decrypted Message :

  0x00 0x01 0x02 0x03 0x04 0x05 0x06 0x07 0x08 0x09 0x0A 0x0B 0x0C 0x0D 0x0E 0x0F
```


Applicative scenario : 

- Secure key provisionning with key injection
- Symmetric encryption and MAC