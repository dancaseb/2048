Hra 2048. Tuto hru lze spustit pomoci prikazu make a nasledne pomoci ./projekt AxB, kde A je pocet radku a B je pocet sloupcu hraciho pole. Standardni pole je 4x4. Pri zadani spatnych vstupnich parametru se hra nespusti. Pravidla hry: hrac pomoci sipek pohybuje dlazdicemi po hracim poli. Dlazdice maji hodnoty mocnin 2 (vyjma nulte mocniny). Pri doteku dvou dlazdic v jednom smeru se dlazdice spoji v jednu a hodnota se zdvojnasobi. Ukolem hrace je takto vytvorit dlazdici s hodnotou 2048 (tedy 2 na 11). Po kazdem pohybu se vygeneruje nova dlazdice s cislem 2 nebo 4 na nahodnem volnem miste. Pokud hrac provede pohyb, po kterem jiz neni kam dat novou dlazdici, hra konci. 

Je nutne mit nainstalovanou SDL knihovnu pro spusteni. https://mrlvsb.github.io/upr-skripta/c/aplikovane_ulohy/sdl.html
