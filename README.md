XCB Tools
=========

XCB Tools is are the building blocks of a scripted window manager.

XCB Tools can be used in conjunction with XCB Watch to script window interactions.

You can use XCB Tools to prototype complext window management scenarios not possible
with more high level window managers.

Because of the specialized nature of these programs it is best to install them
outside your normal path so as not to convolute your bin directory. You can add this
commands directory to your path in control scripts that facilitate window movement.


Plans
 - XCB Tools: low level tools for window management
 - XCB Watch: low level event daemon that watches for X events and fires off scripts
   from a specified directory
 - XCB Hotkeys: watches a hotkey directory for scripts with specifict hotkey names
 - XCB Panel: generic panel drawing utility for scripted UI
 - XCB Decorator: scriptable compositing (if possible)
 - XCB Framer: some layout primitives to facilitate tiling, tabbing, scroll frames,
   accordions and other more complex interactions
