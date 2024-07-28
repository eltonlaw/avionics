target extended-remote localhost:3333

# Ensure the target is halted before setting breakpoints
monitor halt

# Optional: Reset the device and reinitialize the debug session
monitor reset init

# Try setting a breakpoint in the main function
break main

# Load the program symbols, if not already done
load
