# Raspberry Pi clock

A clock + weather display using a Raspberry Pi with a 3.5" LCD display retrofitted inside an Areaware iPhone Dock.
This hack require to hollow out the dock since it's a massive block of wood. The "iPhone" like front panel is composed of a laser cut plexiglass with the back painted black leaving a window for the display, and finally an aluminum band carefully bended and glued with epoxy.

<img src="https://raw.github.com/papabricole/rpiclock/master/RaspberryPiClock.jpg" >

# Instructions (brain dump)

## Tontec 3.5” Screen Driver Install Instructions - Model: MZ61581

### Enable SPI and set overlay for Tontec MZ61581 Screen

```sh
sudo vim /boot/config.txt
```

And add these lines to the bottom

```sh
dtparam=spi=on
dtoverlay=mz61581
```

Then save and reboot

### Set Tontec 3.5 Screen as the default display instead of HDMI

Here we will change the default output display from HDMI to Tontec Screen

sudo vim /usr/share/X11/xorg.conf.d/99-fbturbo.conf

```sh
Change
          Option          "fbdev" "/dev/fb0"
To
          Option          "fbdev" "/dev/fb1"
```

### Edit cmdline.txt 

Here we will enable Tontec Screen to display during the booting process 

sudo vim /boot/cmdline.txt

Add fbcon=map:10 at the end of current line. (No need to start a new line)

## Autostart

```sh
> sudo cat /etc/rc.local
su - morgan -c 'startx' &

> cat .xinitrc
#!/bin/sh
xset s off
xset -dpms 
xset s noblank

while true; do
  rpiclock
done;
```
## Backlight

Turn off backlight
```sh
echo 1 | sudo tee /sys/class/backlight/*/bl_power
```

Turn on backlight
```sh
echo 0 | sudo tee /sys/class/backlight/*/bl_power
```

Crontab
```sh
sudo crontab -e
00 23 * * * echo 1 | sudo tee /sys/class/backlight/*/bl_power
00 07 * * * echo 0 | sudo tee /sys/class/backlight/*/bl_power
```
