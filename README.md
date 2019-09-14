# cpp-process-manager
Messing around with C++ hoping to create a simple process manager similar to "forever" but for any runtime

## Usage
Compile using `compile.sh`, then `cd` to build and run `cppm [processname] [args]`. 
Type `kill` in stdin to terminate main loop.

## Quick test
Running `run.sh` will compile and execute a `node` javascript file. 
It starts up an HTTP server and will never break the event loop. Therefore, the process should never end. 
There is an alternative with a setTimeout of one second which will break the event loop after 1000ms. 
The process should then restart on its own.
