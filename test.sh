#!/bin/bash

export PATH="./:$PATH"
export ROOT=$(window-root)

export MAIN=$(window-current)
export NEW=$(window-new $ROOT 100 100 90 90 new-test)
export RED=$(window-new $ROOT 150 150 500 500 red-test)
export BLUE=$(window-new $ROOT 10 10 90 90 blue-test)

export EVENTS=events

event-trigger session-start

window-focus $(window-list-visible $ROOT)
event-trigger position/side $(window-list-visible $ROOT)
event-trigger position/main $MAIN

window-background 990000 $RED
window-background 000099 $BLUE

#window-show $NEW $RED $BLUE

bash

window-kill $NEW
window-kill $RED
window-kill $BLUE
