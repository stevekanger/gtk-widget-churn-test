while true; do
    grep -E 'VmRSS|VmHeap|RssAnon' "/proc/$(pgrep -f gtk-widget-churn)/status"
    echo "------------------------------"
    sleep 2
done
