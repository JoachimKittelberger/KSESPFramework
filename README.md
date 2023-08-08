# KSESPFramework is a Library with useful functionality to create Applications for PlatformIO with Arduino and ESP8266 / ESP32 [![Build Status](https://travis-ci.org/JoachimKittelberger/KSESPFramework.png)](https://travis-ci.org/JoachimKittelberger/KSESPFramework) [![GitHub release](https://img.shields.io/github/tag/JoachimKittelberger/KSESPFramework.svg)](https://github.com/JoachimKittelberger/KSESPFramework/releases)

## TODO: A indeep description follows later

## Features

- [x] Works with PlatformIO
- [x] Easy to use in own projects
- [ ] TODO

## Versions
- [v0](https://github.com/JoachimKittelberger/KSESPFramework/tree/main) - Initial version.
- [v1](https://github.com/JoachimKittelberger/KSESPFramework/tree/develop) - Current developer version.

## Usage

Verwenden der Frameworkbibliothek in eigenen Projekten.

Dazu gibt es mehrere Möglichkeiten:
- Einbinden als lokale Bibliothek in einem separaten Verzeichnis
	1. In platformio.ini den Eintrag
        ```ini
        lib_extra_dirs = ${PROJECT_DIR}/../
        ```
        eintragen, falls die Biubliothek in einem Verzeichnis parallel zum Projektverzeichnis liegt. Ansonsten den relativen Pfad anpassen
	2. In platformio.ini bei den Eintrag
        ```ini
        lip_deps = KSESPFramework
        ```
        eintragen
	3. Damit wird die Lokale Version der Bibliothek ins Projekt mit eingebunden. Die Bibliothek erscheint dabei nicht unter .pio/libdeps
	4. Die Datei platformio_KSESPFrameworkConfig ins eigene Projekt kopieren und dort die Anpassungen vornehmen, welche Bibliotheken im eigenen Projekt benötigt werden.
		-  In der platformio.ini des Projekts dazu den Verweis auf diese Datei einführen mit
            ```ini
            extra_configs = platformio_KSESPFrameworkConfig.ini
            ```
        - In der kopierten ini-Datei den Abschnitt Build_flags mit den USE_Anweisungen der benutzen Bibliotheken füllen
        - In der kopierten ini-Datei den lib_deps-Abschnitt mit den verwendeten externen Bibliotheken füllen
		- Der Abschnitt lib_deps darf dabei in den verschiedenen ini-Dateien nur einmal vorkommen. Dazu sind bei weiteren Einträgen diese in einer der Dateien zusammen zu führen
- Einbinden als lokale Bibliothek mit direktem Eintrag des relativen Pfads zum Bibliotheksveerzeichnis
	1. In platformio.ini den Eintrag
        ```ini
        lib_deps = /Users/MyProjects/KSESPFramework
        ```
        bzw. den Pfad auf das lokale Verzeichnis KSESPFramework eintragen
	2. Damit wird die Lokale Version der Bibliothek als Bibliothek eingebunden. Die Bibliothek erscheint dabei unter .pio/libdeps/KSESPFramework
	3. Die Punkte unter 4. aus dem vorherigen Abschnitt müssen dann hier genauso umgesetzt werden
- Installation der Bibliothek direkt von Github
	1. In platform.ini den Eintrag
        ```ini
        lib_deps = https://github.com/JoachimKittelberger/KSESPFramework.git
        ```
        eintragen.
	2. Damit wird die auf github verfügbare letzte Version der Bibliothek als Bibliothek eingebunden. Die Bibliothek erscheint dabei unter .pio/libdeps/KSESPFramework
	3. Die Punkte unter 4. aus dem vorherigen Abschnitt müssen dann hier genauso umgesetzt werden
- Einbindung aller Klassen aus dem Framework
	```ini
    build_flags =
		-D USE_PLATFORMIO_INI_CONFIG=1      ; Use Config in Platformio.ini
		-D KSLIBRARIES_USEALL=1
    ```
    Kann auf die anderen Definitionen der verwendeten Klassen verzichtet werden. Dabei müssen dann aber alle externen Bibliotheksabhängigkeiten unter lib_deps mit angegeben werden, wie im Template vorgegeben
- Einbinden ausgewählter Klassen aus dem Framework
    ```ini
    build_flags =
	    -D USE_PLATFORMIO_INI_CONFIG=1      ; Use Config in Platformio.ini  
    ;   -D KSLIBRARIES_USEALL=1
	    -D USE_KSAppInfo=1
	    -D USE_KSBattery=1
    ```
    Hier werden nur die Klassen KSAppInfo und KSBattery eingebunden. In dem Fall müssen auch nur diejenigen externen Bibliotheken unter lib_deps angegeben werden, die für diese Klassen benötigt werden. In den Kommentaren der Bibliotheken steht am Ende, für welche Klassen diese externen Bibliotheken benötigt werden. Alle anderen können gelöscht oder mit `";"` auskommentiert werden.
- Einbinden der Header-Datei aus dem Framework in main.cpp
  - In main.cpp der eigenen Applikation muss ```#include "KSESPFramework.h"``` eingetragen werden.
  - die Datei ```include/KSCredentials.h.tpl``` aus dem Framework in das include-Verzeichnis der eigenen Applikation kopieren und in ```myCredentials.h``` umbenennen.
  - Anschließend die Inhalte der Datei ```myCredentials.h```an die eigenen Gegebenheiten anpassen 
  - Wird der Framework lokal auf der Festplatte installiert, kann hier auf die Original-Datei ```include/KSCredentials.h``` im Framework verwiesen werden, die zuvor aus der Datei ```include/KSCredentials.h.tpl``` erzeugt werden muss..





## Installation

Clone this repository:

```bash
$ git clone https://github.com/JoachimKittelberger/KSESPFramework
```

Or install with [clib](https://github.com/clibs/clib):

```bash
$ clib install JoachimKittelberger/KSESPFramework
```

Or use PlatformIO-Library Manager to install in your specific project

TODO



## License

MIT - Joachim Kittelberger - KibeSoft, [www.kibesoft.de](https://www.kibesoft.de)
