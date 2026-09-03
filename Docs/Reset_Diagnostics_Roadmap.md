# Embedded Firmware Reset Diagnostics Framework
## STM32 Discovery Board — Advanced Implementation Roadmap

> **Implementation-first learning method**
>
> We will build the project level by level.
>
> **Build → Run → Observe → Debug → Understand → Improve**
>
> No long theory sessions upfront. We learn each concept when the implementation requires it.

---

# Project Goal

Build a production-style **Embedded Firmware Reset Diagnostics and Recovery Framework** for an STM32 Discovery Board.

The final framework should be capable of:

- Detecting and classifying reset causes
- Recovering automatically from firmware lockups
- Persisting diagnostic information across watchdog resets
- Capturing useful fault/crash information
- Detecting repeated failures and boot loops
- Applying recovery/safe-mode policies
- Monitoring firmware health before feeding the watchdog
- Providing a UART-based diagnostic interface
- Demonstrating low-level understanding of Cortex-M startup and GNU linker behavior

---

# Level 1 — Solid Foundation

### Objective

Build a reliable basic reset-diagnostics foundation.

### Level 1 Implementation Sequence

```text
1. STM32 Project Baseline
        ↓
2. UART Startup Diagnostics
        ↓
3. IWDG Initialization
        ↓
4. Watchdog Servicing
        ↓
5. RCC Reset-Source Detection
        ↓
6. Boot Counter
        ↓
7. Basic Diagnostic Structure
        ↓
8. Reset-Flag Handling
        ↓
9. Build + Hardware Validation
```

### Step 1 — STM32 Project Baseline

- Verify the STM32CubeIDE project configuration
- Verify the target MCU/Discovery board configuration
- Verify generated `Core/`, `Drivers/`, startup, linker, and project configuration files
- Confirm `.project`, `.cproject`, `.settings/`, and `.ioc` are correctly tracked
- Confirm generated build artifacts are ignored by Git
- Build the untouched baseline project
- Flash it to the Discovery Board
- Confirm the firmware starts correctly
- Commit the clean baseline to Git

### Step 2 — UART Startup Diagnostics

- Configure the selected UART
- Implement a minimal diagnostic output mechanism
- Print a startup banner
- Confirm UART output after every reset
- Keep the implementation reusable for later diagnostic messages

### Step 3 — IWDG Initialization

- Enable/configure the Independent Watchdog
- Define the initial timeout configuration
- Start the watchdog
- Verify the firmware remains operational when the watchdog is serviced

### Step 4 — Watchdog Servicing

- Implement the initial watchdog refresh mechanism
- Keep normal firmware execution alive
- Prepare a controlled way to stop servicing the watchdog
- Verify the MCU resets after watchdog timeout

### Step 5 — RCC Reset-Source Detection

- Read the relevant RCC reset flags during startup
- Identify the initial reset categories required by the project
- Distinguish at least:
  - Power-on/reset condition
  - Software reset
  - IWDG/watchdog reset
- Print the detected reset reason through UART

### Step 6 — Boot Counter

- Add a basic boot counter
- Increment it during startup
- Verify expected behavior across supported reset scenarios
- Use the result as the foundation for later persistent diagnostics

### Step 7 — Basic Diagnostic Structure

- Introduce the first structured diagnostic data type
- Store reset reason and boot information
- Keep the structure simple at this stage
- Prepare the design for the persistent diagnostic record introduced in Level 3

### Step 8 — Reset-Flag Handling

- Determine when reset flags must be read
- Clear the relevant RCC reset flags after processing
- Verify stale flags do not incorrectly affect later boots

### Step 9 — Build + Hardware Validation

For each Level 1 feature:

- Build successfully
- Flash the STM32 Discovery Board
- Execute the relevant test scenario
- Observe UART output
- Inspect the debugger when required
- Record the observed behavior
- Fix implementation issues
- Re-test
- Commit the validated implementation

### Level 1 Completion Criteria

We can demonstrate:

```text
Power/initial reset
        ↓
Startup
        ↓
UART diagnostic output
        ↓
Reset source identified
        ↓
Boot information reported
        ↓
Normal watchdog operation
        ↓
Controlled watchdog timeout
        ↓
MCU resets
        ↓
Next startup identifies watchdog reset
```

---

# Level 2 — Fault Injection Framework

### Objective

Create controlled and repeatable failure scenarios so the reset diagnostics framework can be tested deliberately.

### Level 2 Implementation Sequence

```text
1. Define Fault-Injection Interface
        ↓
2. Implement Watchdog Timeout Scenario
        ↓
3. Implement Software Reset Scenario
        ↓
4. Add Repeatable Fault Triggers
        ↓
5. Observe Reset Behavior
        ↓
6. Validate Reset Classification
        ↓
7. Document Test Results
```

### Step 1 — Define Fault-Injection Interface

- Decide how development fault scenarios will be triggered
- Keep fault injection isolated from normal application logic
- Define clear fault identifiers
- Make fault injection easy to enable/disable

### Step 2 — Implement Watchdog Timeout Scenario

- Intentionally stop servicing the IWDG
- Allow the watchdog to expire
- Observe the MCU reset
- Confirm startup executes again
- Verify diagnostic output

### Step 3 — Implement Software Reset Scenario

- Add a controlled software-reset trigger
- Trigger the MCU software reset mechanism
- Observe startup behavior
- Verify RCC reset classification

### Step 4 — Add Repeatable Fault Triggers

- Provide deterministic triggers for each scenario
- Ensure the same test can be executed repeatedly
- Prevent test code from affecting normal operation when disabled

### Step 5 — Observe Reset Behavior

For every fault scenario:

- Record the trigger
- Observe UART output
- Inspect reset flags
- Inspect debugger state when useful
- Compare expected and actual behavior

### Step 6 — Validate Reset Classification

Verify that the framework correctly distinguishes supported reset causes and does not report stale or incorrect reasons.

### Step 7 — Document Test Results

Record:

- Test ID
- Trigger
- Expected result
- Actual result
- Pass/fail
- Relevant observations

### Level 2 Completion Criteria

Each supported reset scenario can be deliberately triggered and correctly identified after reboot.

---

# Level 3 — Persistent Diagnostic Record

### Objective

Create a structured diagnostic record that survives watchdog resets and becomes the foundation for historical reset information.

### Level 3 Implementation Sequence

```text
1. Design Diagnostic Structure
        ↓
2. Introduce Persistent Storage Area
        ↓
3. Place Structure in .noinit
        ↓
4. Update Record During Boot
        ↓
5. Preserve Data Across IWDG Reset
        ↓
6. Validate Initialization Rules
        ↓
7. Integrate With Startup Diagnostics
```

### Step 1 — Design Diagnostic Structure

Define the diagnostic record with fields such as:

- Magic value
- Version
- Boot count
- Reset count
- Reset reason
- Previous system state
- Fault code
- Watchdog count
- HardFault count

Keep the structure limited to what is required at this level.

### Step 2 — Introduce Persistent Storage Area

- Identify the RAM region to use
- Decide how persistent diagnostic data will be represented
- Separate persistent data from normal initialized variables

### Step 3 — Place Structure in `.noinit`

- Create the persistent diagnostic variable
- Configure the linker to place it in `.noinit`
- Verify its address and section using the linker map

### Step 4 — Update Record During Boot

- Read the existing record during startup
- Validate its basic identity
- Increment/update required counters
- Record the current reset reason

### Step 5 — Preserve Data Across IWDG Reset

- Run the application normally
- Trigger an IWDG reset
- Boot again
- Confirm previous diagnostic information is still present

### Step 6 — Validate Initialization Rules

- Confirm `.noinit` is not cleared by normal C startup initialization
- Verify the record is initialized only when appropriate
- Test invalid/uninitialized first-boot conditions

### Step 7 — Integrate With Startup Diagnostics

Print persistent information through UART so reboot history is visible.

### Level 3 Completion Criteria

After a watchdog reset, the next boot can retrieve and report diagnostic information from the previous execution.

---

# Level 4 — Cortex-M Fault / Crash Capture

### Objective

Capture useful processor fault information when a Cortex-M exception occurs and make it available after reboot.

### Level 4 Implementation Sequence

```text
1. Enable/Configure Fault Handlers
        ↓
2. Create Fault Capture Structure
        ↓
3. Capture Exception Context
        ↓
4. Capture Cortex-M Fault Registers
        ↓
5. Store Diagnostic Information
        ↓
6. Trigger Controlled Fault
        ↓
7. Reboot and Report Fault
        ↓
8. Validate Captured Data
```

### Step 1 — Enable/Configure Fault Handlers

- Establish the required fault handlers
- Route relevant exceptions into the diagnostic mechanism where appropriate
- Keep fault handling deterministic

### Step 2 — Create Fault Capture Structure

Provide storage for:

- Fault type
- R0–R3
- R12
- LR
- PC
- xPSR
- Stack pointer
- Fault status registers

### Step 3 — Capture Exception Context

- Determine how the exception stack frame is obtained
- Capture processor context
- Preserve information before reset/recovery

### Step 4 — Capture Cortex-M Fault Registers

Capture applicable registers such as:

- CFSR
- HFSR
- BFAR
- MMFAR

### Step 5 — Store Diagnostic Information

Integrate captured information with the persistent diagnostic record.

### Step 6 — Trigger Controlled Fault

Create controlled test scenarios for relevant Cortex-M faults.

### Step 7 — Reboot and Report Fault

After reboot:

- Identify the fault-related reset condition
- Retrieve stored crash information
- Print a readable fault report through UART

### Step 8 — Validate Captured Data

Compare captured register values against debugger observations and controlled fault scenarios.

### Level 4 Completion Criteria

A controlled processor fault produces a persistent diagnostic record containing enough information to identify and investigate the failure.

---

# Level 5 — Recovery Policy

### Objective

Move from simply detecting failures to making deliberate recovery decisions.

### Level 5 Implementation Sequence

```text
1. Define System States
        ↓
2. Define Failure Counters
        ↓
3. Define Recovery Levels
        ↓
4. Implement Recovery Decision Logic
        ↓
5. Add Normal Recovery
        ↓
6. Add Degraded Recovery
        ↓
7. Add Safe Mode
        ↓
8. Validate Policy
```

### Step 1 — Define System States

Define application states required by the recovery mechanism, such as:

- Startup
- Initialization
- Running
- Degraded
- Safe mode
- Faulted

### Step 2 — Define Failure Counters

Track relevant failures:

- Watchdog resets
- Fault resets
- Consecutive failures
- Boot failures

### Step 3 — Define Recovery Levels

Create clear recovery decisions:

```text
Normal
Degraded
Safe Mode
```

### Step 4 — Implement Recovery Decision Logic

Use diagnostic history and reset information to select the appropriate boot/recovery path.

### Step 5 — Add Normal Recovery

Allow isolated failures to return to normal operation.

### Step 6 — Add Degraded Recovery

When failures become repeated, disable or reduce non-critical functionality while maintaining essential operation.

### Step 7 — Add Safe Mode

Provide a controlled minimal operating mode for persistent/repeated failures.

### Step 8 — Validate Policy

Intentionally generate increasing failure counts and verify the expected recovery transition.

### Level 5 Completion Criteria

The firmware makes a deterministic recovery decision based on failure history instead of merely reporting the failure.

---

# Level 6 — Boot-Loop Detection

### Objective

Detect repeated reset/fault cycles that prevent the firmware from reaching a stable operating state.

### Level 6 Implementation Sequence

```text
1. Define Successful-Boot Condition
        ↓
2. Track Consecutive Boot Attempts
        ↓
3. Define Boot-Loop Window
        ↓
4. Detect Repeated Failure
        ↓
5. Enter Safe Mode
        ↓
6. Clear Failure State After Stable Boot
        ↓
7. Validate Boot-Loop Recovery
```

### Step 1 — Define Successful-Boot Condition

Define what constitutes a successful boot, such as reaching a stable application state.

### Step 2 — Track Consecutive Boot Attempts

Maintain required persistent counters across resets.

### Step 3 — Define Boot-Loop Window

Choose a deterministic mechanism for deciding when repeated failures represent a boot loop.

### Step 4 — Detect Repeated Failure

Compare the current boot attempt with persistent history.

### Step 5 — Enter Safe Mode

When the configured threshold is exceeded, stop normal startup and enter the defined safe-mode behavior.

### Step 6 — Clear Failure State After Stable Boot

After the system remains healthy long enough:

- Mark the boot successful
- Reset consecutive-failure tracking
- Preserve useful historical counters

### Step 7 — Validate Boot-Loop Recovery

Create an intentional repeated-failure scenario and confirm the firmware eventually escapes the reset loop.

### Level 6 Completion Criteria

Repeated startup failures are detected and result in controlled safe-mode entry rather than endless uncontrolled rebooting.

---

# Level 7 — Watchdog Health Monitoring

### Objective

Replace simple periodic watchdog feeding with system-level health monitoring.

### Level 7 Implementation Sequence

```text
1. Identify Critical Components
        ↓
2. Define Health Indicators
        ↓
3. Implement Health Reporting
        ↓
4. Implement Watchdog Manager
        ↓
5. Gate IWDG Refresh on System Health
        ↓
6. Inject Component Failure
        ↓
7. Validate Recovery
```

### Step 1 — Identify Critical Components

Define firmware components that must demonstrate health before the watchdog is refreshed.

Examples:

- Application
- Communication
- Sensor manager
- Control logic
- Diagnostics

### Step 2 — Define Health Indicators

Define deterministic heartbeat/status conditions for each component.

### Step 3 — Implement Health Reporting

Allow components to report that expected activity completed.

### Step 4 — Implement Watchdog Manager

Create a central watchdog manager responsible for evaluating health state.

### Step 5 — Gate IWDG Refresh on System Health

Refresh the watchdog only when required health conditions are satisfied.

### Step 6 — Inject Component Failure

Intentionally stop one component from reporting healthy operation.

### Step 7 — Validate Recovery

Confirm:

```text
Component failure
      ↓
Health check fails
      ↓
IWDG refresh withheld
      ↓
Watchdog timeout
      ↓
Reset
      ↓
Diagnostics
```

### Level 7 Completion Criteria

The watchdog operates as a system-level recovery mechanism rather than merely a timer that is always refreshed.

---

# Level 8 — Persistent Memory Integrity

### Objective

Make the persistent diagnostic record robust against invalid or corrupted data.

### Level 8 Implementation Sequence

```text
1. Define Persistent Record Format
        ↓
2. Add Magic and Version
        ↓
3. Add Sequence Number
        ↓
4. Add CRC
        ↓
5. Validate Record at Boot
        ↓
6. Handle Invalid Data
        ↓
7. Validate Corruption Scenarios
```

### Step 1 — Define Persistent Record Format

Finalize the layout of the persistent diagnostic structure.

### Step 2 — Add Magic and Version

Use a known magic value and version field to distinguish valid records from arbitrary RAM contents.

### Step 3 — Add Sequence Number

Use a sequence number to identify record progression and support future history management.

### Step 4 — Add CRC

Calculate and store a CRC over the protected diagnostic fields.

### Step 5 — Validate Record at Boot

At startup:

```text
Magic valid?
   ↓
Version valid?
   ↓
CRC valid?
   ↓
Record usable
```

### Step 6 — Handle Invalid Data

Define deterministic behavior when the record is invalid:

- Discard
- Reinitialize
- Report invalid data
- Continue normal boot

### Step 7 — Validate Corruption Scenarios

Intentionally modify/corrupt diagnostic data and verify invalid records are rejected.

### Level 8 Completion Criteria

Persistent diagnostic information is validated before use and is not blindly trusted merely because it resides in `.noinit`.

---

# Level 9 — Linker and Cortex-M Startup Engineering

### Objective

Prove how startup code, C runtime initialization, memory sections, and the custom linker script interact with the persistent diagnostics framework.

### Level 9 Implementation Sequence

```text
1. Inspect Vector Table
        ↓
2. Trace Reset_Handler
        ↓
3. Inspect .data Initialization
        ↓
4. Inspect .bss Initialization
        ↓
5. Verify .noinit Behavior
        ↓
6. Inspect Linker Script
        ↓
7. Analyze Linker Map
        ↓
8. Verify Runtime Addresses
        ↓
9. Validate With Debugger
```

### Step 1 — Inspect Vector Table

Identify:

- Initial stack pointer
- Reset handler address
- Relevant exception handlers

### Step 2 — Trace Reset_Handler

Follow the actual startup sequence from reset toward C runtime and `main()`.

### Step 3 — Inspect `.data` Initialization

Verify how initialized global/static variables are copied from their load address to RAM.

### Step 4 — Inspect `.bss` Initialization

Verify how zero-initialized variables are cleared.

### Step 5 — Verify `.noinit` Behavior

Demonstrate that the `.noinit` diagnostic region is not cleared by normal startup initialization.

### Step 6 — Inspect Linker Script

Analyze:

- MEMORY regions
- Section placement
- Load/run addresses
- `.noinit` placement
- Stack placement

### Step 7 — Analyze Linker Map

Verify:

- Flash usage
- RAM usage
- Section sizes
- `.noinit` address and size
- Symbol addresses

### Step 8 — Verify Runtime Addresses

Use the debugger to compare actual addresses against linker-map expectations.

### Step 9 — Validate With Debugger

Trace startup and inspect memory before and after reset.

### Level 9 Completion Criteria

We can demonstrate, using actual startup code, linker configuration, map output, and debugger observations, why the reset-diagnostics memory behaves as designed.

---

# Level 10 — Professional Diagnostic Interface

### Objective

Create a practical UART diagnostic interface for inspecting and controlling the reset-diagnostics subsystem.

### Level 10 Implementation Sequence

```text
1. Define CLI Command Structure
        ↓
2. Implement UART Command Input
        ↓
3. Implement Status Command
        ↓
4. Implement History Command
        ↓
5. Implement Fault Command
        ↓
6. Implement Health Command
        ↓
7. Implement Clear/Control Commands
        ↓
8. Validate CLI Behavior
```

### Step 1 — Define CLI Command Structure

Define a small, deterministic command set.

Examples:

```text
reset status
reset history
reset clear
fault status
system health
```

### Step 2 — Implement UART Command Input

- Receive commands
- Parse commands
- Handle invalid commands
- Return readable responses

### Step 3 — Implement Status Command

Report:

- Last reset reason
- Boot count
- Reset count
- Watchdog count
- Fault count
- Current recovery mode

### Step 4 — Implement History Command

Display stored reset/fault history in chronological or sequence order.

### Step 5 — Implement Fault Command

Display latest captured fault information and processor registers.

### Step 6 — Implement Health Command

Display current health state of monitored components.

### Step 7 — Implement Clear/Control Commands

Provide controlled commands for operations such as clearing diagnostic history where appropriate.

### Step 8 — Validate CLI Behavior

Test:

- Valid commands
- Invalid commands
- Repeated commands
- Commands after resets
- Commands while in degraded/safe mode

### Level 10 Completion Criteria

A developer can interact with the running firmware through UART and inspect the reset, fault, recovery, and health state without a debugger.

---

# Final Target — Production-Style Reset Diagnostics Framework

At completion, the architecture should resemble:

```text
                         APPLICATION
                              │
                              ▼
                  ┌────────────────────────┐
                  │ Reset Diagnostics       │
                  │ Manager                 │
                  │                        │
                  │ • Reset Identification │
                  │ • Fault Capture        │
                  │ • Boot Tracking        │
                  │ • Recovery Policy      │
                  │ • Safe Mode             │
                  └───────────┬────────────┘
                              │
          ┌───────────────────┼───────────────────┐
          ▼                   ▼                   ▼
      RCC Flags             IWDG              .noinit
          │                   │                   │
          └───────────────────┼───────────────────┘
                              │
                              ▼
                    Persistent Diagnostics
                              │
                    ┌─────────┴─────────┐
                    ▼                   ▼
               Fault Data          Reset History
                    │                   │
                    └─────────┬─────────┘
                              ▼
                     Recovery Decision
                              │
                    ┌─────────┴─────────┐
                    ▼                   ▼
                Normal Mode          Safe Mode

                 UART Diagnostic Interface
```

---

# Implementation Rules

## Rule 1 — Implementation First

We do not study all the theory before coding.

We implement first and learn the required concept when we reach it.

## Rule 2 — One Level at a Time

We complete and validate one level before moving to the next.

## Rule 3 — Real Hardware Validation

Whenever practical, behavior should be verified on the STM32 Discovery Board rather than only through code inspection.

## Rule 4 — Deliberate Fault Injection

Failures should be intentionally generated so that recovery and diagnostics can be validated.

## Rule 5 — Understand What We Build

After an implementation exposes a low-level concept, we investigate that concept sufficiently to explain the implementation and debugging result.

## Rule 6 — No Unnecessary Scope Creep

Advanced features should support the reset-diagnostics objective. We do not add features merely to make the project larger.

## Rule 7 — Every Level Must Be Demonstrable

Each completed level should have:

- Implementation
- Test scenario
- Observed result
- Debug/analysis evidence
- Validation
- Completion status

---

# Progress Tracker

| Level | Area | Status |
|---|---|---|
| 1 | Solid Foundation | 🟡 In Progress |
| 2 | Fault Injection Framework | ⬜ Not Started |
| 3 | Persistent Diagnostic Record | ⬜ Not Started |
| 4 | Cortex-M Fault / Crash Capture | ⬜ Not Started |
| 5 | Recovery Policy | ⬜ Not Started |
| 6 | Boot-Loop Detection | ⬜ Not Started |
| 7 | Watchdog Health Monitoring | ⬜ Not Started |
| 8 | Persistent Memory Integrity | ⬜ Not Started |
| 9 | Linker & Startup Engineering | ⬜ Not Started |
| 10 | Professional Diagnostic Interface | ⬜ Not Started |
| Final | Production-Style Framework | ⬜ Not Started |

---

# Working Method for Each Level

For every level:

```text
1. Define the immediate implementation goal
        ↓
2. Inspect the current project
        ↓
3. Implement the smallest useful change
        ↓
4. Build
        ↓
5. Flash and run on STM32
        ↓
6. Observe UART / debugger / memory
        ↓
7. Debug failures
        ↓
8. Learn the specific concept involved
        ↓
9. Improve the implementation
        ↓
10. Validate the level
        ↓
11. Mark the level complete
        ↓
12. Move to the next level
```

**Current target: Level 1 — Solid Foundation**
