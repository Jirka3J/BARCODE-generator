# Barcode Generator (C)

## 1. Popis programu

Tento program slouží k generování čárových kódů ve formátu **EAN-13** nebo z uživatelem zadaného binárního řetězce. Výstupem je bitmapový soubor (.bmp), který obsahuje vykreslený čárový kód.

Program lze spustit dvěma způsoby:
- z příkazové řádky s argumenty (CLI režim)
- interaktivně (menu po spuštění bez argumentů)

---

## 2. Ovládání programu

### 2.1 Spuštění z příkazové řádky
#### help

možnost vyvolání nápovědy pomocí
```bash
barcode.exe -h
``` 
#### CLI call
Formát zadávaného příkazu.
```bash
barcode.exe <soubor> <typ> <data> <scale>
```
argument soubor je název souboru do kterého se má uložit BMP, nemusí obsahovat příponu .bmp, v případě absence se automaticky doplní

argument typ musí být jeden z podporovaných argumentů BIN/EAN13 určí typ BARCODE. BIN pouze vykreslí jednotlivé znaky jako černá(1) nebo bílá (0). EAN13 je standardizovaný typ 12 ciferný EAN-13 a vykresluje se podle standardu.

argument data samotný string dat pokud BIN tak musí být složen z '1'a '0' pokud EAN13 tak musí obsahovat přesně 12 číslic.

argument scale určuje  šířku jednotlivých čar v pixelech (doporučuju do 20 kvůli velikosti bitmapy, ale 1 je dostačující)

v případě nesprávného zadání argumentů se ozve chybová hláška

### 2.2 Interaktivní menu
#### volání
```bash
barcode.exe
```
Otevře interaktivní menu, které vás postupně provede zadáváním argumentů pro generaci BARCODE. Každý krok má k sobě komentář pro snažší průchod.
Postupně se zadává název soubor pro bmp výstup, výběr typu BARCODE, zadání dat a měřítka.

Nakonec zobrazí vygenerovaný BARCODE  i v terminálu pomocí ASCII znaků a uloží bmp.

Menu je v nekonečné smyčce která je možná ukončit vstupem 'q' vždy na začátku.

## 3 Řešení problematiky

### 3.1 EAN-13

#### Generování čárového kódu EAN-13

Formát EAN-13 je standardizovaný způsob reprezentace číselného kódu pomocí čárového kódu. Výsledný kód se skládá z 13 číslic, přičemž poslední číslice slouží jako kontrolní (checksum).
K jeho generaci slouží funkce:
```c
int generateEAN(const char* code, char** bin_code);
```


#### Výpočet kontrolní číslice

Na vstupu program přijímá 12 číslic. Nejprve dojde k jejich validaci a převodu ze znakové reprezentace na číselnou. Následně je vypočítána kontrolní číslice podle standardu EAN-13:

- číslice jsou zpracovávány zprava doleva
- sudé pozice jsou násobeny třemi
- liché pozice jsou přičítány beze změny

Výsledná kontrolní 13. číslice je odečet zbytku po dělení součtu deseti od desíti.

```c
int checkEAN(const char* code, unsigned int *ncode);
```

#### Struktura výsledného kódu

Výsledný čárový kód má pevnou binární strukturu o délce 95 bitů:

- Start a stop sekvence (`101`) slouží k synchronizaci čtečky
- Středová sekvence (`01010`) odděluje levou a pravou polovinu bitů reprezentující kódované cifry

#### Kódování číslic

Každá číslice je reprezentována sedmibitovým vzorem. Existují tři typy kódování:

- **L (left odd parity)** – používá se na levé straně
- **G (left even parity)** – používá se na levé straně
- **R (right)** – používá se na pravé straně

Pravá část kódu vždy používá R kódování. Levá část používá kombinaci L a G kódování, která je určena první číslicí kódu. Ta není přímo zakódována, ale definuje paritní vzor pro následujících šest číslic, 
ze kterého lze zpětně vyčíst

Parita je určena pomocí pevně dané tabulky, která pro každou první číslici určuje, zda se pro danou pozici použije L nebo G kódování.

#### Generování binárního řetězce

Program postupně skládá výsledný binární řetězec:

1. zapíše startovací sekvenci
2. zakóduje šest číslic levé části podle parity
3. vloží středovou sekvenci
4. zakóduje šest číslic pravé části
5. zapíše koncovou sekvenci

Výsledkem je řetězec o délce 95 znaků (`0` a `1`), který reprezentuje čárový kód.

### 3.2 Generování bitmapy (BMP)

Pro vykreslení čárového kódu do obrazové podoby je použita funkce:

```c
void save_bar_bmp(const char *filename, const char *pattern, int scale, int height, enum barcodeType type);
```
Funkce převádí binární řetězec (0 a 1) na bitmapový obrázek ve formátu BMP.

Šířka výsledného obrázku je dána délkou binárního kódu a parametrem scale, který určuje horizontální zvětšení jednotlivých pruhů. Výška je škálována stejným způsobem pro zachování proporcí.

Obraz je generován jako 24bitová bitmapa, kde každý pixel obsahuje tři bajty v pořadí BGR. Hodnota 1 v binárním řetězci reprezentuje černý pruh, zatímco 0 bílý pruh.

Při generování je nutné respektovat specifikaci BMP formátu, která vyžaduje, hlavičku souboru obsahující podpis a rozměry, zarovnání každého řádku na násobek 4 bajtů. To je zajištěno výpočtem tzv. padded width, přičemž nevyužité bajty na konci řádku jsou vyplněny nulami.

Samotné vykreslení probíhá tak, že se nejprve vytvoří jeden řádek obrazu v paměti, který je následně opakovaně zapisován do souboru pro dosažení požadované výšky čárového kódu.

Výsledkem je bitmapový soubor (.bmp)

### 3.3 Interaktivní menu programu

Program umožňuje kromě spuštění z příkazové řádky také interaktivní režim, ve kterém uživatel zadává vstupy pomocí jednoduchého textového menu.

#### Hlavní řízení menu je realizováno funkcí:

```c
int runTerminal(arguments_t* args);
```
Je hlavní funkce menu která zajišťuje postupný průchod menu a volání podfunkcí.

zajišťuje:   
zobrazení menu,
zpracování vstupu od uživatele
, naplnění struktury arguments_t, která obsahuje všechny potřebné parametry pro generování čárového kódu, Zpracování vstupu z klávesnice

#### Pro práci s klávesnicí je použita funkce:
```c
action_t selection();
```
Funkce čte stisknuté klávesy (např. šipky, Enter, 'q') a převádí je na výčtový typ (enum), který reprezentuje jednotlivé akce uživatele. Díky tomu je možné jednoduše reagovat na navigaci v menu.

#### Získání konkrétních vstupních hodnot (typ kódu, data, název souboru) probíhá pomocí funkce:
```c
void get_data(arguments_t* args);
```

vyzve uživatele k zadání vstupních údajů
provádí základní validaci dat (např. formát EAN-13 nebo binární vstup)
ukládá výsledky do struktury arguments_t

#### Pro přenos dat mezi funkcemi je použita struktura:

```c
typedef struct arguments {
    char* file;
    enum barcodeType btype;
    char* data;
    int scale;
} arguments_t;
```
Tato struktura sjednocuje všechny vstupní parametry programu a umožňuje jejich snadné předávání mezi jednotlivými částmi aplikace.

## 4 Celková struktura programu

```text
project/
├── main.c          
├── EAN13.c         //generace EAN-13 kódu
├── EAN13.h
├── barcode.c       //pomocné fce
├── barcode.h
├── bmp.c           //generace bitmapy
├── bmp.h
├── menu.c          //interaktivní menu
├── menu.h
└── CMakeLists.txt
