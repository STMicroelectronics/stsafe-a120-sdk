# STSAFE-A120 Secure Data Storage Counter Access {#STSAFE-A120_secure_data_storage_counter_access}

The STSAFE-A120 is a secure element designed to provide robust data storage and counter management capabilities. In this example, you will learn how to:

- Retrieve the total number of data partitions configured on the STSAFE-A120 device.
- Access detailed information about each partition, including counter support, data segment size, and access control policies.
- Read the value and associated data from a specific counter zone (zone 5).
- Update (decrement) the counter in zone 5, demonstrating secure usage tracking.

These operations are essential for applications requiring secure monitoring of consumable usage, lifecycle management, or tamper-resistant counters.

![STSAFE-A110_secure_data_storage_counter_access_example](./secure_data_storage_counter_access_example.png)

> **Note:** The zone IDs referenced in this example correspond to the STSAFE-A120 SPL05 personalization. If your device is personalized differently, you must adapt the access parameters accordingly.

## Example Flowchart

The following flowchart illustrates the main steps of the example application, from initialization to secure counter access and update:

@startuml{STSAFE-A120_secure_data_storage_zone_access.png} "STSAFE-A120_Device_authentication Example flowchart" width=5cm

	:MAIN;
	:Initialize Apps terminal (baudrate = 115200)];
	:Print example title and instructions];
	:ret = <b>stse_init</b>|;
	if(ret != STSE_OK) then (No)
	else (Yes)
		:Print ERROR];
		-[hidden]->
		detach
	endif

	:Print data partition record table];

	:ret = <b>stse_data_storage_read_counter</b> (
	\tzone 5 , 
	\toffset 0 , 
	\tsize of associated buffer
	)|;
	if(ret != STSE_OK) then (No)
	else (Yes)
		:Print ERROR];
		-[hidden]->
		detach
	endif

	:Print counter value and associated data];

	:Randomize associated data buffer];

	:ret = <b>stse_data_storage_decrement_counter</b> (
	\tzone 5 , 
	\amount 1,
	\toffset 0 , 
	\tassociated data buffer,
	\tsize of associated buffer
	)|;
	if(ret != STSE_OK) then (No)
	else (Yes)
		:Print ERROR];
		-[hidden]->
		detach
	endif
	:Print new counter value ];

	:ret = <b>stse_data_storage_read_counter</b> (
	\tzone 5 , 
	\toffset 0 , 
	\tsize of associated buffer
	)|;
	if(ret != STSE_OK) then (No)
	else (Yes)
		:Print ERROR];
		-[hidden]->
		detach
	endif

	:Print counter value and associated data];

	while (while(1) )
	end while
	-[hidden]->
	detach
@enduml

## STSELib APIs Utilized

The following STSAFE-A120 APIs are used in this example:

- `stse_init`: Initializes the STSAFE-A120 device and prepares it for data storage operations.
- `stse_data_storage_read_counter`: Reads the current value and associated data from a specified counter zone.
- `stse_data_storage_decrement_counter`: Securely decrements the counter in a specified zone and updates the associated data.

## Example Output

When the example is executed, the host computer terminal displays detailed logs that reflect each step of the process, including partition configuration, counter reads, and updates. Below is a sample output:

```
----------------------------------------------------------------------------------------------------------------
-                             STSAFE-A120 secure data storage counter access example                           -
----------------------------------------------------------------------------------------------------------------
-
- Description:
- This example illustrates how to leverage the STSAFE-A120 data storage APIs to perform secure operations on
- the device, including querying partition information and managing counter zones.
-          o Query STSAFE-A total partition count
-          o Query STSAFE-A partitions information
-          o Read STSAFE-A counter zone 5
-          o Update STSAFE-A counter zone 5
-
- Note: Zone IDs used in this example are aligned with STSAFE-A120 SPL05 personalization.
-       Access parameters must be adapted for other device personalization.
-
----------------------------------------------------------------------------------------------------------------

- Initialize peripheral STSAFE:

 - stse_get_data_partitions_configuration
  ID | COUNTER | DATA SEGMENT SIZE | READ AC CR |  READ AC | UPDATE AC CR | UPDATE AC |  COUNTER VAL
 000 |    x    |       1000        |    DENIED  |  ALWAYS  |    ALLOWED   |  NEVER    | 000000
 001 |    x    |       0700        |    DENIED  |  ALWAYS  |    ALLOWED   | ALWAYS    | 000000
 002 |    x    |       0600        |    DENIED  |  ALWAYS  |    ALLOWED   | ALWAYS    | 000000
 003 |    x    |       0600        |    DENIED  |  ALWAYS  |    ALLOWED   | ALWAYS    | 000000
 004 |    x    |       1696        |    DENIED  |  ALWAYS  |    ALLOWED   | ALWAYS    | 000000
 005 |    -    |       0064        |    DENIED  |  ALWAYS  |    ALLOWED   | ALWAYS    | 499990
 006 |    -    |       0064        |    DENIED  |  ALWAYS  |    ALLOWED   | ALWAYS    | 500000
 007 |    x    |       1578        |    DENIED  |  ALWAYS  |    ALLOWED   | ALWAYS    | 000000

 - stsafe_data_storage_read_zone (zone: 05; length: 16)
		 o Associated Data:
  0x7D 0x32 0x09 0x10 0x6F 0x65 0x31 0x3A 0x34 0xDF 0xBF 0xF3 0x38 0x36 0xCE 0xE8
		 o Counter Value: 499990

 - stsafe_data_storage_read_zone (zone: 05; length: 16)
		 o New Associated Data:
  0x8B 0x6E 0xD2 0xB4 0x96 0xCC 0x5B 0xDB 0xE9 0x37 0xED 0x86 0x42 0xEB 0xD2 0x89
		 o New Counter Value: 499989

 - stsafe_data_storage_read_zone (zone: 05; length: 16)
		 o Associated Data:
  0x8B 0x6E 0xD2 0xB4 0x96 0xCC 0x5B 0xDB 0xE9 0x37 0xED 0x86 0x42 0xEB 0xD2 0x89
		 o Counter Value: 499989
```

## Applicative Scenarios

The secure counter and data storage features demonstrated in this example are ideal for:

- **Accessory/Consumable Usage Monitoring:** Track the usage of replaceable components or consumables in a secure, tamper-resistant manner.
- **Accessory/Consumable Life/Cycle Management:** Monitor and enforce lifecycle policies for accessories, ensuring compliance and safety.

By following this example, developers can implement robust, secure usage tracking and data partition management in their own STSAFE-A120-based solutions.