Forked from stough to add compatibility for the SDK2 Beta.
<p>
## LCARS - Watchface
I loved this face, but updated my Pebble to the 2.0 firmware immediately upon getting my grubby little hands on it.  I didn't make any aesthetic changes, just updated the code to be in line with SDK 2.  
</p>
<p>
### Building
You'll need a working install of SDK 2.0.  This is my first attempt at the Pebble SDK so feel free to patch this baby if I forgot anything.  I'm used to higher level langauges that deal with garbage collecting and memory leaks for me.  It's been running on my watch for a few days now with now problems.  
</p>
<p>
git clone https://github.com/koepnick/lcars<br />
cd lcars<br />
pebble build<br />
pebble install --phone <ip address>
</p>
