# STSAFE-A120 Symmetric Key Management AES-128 CMAC {#STSAFE-A120_Symmetric_key_management_AES-128_CMAC}

This example demonstrates a symmetric key (AES-128 CMAC) provisionning and usage in STSAFE-A1xx Secure Element.
It uses the STSAFE Middleware APIs to a write a key and compute then verify a MAC.

The example applicative flowchart is illustrated below :

@startuml{STSAFE-A120_Symmetric_key_management_AES-128_CMAC.png} "STSAFE-A120_Symmetric_key_management_AES-128_CMAC Example flowchart" width=5cm
	:MAIN;
	:Initialize UART (baudrate = 115200)]
	:Print example title and instructions]
	:Initialize STSAFE device]
    :Ask user to enter a slot number]
	:Write AES-128 CMAC key in selected slot]
	:Compute MAC over message with the STSAFE-A API]
	:Verify MAC over message with the STSAFE-A API]

	while (while(1) )
	end while
	-[hidden]->
    detach
@enduml

When executed following logs can be seen on the host attached computer terminal window \n

```
----------------------------------------------------------------------------------------------------------------
-                   STSAFE-A120 Symmetric key cryptography and key management example                          -
-                                              AES 128 CMAC                                                    -
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

 - Encrypted Message MAC :

  0xBE 0x4C 0xA3 0xF2

 - Encrypted Message MAC verification : PASS
```


Applicative scenario : 

- Secure key provisionning with key injection
- Symmetric message authentication (MAC)