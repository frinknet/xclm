XCLM! X Command Line Management
=========

XCLM! is the building blocks of a scripted window manager. They can be used in
conjunction with XCB Watch to script complex window interactions. You can use XCB
Tools to prototype complext window management scenarios not possible with more high
level window managers.

Because of the specialized nature and shear number of these programs it is best to
install them outside of your normal $PATH so as not to convolute your bin directory
with random executables. You can then selectively add the commands directory to your
$PATH in control scripts that facilitate window movement.

Each command is intended to be *extremely succinct* in order to demonstrate the
usage capabilities of XCB. XCLM! also provides a library of XCB helper functions
to encapsulate common actions. As some point we will roll this into a proper library
to reduce footprint even farther. But first we need to make sure all commands for
window management exist.

It is not intended to have a manpage fore each command. But rather to limit each
command to a single usecase. Typing a command will either provide usage details or
produce a result if no arguments are expected. Commands without arguments will not
manipulate windows but only display information.

It is expected that this command set will continue to grow until it covers every
possible wm scenario. There is an evolving GUIDELINES.md document that details the
proper nominclature for new commands. These guildelines insure a standard interface
for all commands.

Fininshed commands have a .c extention and should compile fine. Planned commands
have a .\_ extension to denote their unfinished status. This way it is easy to see
where contribution is needed.

Plan
 - XCLM!: low level tools for window management
 - XCB Watch: low level event daemon that watches for X events and fires off scripts
   from a specified directory
 - XCB Hotkeys: watches a hotkey directory for scripts with specifict hotkey names
 - XCB Panel: generic panel sdrawing utility for scripted UI
 - XCB Decorate: scriptable compositing (if possible)
 - XCB Framing: some layout primitives to facilitate tiling, tabbing, scroll frames,
   accordions and other more complex interactions
