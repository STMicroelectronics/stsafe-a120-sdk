# STSAFE-A120 secret or key wrap/unwrap{#STSAFE-A120_wrap_unwrap}

This example demonstrates how how to wrap/unwrap payload using a target STSAFE-A120 device

The example applicative flowchart is illustrated below :

@startuml{STSAFE-A120_wrap_unwrap.png} "STSAFE-A120_wrap_unwrap Example flowchart" width=5cm

	:MAIN;
	:Initialize Apps terminal (baudrate = 115200)]
	
	:Print example title and instructions]
	
	:ret = <b>stse_init</b>|
	if(ret != STSE_OK) then (No)
	else (Yes)
		:Print ERROR]
		-[hidden]->
		detach
	endif

	:ret = <b>stsafea_generate_wrap_unwrap_key</b>|
	if((ret != STSE_OK) && (ret != STSE_ACCESS_CONDITION_NOT_SATISFIED)) then (yes)
		:Print ERROR]
		-[hidden]->
		detach
	(No) else if (ret != STSE_ACCESS_CONDITION_NOT_SATISFIED) then (Yes)
		:Print "New key provisioned"]
	else(No)
	endif
	
	:Print plain-text payload]

	:<b>stsafea_session_handler_allocate</b>|

	:ret = <b>stsafea_open_host_session</b>|
	if(ret != STSE_OK) then (No)
	else (Yes)
		:Print ERROR]
		-[hidden]->
		detach
	endif	

	:ret = <b>stsafea_wrap_payload</b> (key 0 , plain-text payload) |
	if(ret != STSE_OK) then (No)
	else (Yes)
		:Print ERROR]
		-[hidden]->
		detach
	endif	

	:ret = <b>stsafea_wrap_payload</b> (key 0 , plain-text payload) |
	if(ret != STSE_OK) then (No)
	else (Yes)
		:Print ERROR]
		-[hidden]->
		detach
	endif	

	:Print wrapped payload]


	:ret = <b>stsafea_unwrap_payload</b> (key 0 , wrapped payload) |
	if(ret != STSE_OK) then (No)
	else (Yes)
		:Print ERROR]
		-[hidden]->
		detach
	endif	

	:Print un-wrapped payload]

	while (while(1) )
	end while
	-[hidden]->
    detach
@enduml

API and services used in the example :

- stse_init
- stsafea_generate_wrap_unwrap_key
- stsafea_session_handler_allocate
- stsafea_wrap_payload
- stsafea_unwrap_payload


When executed following logs can be seen on the host attached computer terminal window \n


```
----------------------------------------------------------------------------------------------------------------
-                     STSAFE-A120 Wrap/Unwrap example                            -
----------------------------------------------------------------------------------------------------------------
-
- description :
- This examples illustrates how to wrap/unwrap payload using a target STSAFE-A120 device
-
----------------------------------------------------------------------------------------------------------------
 - Plain-text buffer to be wrapped  :
  0x00 0x01 0x02 0x03 0x04 0x05 0x06 0x07 0x08 0x09 0x0A 0x0B 0x0C 0x0D 0x0E 0x0F

 - Wrapped buffer  :

  0x76 0x90 0xDC 0xB0 0xD8 0x43 0x2C 0xD7 0xEE 0x53 0x3B 0x9C 0x0E 0x9B 0xA7 0x5A
  0x43 0xA8 0x2B 0x4C 0xBA 0x6F 0xA7 0xE1

 - Un-wrapped buffer  :

  0x00 0x01 0x02 0x03 0x04 0x05 0x06 0x07 0x08 0x09 0x0A 0x0B 0x0C 0x0D 0x0E 0x0F

```

Applicative scenario : 

- storage of secret information on host
- pereparation of secret information to be store in STSAFE-A device 