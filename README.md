# Testing odd behavior for gtk widgets when created and destroyed on click.

I experienced odd behavior with gtk buttons. When I click on a button that triggers the parent container to clear and rebuild the buttons memory usage increases but is never released.

This behavior only happens when you click on the buttons being destroyed. There is a control button on the left which triggers the exact same updating of the buttons only this button is static and not destroyed. When you use the control button memory is steady. When using the other buttons memory steadily increases. You may need to rapidly press buttons to see anything meaningful but it happens. Memory may slightly increase when first starting the app due to gtk caching. Play around with it a bit and you should eventually see the pattern.

## Usage

Build and run

```
make && make run
```

Watch memory usage (Linux). This polls `proc/$(pgrep -f gtk-widget-churn)/status` to check the total memory usage of the running application every 2 seconds.

```
./poll-mem.sh
```
