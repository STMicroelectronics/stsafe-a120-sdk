# Project package example

This project folder contains examples illustrating typical use cases for the STSAFE-A120.  
The index preceding the project folder name illustrates the execution hierarchy of the examples.  
This means that the project with a higher index must be executed after the project with a lower index.  
For example, index project 03 must be run after index project 02.  
Indeed, some tests configure the STSAFE-A120 according to a typical configuration for field usage.

Dependency index between groups of examples:
- 0: Template (No dependency)
- 1: Examples without dependency
- 2: Provisioning mode configuration examples 
- 3: Crypto examples with dependency on group 2 (requires provisioning mode configured in group 2)
- 4: Configuration examples without dependency
- 5: Advanced examples with dependency of groups 2 & 4 (requires both provisioning mode and group 4 configuration on different key slots)

Note: Execution on SPL05 requires setting provisioning mode on the appropriate key slots before running crypto examples from group 3 and group 5. Ensure group 2 configuration is completed first to properly initialize the required key slots. 