# TelescopeFilter

##This is all super hacky but my setup is:

* Stepper motor has 3D print from `Filterv3.stl` attached 
* Arduino connected to the stepper by ULN2003 running `TelescopeFilter.ino` 
* Arduino is connected to Raspberry Pi over USB (Because I wanted it 15ft away so it wouldn't overheat)
* Raspberry Pi runs `telescope-eclipse.py` **You must change the local times and the timezone** 
* Raspberry Pi is synced to UTC via NTP (all the stratum 1 clocks in my house.)
