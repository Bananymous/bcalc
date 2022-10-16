# bcalc
Minimal numeric calculator for linux (and probably mac) written in C++ with ncurses.

Supports complex numbers, variables and functions.

# Dependencies
bcalc itself only depends on ncurses.

You will also need git, premake5, make and C++20 compliant compiler to build the packet.

# Installation
Building and installing from source
```
git clone https://github.com/Bananymous/bcalc
cd bcalc
premake5 gmake2
make config=release
sudo cp bin/Release/bcalc /usr/local/bin/bcalc
```

To uninstall, simply remove the executable
```
sudo rm /usr/local/bin/bcalc
```

# Usage
bcalc can be used as CLI or TUI.

Running the executable without any arguments starts it in TUI mode where you can do basic numeric calculations.

Special commands are 'exit' and 'clear'

![image](https://user-images.githubusercontent.com/68776844/196057066-be6ba813-095d-4f44-82e5-481fecea13e7.png)

![image](https://user-images.githubusercontent.com/68776844/196057857-cbe9f71f-86c9-44eb-9118-b8259ddc1cfb.png)

You can also run bcalc with the expressions as command line arguments, each expression separated by ';'.

![image](https://user-images.githubusercontent.com/68776844/196057372-307f879b-eccb-4ea1-a404-689f03431456.png)

Defined constants are pi and e.

Builtin functions include trigonometric functions, their hyperbolic counterparts and inverses, log, sqrt, exp, round, floor, ceil
