# Napi Example
Example of using C++ in Node.js with Napi<br>

```
In this example we launch a GBA Emulator process
We create a object that is defined in C++ with the pid of the GBA Emulator
After that we can call the functions from that object to send input or take a screenshot
```

```
node-gyp configure build -> to build the C++ code
lib/binding.js -> binds the C++ class/methods to Node.js class/methods
src/includes/lodepng -> png lib
```