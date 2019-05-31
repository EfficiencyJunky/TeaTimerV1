# TeaTimer

This is a simple yet elegant tea timer that will allow you to properly time your brews every time! You can build one yourself with the components listed in the BOM, and then upload the Arduino code in the "TeaTimer_Main_V1" folder to make it work. If you have access to a 3D printer, you can print an enclosure using the files in the "3DPrintFiles" folder  to make it easy to assemble and look more polished.

Of course this is all up for interpretation! Please take this project and modify it and make it everything you want it to be. The simplest modification would be simply to purchase a smaller, cheaper battery. You could easily knock $10 off the price, make it smaller, and it will work just as well.

For what it's worth though, I will provide documentation on how I built mine in case you want a good a good place to start.

# See it in action
For some background into why I made this, and a demo of it in action, check out my instagram post here:
[https://www.instagram.com/p/ByHbxj5pm57/](https://www.instagram.com/p/ByHbxj5pm57/)


# Build your own!
This project is a great way to get your feet wet in DIY product making. It requires only basic knowledge of Arduino, breadboarding, soldering, and optional 3D Printing.

### Acquire the Parts
The version I built cost around $60 in parts. All of the components can be purchased on Adafruit.com 

Here's the parts list (aka BOM or Bill of Materials) with links and descriptions of everything you'll need to create your very own Tea Timer V1
[BOM-aka-parts_list.csv](https://github.com/EfficiencyJunky/TeaTimerV1/blob/master/BOM-aka-parts_list.csv "BOM-aka-parts_list.csv")

Once you have the parts, you will need to do a little bit of Soldering to get them all prepped to plug and play.

The hardest soldering job will be soldering the Adafruit LiIon/LiPoly Backpack onto the Pro Trinket. They sort of describe how to do this on the product page, so hopefully it's not too hard to figure that out! Before you solder the Backpack to the Trinket Pro though, be sure to solder a dual header or pair of wires with at least 8 inches to the two holes on the LiIon/LiPoly Backpack that allow you to attach an on/off switch. Here's a picture that shows what I'm talking about
[https://cdn-shop.adafruit.com/970x728/2124-01.jpg](https://cdn-shop.adafruit.com/970x728/2124-01.jpg)
Of course the switch I have spec'd in the BOM is not the same switch in the picture, but the connections will be the same.

Besides that, the only other soldering jobs will be connecting wires to terminals on the On/Off switch, 8 Position Rotary Switch, and Button with RGB LED. 
<!--stackedit_data:
eyJoaXN0b3J5IjpbMTMxNDI1NzIyOCw4MzQ3OTA3ODcsMTIzMD
Y1MzY4MV19
-->