#!/bin/bash

export PATH="./:$PATH"

export ROOT=$(window-root)
export WINDOW=$(window-current)
export NEW=$(window-new $ROOT 100 100 90 90 new-test)
export RED=$(window-new $ROOT 150 150 500 500 red-test)
export BLUE=$(window-new $ROOT 10 10 90 90 blue-test)

window-background 990000 $RED
window-background 000099 $BLUE

window-show $NEW $RED $BLUE

echo
echo starting elegacy shell
echo
echo \$NEW, \$RED, \& \$BLUE are created for your convenience

bash

window-kill $NEW
window-kill $RED
window-kill $BLUE
