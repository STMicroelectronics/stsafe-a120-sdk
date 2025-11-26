# STSAFE-A120 Project Template {#STSAFE-A120_Project_template}

This template provides a minimal starting point for developing custom STSAFE-A120 application projects. It includes the basic initialization and structure needed to communicate with the STSAFE-A120 secure element, allowing developers to quickly build their own examples and applications.

The template demonstrates the essential setup steps required for any STSAFE-A120 application, including terminal initialization and the main application loop structure. Developers can use this as a foundation and add their specific functionality.

## Example Flowchart

The following flowchart illustrates the basic template structure:

@startuml{Project_template_flowchart.png} "Project Template flowchart" width=5cm
	:MAIN;
	:Initialize application terminal (baudrate = 115200);
	note right
	Add your initialization code here:
	- STSAFE device initialization
	- Variable declarations
	- Configuration setup
	end note
	while (while(1))
	note right
	Add your application logic here:
	- STSAFE operations
	- Data processing
	- Communication handling
	end note
	end while
	-[hidden]->
	detach
@enduml

## Template Code Structure

The template provides the following structure:

```c
/**
 * @brief  Main program entry point
 * @note   This is a template project for STSAFE-A120 examples
 *         Add your application code here
 * @retval Not applicable (infinite loop)
 */
int main(void) {

    /* Initialize Terminal */
    apps_terminal_init(115200);

    while (1) {
        /* Infinite loop - add your application code here */
    }
}
```

## Using the Template

To create a new STSAFE-A120 application project:

1. **Copy the Template:**
   - Duplicate the `00_Project_template` directory
   - Rename it according to your project name

2. **Add Device Initialization:**
   ```c
   stse_ReturnCode_t stse_ret;
   stse_Handler_t stse_handler;
   
   /* Initialize STSAFE-A device handler */
   stse_ret = stse_set_default_handler_value(&stse_handler);
   stse_handler.device_type = STSAFE_A120;
   stse_handler.io.busID = 1;
   stse_handler.io.BusSpeed = 400;
   
   /* Initialize device */
   stse_ret = stse_init(&stse_handler);
   ```

3. **Add Your Application Logic:**
   - Implement your specific STSAFE-A120 operations
   - Add error handling
   - Configure device features as needed

4. **Customize Configuration:**
   - Modify `stse_conf.h` to enable/disable specific features
   - Adjust memory allocations and buffer sizes

## Common Operations to Add

Typical additions to the template include:

- **Device Authentication:**
  ```c
  stse_device_authenticate(&stse_handler, ca_cert, cert_zone, key_slot);
  ```

- **Key Generation:**
  ```c
  stse_generate_key_pair(&stse_handler, slot_number, key_type);
  ```

- **Data Storage:**
  ```c
  stse_data_partition_write(&stse_handler, zone_id, offset, data, length);
  stse_data_partition_read(&stse_handler, zone_id, offset, buffer, length);
  ```

- **Cryptographic Operations:**
  ```c
  stse_generate_random(&stse_handler, random_buffer, length);
  stse_compute_hash(&stse_handler, algo, message, msg_len, hash, hash_len);
  stse_aes_cmac(&stse_handler, key_slot, message, length, mac);
  ```

## Best Practices

When developing your STSAFE-A120 application:

- **Error Handling:** Always check return codes from STSE API calls
- **Resource Management:** Properly allocate and free buffers
- **Security:** Follow secure coding practices, especially for key handling
- **Documentation:** Document your code with Doxygen comments
- **Testing:** Test thoroughly with different device configurations
- **Logging:** Use the terminal output for debugging and status messages

## Applicative Scenarios

The template can be extended for various applications:

- **Custom Security Solutions:** Implement proprietary security protocols
- **Integration Testing:** Develop test applications for device validation
- **Proof of Concept:** Quickly prototype new security features
- **Educational Projects:** Learn STSAFE-A120 programming with a clean starting point
- **Production Applications:** Build production-ready firmware from the template base

## File Structure

A typical project based on the template includes:

```
00_Project_template/
├── main.c              # Application entry point
├── stse_conf.h         # STSAFE library configuration
├── <IDE files>         # Project files for STM32CubeIDE, Keil, IAR
```

Refer to other example projects in the package to see complete implementations of specific STSAFE-A120 features and use cases.
