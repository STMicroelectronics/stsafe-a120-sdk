# STSAFE-A120 Hash{#STSAFE-A120_Hash}

This example demonstrates how to performs xxx . 

![STSAFE-A120_Hash](../../../../Documentation/resources/pictures/STSAFE-A120_Hash.png)

The example applicative flowchart is illustrated below :

@startuml{STSAFE-A120_Hash.png} "STSAFE-A120_Device_authentication Example flowchart" width=5cm
	:MAIN;
	: Initialize apps terminal]
	:Initialize STSAFE device (addr 0x20) ]
	while (while(1))
	end while
	-[hidden]->
	detach
@enduml

The following diagram shows the interactions between host and target STSAFE device :

@startuml{STSAFE-A120_Get_product_information.png} "STSAFE-A120 Get product information Interactions Diagram" width=5cm
	skinparam sequenceMessageAlign center
	participant HOST order 1
	participant STSAFE order 2
	rnote over HOST: Initialize STSAFE device handler\n (STSAFE-A120 ; addr 0x20)
@enduml


When executed following logs can be seen on the host attached computer terminal window \n


```

```


Applicative scenario : 

- ...
- ...