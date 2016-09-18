# defx-xmms
DeFX plug-in for XMMS v0.9.8

----------------------------------
Compilation Instructions

If you have downloaded the source, you can compile it by your own.
You only need to have some basic development packages
for Linux, like GTK+ and XMMS.

Note that you need the runtime packages to execute the plugin,
and the additional development packages to compile it.

To compile just do: 

make

----------------------------------
Install Instructions

DeFX is just one file. It must be installed into the effect plugins
directory of xmms (usually located in /usr/lib/xmms/Effect).

For automatic installation you can do as root:

make install

----------------------------------
Activation Instructions

Launch XMMS and the DeFX control panel will be showed up. It will not be
activated until you do it in the XMMS' preferences window

You can press Ctrl-P to view the preferences window. From there, activate
the DeFX in the Effect Plugins section
