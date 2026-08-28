/*

# Challenge 1 – Ethernet Frame Parser

## Ziel

Einen Ethernet-Frame als `unsigned char`-Array in C untersuchen und seine Bestandteile direkt aus den Bytes auslesen – ohne fertige Ethernet-Library.

## Ethernet-Frame

```text
6 Byte   Ziel-MAC
6 Byte   Quell-MAC
2 Byte   EtherType
Payload
FCS
```

Bei VLAN kommt vor dem eigentlichen EtherType ein 4-Byte-Tag hinzu:

```text
0x8100 + VLAN-Informationen
```

Beispiele für EtherType:

```text
0x0800 → IPv4
0x86DD → IPv6
0x0806 → ARP
0x0842 → Wake-on-LAN
```

## Umsetzung

Ich habe Frames aus Wireshark als C-Array verwendet:

```c
unsigned char eth_fr[] = { ... };
```

Die wichtigsten Positionen:

```text
Bytes 0–5   → Ziel-MAC
Bytes 6–11  → Quell-MAC
Bytes 12–13 → EtherType
```

Den EtherType habe ich selbst aus zwei Bytes zusammengesetzt:

```c
eth_type = eth_fr[12] * 0x0100 + eth_fr[13];
```

Dabei habe ich Big Endian und die Byte-Reihenfolge besser verstanden. Bei VLAN verschiebt sich die Position des EtherType.

Getestet habe ich ARP-, IPv4- und IPv6-Frames mit unterschiedlichen Längen.

## Padding

Ursprünglich dachte ich, dass `0x00` automatisch Padding bedeutet. Das stimmt nicht allgemein. In meinen Wireshark-Testdaten waren die Padding-Bytes zwar `0x00`, daraus kann ich aber keine allgemeine Ethernet-Regel ableiten.

## FCS / CRC

Ein vollständiger Ethernet-Frame enthält am Ende einen 4-Byte-FCS. Die verwendeten Wireshark-Arrays enthalten diese Bytes nicht. Deshalb muss ich zwischen dem vollständigen Frame auf dem Medium und den aufgezeichneten Capture-Daten unterscheiden.

CRC/FCS implementiere ich später als eigene Challenge.

## Erkenntnisse

Die Challenge hat mehrere Bereiche verbunden:

```text
C:          Arrays, unsigned char, sizeof, Indexierung
Computer:   Bytes, Hexadezimalzahlen, Big Endian
Netzwerk:   MAC, EtherType, VLAN, Payload, Padding, FCS
```

Ich verstehe Ethernet-Frames jetzt nicht mehr nur als fertige Struktur, sondern als Folge von Bytes im Speicher.

## Offene Punkte

* FCS/CRC selbst implementieren
* Frame-Längen und Gültigkeit prüfen
* Parser später mit echten Hardware-Daten verwenden

## Ergebnis

**Challenge 1 abgeschlossen.**

```text
C → Bytes → Ethernet-Frame → Hardware → Bare-Metal-Ethernet
```


*/

#include <stdio.h>
#include <stdlib.h>

int main()
{

    unsigned char eth_fr[] = {0x33, 0x33, 0xff, 0x24, 0x45, 0x48, 0xac, 0x1f, 0x6b, 0x18, 0xe8, 0xee, 0x86, 0xdd, 0x60, 0x0, 0x0, 0x0, 0x0, 0x20, 0x3a, 0xff, 0xfe, 0x80, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0xde, 0xac, 0x5f, 0x1, 0x51, 0xb0, 0xa3, 0x87, 0xff, 0x2, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x1, 0xff, 0x24, 0x45, 0x48, 0x87, 0x0, 0x8d, 0xa6, 0x0, 0x0, 0x0, 0x0, 0xfe, 0x80, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x11, 0x32, 0xff, 0xfe, 0x24, 0x45, 0x48, 0x1, 0x1, 0xac, 0x1f, 0x6b, 0x18, 0xe8, 0xee};
    /*
    * Ziel MAC-Adresse: ff:ff:ff:ff:ff:ff
    * Quelle MAC-Adresse: 18:a9:5:b9:c7:e7
    * EtherType: Address Resolution Protocol (0x0806)
    * Padding = 18
    * Payload Size = 28
    */
    
    /*
    unsigned char eth_fr[] = {0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x18, 0xa9, 0x5, 0xb9, 0xc7, 0xe7, 0x8, 0x6, 0x0, 0x1, 0x8, 0x0, 0x6, 0x4, 0x0, 0x1, 0x18, 0xa9, 0x5, 0xb9, 0xc7, 0xe7, 0xc0, 0xa8, 0x4d, 0x60, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0xc0, 0xa8, 0x4d, 0xe9, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0};

    * Ziel MAC-Adresse: ff:ff:ff:ff:ff:ff
    * Quelle MAC-Adresse: 18:a9:5:b9:c7:e7
    * EtherType: Address Resolution Protocol (0x0806)
    * Padding = 18
    * Payload Size = 28
    */
    
    /*

    unsigned char eth_fr[] = {0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x0, 0x50, 0x56, 0xb8, 0x75, 0x7f, 0x8, 0x0, 0x45, 0x0, 0x0, 0x47, 0x94, 0x69, 0x0, 0x0, 0x40, 0x11, 0x95, 0x2, 0x14, 0x14, 0x14, 0x14, 0x14, 0x14, 0x14, 0xff, 0x36, 0xaf, 0x13, 0xbf, 0x0, 0x33, 0xa6, 0xce, 0x0, 0x50, 0x56, 0xb8, 0x75, 0x7f, 0xb8, 0x66, 0xdf, 0x69, 0xba, 0xc, 0x1b, 0x7e, 0xd8, 0x61, 0x2e, 0x15, 0x0, 0x1, 0x0, 0x1, 0x0, 0x1, 0x0, 0x0, 0x0, 0x0, 0x0, 0x11, 0x43, 0x99, 0x0, 0x1, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x8, 0x39};
    
    * Ausgaben:
    * Ziel MAC-Adresse: ff:ff:ff:ff:ff:ff
    * Quelle MAC-Adresse: 0:50:56:b8:75:7f
    * EtherType: IPv4 (0x0800)
    * Payload Size = 71 
    */
    
    /*
    
    unsigned char eth_fr[] = {0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x0, 0x60, 0xb5, 0x3a, 0x42, 0x50, 0x8, 0x0, 0x45, 0x0, 0x0, 0x3c, 0x1a, 0x53, 0x0, 0x0, 0xff, 0x11, 0x98, 0x2b, 0xc0, 0xa8, 0x48, 0x8a, 0xff, 0xff, 0xff, 0xff, 0x1b, 0xb2, 0x1b, 0xb3, 0x0, 0x28, 0xa3, 0xbe, 0x23, 0x32, 0x30, 0x30, 0x32, 0x38, 0x31, 0x66, 0x38, 0x20, 0x69, 0x70, 0x20, 0x72, 0x65, 0x6a, 0x65, 0x63, 0x74, 0x65, 0x64, 0x20, 0x3a, 0x20, 0x31, 0x34, 0x31, 0x34, 0x31, 0x34, 0x31, 0x34};

    * Ausgabe des Programs   
    * Ziel MAC-Adresse: ff:ff:ff:ff:ff:ff
    * Quelle MAC-Adresse: 0:60:b5:3a:42:50
    * EtherType: IPv4 (0x0800)
    * Payload Size = 60
    */
    size_t fr_size = sizeof(eth_fr) / sizeof(eth_fr[0]);
    

    // Possible TODO: checks for valid frame, length, etc

    printf("Ziel MAC-Adresse: %x:%x:%x:%x:%x:%x\n", eth_fr[0], eth_fr[1], eth_fr[2], eth_fr[3], eth_fr[4], eth_fr[5]);
    printf("Quelle MAC-Adresse: %x:%x:%x:%x:%x:%x\n", eth_fr[6], eth_fr[7], eth_fr[8], eth_fr[9], eth_fr[10], eth_fr[11]);

    unsigned int eth_type  = 0x0000;
    unsigned int header_size  = 14;


    if (eth_fr[12] == 0x81 && eth_fr[13] == 0x00) {
        printf("Frame Tagged with IEEE 802.1Q\n");
        
         eth_type = eth_fr[16] * 0x0100 +  eth_fr[17];
         header_size  = 18;
    } else {
        eth_type = eth_fr[12] * 0x0100 +  eth_fr[13];
    }

    switch(eth_type) {
        case 0x0800: 
            printf("EtherType: IPv4 (0x%04x)\n", eth_type); break;
        case 0x86dd: 
            printf("EtherType: IPv6 (0x%04x)\n", eth_type); break;
        case 0x0806:
            printf("EtherType: Address Resolution Protocol (0x%04x)\n", eth_type); break;
        case 0x0842:
            printf("EtherType: Wake on LAN (WoL) (0x%04x)\n", eth_type); break;
        default:
            printf("Unerkanntes EtherType"); break;
    }

    //PAD-Feld aufs 0 setzen
    size_t pad = 0;
   
    // weniger als 64 Bytes kann die Framelänge nicht sein.
    if (fr_size <= 64) {
        // find heraus ob es ein Füllfeld gibt, wie lang ist es.
        // IEEE 802.3 sagt, das die Daten mit die meist significant bytes erst übertragt werden. => Man sicher die Anzahl von Nulls als Padding zählen kann.
        // Korrektur: zu oben: Nulls sind die Paddings immer nicht, die Spezifikation sagt nicht was die Padding sein soll. Hier nur in Wireshark waren die Paddings als Null bezeichnet.


        // 59 weil, 0 index, und 64 - 4 - 1 = 59. -4 für 4 FCS (frame checksum) Bytes CRC (Cyclic Redundency Check) 
        for (; eth_fr[59 - pad] == 0x00; pad++) ;
        

        printf("Padding = %lu\n", pad);
    }

    size_t payload = fr_size - header_size - (fr_size == 64 && eth_fr[63] != 0x00 /* CRC 4 Bytes sind vorhanden */ ? 4 : 0) - pad; 
    printf("Payload Size = %lu\n", payload);

    return EXIT_SUCCESS;
}