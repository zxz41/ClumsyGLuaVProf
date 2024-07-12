# What is Clumsy GLua VProf?
Like it in the name, its a very clumsy, hacked together module for Garry's Mod to add VProf nodes to GLua script. If you find any crashes or bugs they probably won't be fixed any time soon.

Clumsy GLua VProf is NOT a stable module, there WILL likely be bugs, crashes and heap corruptions.
You should NEVER run this unless you're activily profiling, or know what you're doing.

The project is dependent on [garrysmod_common](https://github.com/danielga/garrysmod_common) to build.
### Why use the internal Valve profiler?
It's heavily inspired from (and based on) sigsegv's [Custom VProf analysis of attribute munger performance](https://youtu.be/RFdD4Sqe7i4) ([Code](https://github.com/sigsegv-mvm/sigsegv-mvm/blob/master/src/mod/prof/attr_munge.cpp)) and Garry's blog post about [Optimising GMod](https://garry.net/posts/optimising-gmod) (which have left a bunch of vprof nodes behind in the game's code.)
I tried using [FPtje's FProfiler](https://github.com/FPtje/FProfiler) but it felt at the time (a few years ago) very unwieldy, performance heavy, and didn't paint a full picture of game code vs user code (lua.) Although that probably have changed since then.
### Why base the code on the Source SDK?
At the time I'm mainly working with sourcemods, and it felt like a good practice to write something mostly from the bottom and up within it's constraints. Mostly to be more familiar with some of the inner workings of I.E CUtl*dotdotdot*. It also handily come in a minimal version with [garrysmod_common](https://github.com/danielga/garrysmod_common) which is required.
### Why not use detours?
I've had plans to use detours to properly update the VGUI panels for VProf. But at this time it's not within my scope.
### Can I reference your code for my own project?
Sure, credit would be nice. But isn't a requirement.
Although it's probably best you don't use the same coding practices...
### The code SUCKS
Why do you think it's called clumsy.
