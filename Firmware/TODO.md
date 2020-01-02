
# SL TODO/Roadmap

This file serves as my personal TODO feature and bugfix list.

----

# Todo

- VCS Keyboard/Video Pad support
- Strip out LED debug output from the code (or toggle mode)
- LED support via serial

	[l]eds

	Mode? Qdrg

	[Q]uit

	[d]ebug
		Q10
		[Q]uit
		[1] on
		[0] off

	[r]ed immediate
	[g]reen immediate
		Q0-9

		[Q]uit
		[0-9] number indicating intensity, 0=off, 9=bright



# Future

- Automatic controller/config 
- CD32 controller support - maybe?
- My own HID library providing all of the above functionality, plus:
 - Ability to change HID Descriptor name
 - Handling of keyboard LEDs -> board LEDs

# Bugfixes

None yet.


# DONE

## 2020-01-01

- Tested and fixed Paddle ranging
- Paddles clear min/max when reselected
- 2800 controller configuration is working (joystick + analog paddle)
- CD-32 controller removed from menu