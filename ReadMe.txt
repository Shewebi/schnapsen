/* Hinweise zum Kompilieren */
Zum Kompilieren nutze ich normalerweise G++, dementsprechend
setzen auch die Befehle im beiliegenden Batchskript auf G++
bzw. hier konkret auf die MinGW-Variante, denn Game.hpp
bindet windows.h ein.
Die vorkompilierte Schnapsen.exe sollte aber problemlos laufen.

/* Regeln */
Die Regeln sind anders als die, die man z.B. in dem Wikipedia-
artikel findet, stattdessen sind sie, wie ich das Spiel von 
meiner Familie kenne, nur der Einfachheit halber mit den hier
üblichen Farben.

- Am Anfang ziehen beide Spieler 4 Karten
- In der ersten Runde fängt Spieler 1 an, danach abwechselnd
- Wer als letztens gestochen hat, spielt zuerst eine Karte
- Gestochen wird folgendermaßen:
    - Bei gleichen Farben sticht immer der höhere Wert
    - Bei verschiedenen Farben sticht Trumpf, sonst die
      zuerst gelegte Karte
- Wer 66 Punkte hat oder den letzten Stich einer Runde hat,
  gewinnt die Runde und erhält
    - 6 Punkte, wenn der andere Spieler 0 Punkte hat
    - 4 Punkte, wenn der andere Spieler weniger als 33 Punkte hat
    - 2 Punkte, wenn der andere Spieler mehr als 33 Punkte hat