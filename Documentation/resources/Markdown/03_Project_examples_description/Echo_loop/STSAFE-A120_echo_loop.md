# STSAFE-A120 Echo Loop {#STSAFE-A120_echo_loop}

This example provides a comprehensive demonstration of the echo loop functionality between a Host system and a target STSAFE-A120 device. The echo loop is a fundamental mechanism used to verify communication integrity, device responsiveness, and data consistency between the host and the secure element (SE). By continuously sending randomized messages from the host to the STSAFE-A120 and receiving the echoed responses, users can validate both the hardware and software integration of the SE in real-world scenarios.

![STSAFE-A120_echo_loop_example](./STSAFE-A120_Echo_loop.png)

## Example Flowchart

The following flowchart outlines the detailed steps performed by the echo loop example application:

@startuml{Echo_loop_Example_flowchart.png} "Echo Loop Example flowchart" width=5cm
  :MAIN;
  :Initialize application terminal (baudrate = 115200);
  :Display example title and user instructions;
  :ret = <b>stse_init</b>;
  if(ret == STSE_OK) then (No)
  :Display error message;
  :Terminate application;
    -[hidden]->
  detach
  else (Yes)
  while (while(1))
    :Generate a randomized "<b>Message</b>" with variable length and content;
    :Display the generated <b>"Message"</b>;
    :ret = <b>stse_device_echo</b>;
    if(ret == STSE_OK) then (Yes)
    :Display the <b>"Echoed message"</b> received from STSAFE-A120;
    :Wait for 1 second before next iteration;
    else (No)
    :Display error message;
    :Terminate application;
    -[hidden]->
    detach
    endif
  end while
  -[hidden]->
    detach
  endif
@enduml

## Host Terminal Output

Upon execution, the host terminal displays logs that reflect the ongoing communication between the host and the STSAFE-A120 device. Each iteration shows the message sent and the echoed response received, allowing users to observe the data exchange and verify correctness.

```
----------------------------------------------------------------------------------------------------------------
-                                         STSAFE-A120 Echo loop example                                                -
----------------------------------------------------------------------------------------------------------------
 ## Message :

  0x80 0x06 0xC8 0xA6 0x76 0x88 0x13 0xA9 0x3B 0xBE 0x33 0xCA 0xCE 0x71 0x09 0x80
  0x00 0xC5 0x5F 0xF5 0x09 0xC0 0xCC 0x6F 0x70 0x2B 0x15 0x77 0x62 0xB1 0xDC 0xE6
  0x58 0xFE 0x65 0x02 0x95 0xD3 0x1D 0x48 0xA1 0x49 0xBE 0xED 0x3B 0x8A 0x56 0x14
  0xEC 0x4A 0x79 0xD4 0x82 0x03 0xD5 0x44 0xDA 0xF3 0x27 0x66 0x0C 0xD1 0x19 0x8F
  0xE2 0x24 0x82 0x0A 0x17 0x3C 0xC8 0x94 0xAB 0x42 0x30 0x91 0x44 0x78 0x12 0xED
  0xDF 0xB8 0x8A 0x3C 0xF0 0xAB 0xC2 0x9A

 ## Echoed Message :

  0x80 0x06 0xC8 0xA6 0x76 0x88 0x13 0xA9 0x3B 0xBE 0x33 0xCA 0xCE 0x71 0x09 0x80
  0x00 0xC5 0x5F 0xF5 0x09 0xC0 0xCC 0x6F 0x70 0x2B 0x15 0x77 0x62 0xB1 0xDC 0xE6
  0x58 0xFE 0x65 0x02 0x95 0xD3 0x1D 0x48 0xA1 0x49 0xBE 0xED 0x3B 0x8A 0x56 0x14
  0xEC 0x4A 0x79 0xD4 0x82 0x03 0xD5 0x44 0xDA 0xF3 0x27 0x66 0x0C 0xD1 0x19 0x8F
  0xE2 0x24 0x82 0x0A 0x17 0x3C 0xC8 0x94 0xAB 0x42 0x30 0x91 0x44 0x78 0x12 0xED
  0xDF 0xB8 0x8A 0x3C 0xF0 0xAB 0xC2 0x9A
----------------------------------------------------------------------------------------------------------------
 ## Message :

  0xA9 0x7B 0xAA 0x89 0xE5 0xB6 0x93 0x58 0x1E 0x9C 0xA1 0x69 0xB8 0xF8 0xC2 0xEE
  0x40 0x71

 ## Echoed Message :

  0xA9 0x7B 0xAA 0x89 0xE5 0xB6 0x93 0x58 0x1E 0x9C 0xA1 0x69 0xB8 0xF8 0xC2 0xEE
  0x40 0x71
----------------------------------------------------------------------------------------------------------------
```

## Applicative Scenarios

The echo loop example serves several practical purposes in the context of secure element integration:

- **SE Integration and Stability Testing:** By repeatedly exchanging messages, developers can assess the robustness and reliability of the communication link between the host and the STSAFE-A120 device. This helps identify potential issues related to hardware connections, firmware compatibility, or protocol implementation.
- **Accessory Hot-Plug and Presence Detection:** The echo loop can be used to monitor the presence and readiness of accessories equipped with STSAFE-A120 devices. Continuous message exchange ensures that the SE is properly detected and operational whenever an accessory is connected or disconnected.
- **Debugging and Diagnostics:** The verbose logging of sent and received messages provides valuable insights during development and troubleshooting, enabling quick identification of anomalies or failures in the data path.
- **Performance Evaluation:** By analyzing the response times and throughput of the echo loop, users can benchmark the performance of the STSAFE-A120 integration under various conditions.

This example is an essential tool for developers and system integrators aiming to ensure seamless and secure communication with STSAFE-A120 devices in embedded applications.