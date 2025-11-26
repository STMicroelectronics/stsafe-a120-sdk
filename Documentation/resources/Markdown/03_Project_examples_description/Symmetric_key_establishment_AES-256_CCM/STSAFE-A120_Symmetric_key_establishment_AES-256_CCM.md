# STSAFE-A120 Symmetric Key Establishment AES-256 CCM {#STSAFE-A120_Symmetric_key_establishment_AES-256_CCM}

This example demonstrates a symmetric key (AES-256 CCM) establishment and usage in STSAFE-A1xx Secure Element.
It uses the STSAFE Middleware APIs to a establish a key and encrypt then decrypt a message.

The example applicative flowchart is illustrated below :

@startuml{STSAFE-A120_Symmetric_key_establishment_AES-256_CCM.png} "STSAFE-A120_Symmetric_key_establishment_AES-256_CCM Example flowchart" width=5cm
	:MAIN;
	:Initialize UART (baudrate = 115200)]
	:Print example title and instructions]
	:Initialize STSAFE device]
    :Ask user to enter a slot number]
	:Establish AES-256 CCM* key in selected slot]
	:Encrypt message with the STSAFE-A API]
	:Decrypt message with the STSAFE-A Platform]

	while (while(1) )
	end while
	-[hidden]->
    detach
@enduml

When executed following logs can be seen on the host attached computer terminal window \n

```
----------------------------------------------------------------------------------------------------------------
-                  STSAFE-A120 Symmetric key cryptography and key establishment example                        -
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

 - Establish symmetric key in slot 0

 - Established key :

  0x5D 0x86 0x6C 0x2F 0xDF 0x7D 0xE4 0x6B 0x97 0xBE 0xA9 0xC7 0xA5 0x13 0x7C 0xC0
  0x52 0x3B 0x27 0xCF 0xED 0x33 0xB8 0x0B 0xE1 0x1F 0x0A 0xF9 0x9C 0x5D 0xEC 0x0C

 - Encrypted Message :

  0x6A 0x21 0xED 0x44 0xB4 0x97 0x3A 0x82 0xE3 0xC4 0x1D 0xE8 0x6B 0x88 0x81 0xB0

 - Authentication tag :

  0x95 0x56 0x35 0x43

 - Decrypted Message :

  0x00 0x01 0x02 0x03 0x04 0x05 0x06 0x07 0x08 0x09 0x0A 0x0B 0x0C 0x0D 0x0E 0x0F
```


Applicative scenario : 

- Secure key provisionning with key establishment
- Symmetric encryption and MAC