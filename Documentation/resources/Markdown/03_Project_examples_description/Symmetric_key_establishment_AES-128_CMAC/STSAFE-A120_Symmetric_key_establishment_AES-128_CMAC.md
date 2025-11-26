# STSAFE-A120 Symmetric Key Establishment AES-128 CMAC {#STSAFE-A120_Symmetric_key_establishment_AES-128_CMAC}

This example demonstrates a symmetric key (AES-128 CMAC) establishment and usage in STSAFE-A1xx Secure Element.
It uses the STSAFE Middleware APIs to a establish a key and compute then verify a MAC.

The example applicative flowchart is illustrated below :

@startuml{STSAFE-A120_Symmetric_key_establishment_AES-128_CMAC.png} "STSAFE-A120_Symmetric_key_establishment_AES-128_CMAC Example flowchart" width=5cm
	:MAIN;
	:Initialize UART (baudrate = 115200)]
	:Print example title and instructions]
	:Initialize STSAFE device]
    :Ask user to enter a slot number]
	:Establish AES-128 CMAC key in selected slot]
	:Compute MAC over message with the STSAFE-A API]
	:Verify MAC over message with the STSAFE-A Platform]

	while (while(1) )
	end while
	-[hidden]->
    detach
@enduml

When executed following logs can be seen on the host attached computer terminal window \n

```
----------------------------------------------------------------------------------------------------------------
-                   STSAFE-A120 Symmetric key cryptography and key establishment example                          -
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

 - Establish symmetric key in slot 0

 - Established key :

  0xB9 0x33 0xC5 0x61 0xA9 0xBF 0x3B 0x3A 0x7F 0x17 0x60 0xFC 0xF6 0xD2 0xCD 0x8E
  0x00 0x00 0x00 0x00 0x00 0x01 0x02 0x03 0x04 0x05 0x06 0x07 0x08 0x09 0x0A 0x0B

 - Encrypted Message MAC :

  0xFD 0xDF 0xB4 0xC1

 - Encrypted Message MAC verification : PASS
```


Applicative scenario : 

- Secure key provisionning with key establishment
- Symmetric message authentication (MAC)