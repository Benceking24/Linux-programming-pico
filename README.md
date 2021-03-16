# Feladat kiírás
A célom ARM architektúrára épülő rendszerben egy szenzor adatának feldolgozás grafikus felületre és az eszköz külvilág felé történő interaktálása. Fejleszőeszköként egy **Raspberry Pi B rev.2** *(2011.12 Model B Revision 2.0 512mb)* -t és ehhez egy **ROHM BH1750** 16-bites digitális fényerősségmérőt használok amit végül egy LED-et PWM jellel vezérelve a szenzor elé helyezek visszajelző hurkot alkotva. Az alkalmazás elsődleges fő feladata az *I/O-k* kezelése és felcsatolása, hogy a grafikus felület tudjon vele interaktálni. A grafikus felületen leolvasható lesz a mért fényerősség és állítható a LED fényereje. Az alkalmazásba megadható a leolvasás gyakorisága és skálázás, továbbá rögzíti a mért átlag és szélső értékeket.

A program az alábbi funkciókkal fog rendelkezni:
* Grafikus felület Qt-ban megvalósítva.
* I2C szenzor felcsatolás és konfigurálás
* PWM led manuális kezelés
* Mért adatok nyers és statisztikai megjelnítése
* Mérés megállítás manuális elemzéshez
* Mérési időköz megadása

# Megvalósított program
Az alkalmazás képes olvasni az i2c fényérzékelőt és vezérelni egy LED-et PWM-el. Az olvasott adatokat lokálisan eltárolja és abból statiszitkát jelenít meg változtatható formátumban. A mérés megállítható és újraindítható. A LED értéke vezérelhető gyorsgombokkal és csúszkával is. A forráskódból QtCreator-ból kell buildelni a forráskódhoz tartozó .pro fájl alapján olyan Raspberry Pi-re aminek az i2c bus-a a /dev/i2c-1 alatt található. (512mb-os és újabb modellek) A futtatható állományt root felhasználóként lehet csak futtatni a wiringPi könyvtár miatt, mivel az csak így éri el a PWM-hez szükséges interface-t.

**Bemutató videó URL:** https://youtu.be/a8OswxzI73U


# Tapasztalatok
A Qt-t és a wiringpi könyvtár i2c részét nem egyszerű összeilleszteni, könnyebb út az egyenes direktíva kezelés, ha dokumentálva van a illesztendő eszköz i2c kommunikációja. Emiatt a projek 1/3-át az tette ki, hogy a már shell-ben futó kódot hogy tudom átültetni Qt-alá. QtCreator-t nem célszerű egyenesen a Rapsberryn futtani (egy build 5-8 perc is lehet egy egyszerűbb widgetnél) se Lxterminal-on keresztül futtatni az alkalmazást mert nagyon limitált az eszköz erőforrása.
