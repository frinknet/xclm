XCLM!
=====

X Command Line Manipulation Suite
---------------------------------

XCLM! is the building blocks of a scripted window manager. This includeds the XMPL
library to use as a buiding block in other window managers. You can use XCLM to
prototype complext window management scenarios inot possible with more high level
window managers. It does not manage more windows than you ask it to.

Because of the specialized nature and shear number of these programs it is best to
install them outside of your normal $PATH so as not to convolute your bin directory
with random executables. You can then selectively add the commands directory to your
$PATH in control scripts that facilitate window movement.

Each command is intended to be *extremely succinct* in order to demonstrate the
usage capabilities of XMPL.  As some point we will roll this into a proper library
to reduce footprint even farther. But first we need to make sure all commands for
window management exist.

It is not intended to have a manpage fore each command. But rather to limit each
command to a single usecase avoiding the need for complex documentation. Typing a
command will either provide usage details or produce a result if no arguments are
expected. Commands without arguments will not manipulate windows but only display
information so you can run them without fear. (e.g. window-current, window-root)

It is expected that this command set will continue to grow until it covers every
possible window management scenario and XMPL is complete. There is an evolvin
GUIDELINES.md document that details the proper nominclature for new commands.
These guildelines insure a standard interface for all commands.

Fininshed commands have a .c extention and should compile fine. Planned commands
are in the todo folder. This way it is easy to see where contribution is needed.
All command names should be self explanitory.

Install
-------

Should be pretty simple with the usual *make* and *make install*. Relies on a few
xcb libs that should be evident if you make and they aren't there.

Install process is still a little squirrelly and likely to change.
