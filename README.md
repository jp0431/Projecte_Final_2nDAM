# Projecte_Final_2nDAM
Control d'una ciutat feta amb Unity mitjançant MQTT
Treball final de cicle
El meu treball de final de curs consisteix en controlar els semàfors i la llum dels fanals
d’una ciutat feta amb Unity, emprant una aplicació QML i que el control de la ciutat es
vegi reflectit a una placa ESP32 o similar.
Explicat amb més profunditat la idea és que els semàfors tinguin un control
automatitzat, és a dir, cada pocs segons el semàfor canvia; i el mateix passa amb els
fanals. A més a més, aquesta configuració es podrà canviar manualment des de
l’aplicació.
La placa actuaria en forma de maqueta de la ciutat, vull dir, quan s’encenguin els fanals
que s’encengui el LED blanc, quan els semàfors es posin en verd que s’encengui el LED
verd...
També s’enviarà tota la informació de la ciutat (el temps que triga en canviar el color
els semàfors, el temps de cada estat dels fanals -encès o apagat-) a una base de dades,
el Fire Base o una SQL creada amb el SQL management. Aquesta informació serà
recuperable des de l’aplicació QML.
