# xff (indexovací nástroj)

## Zadání - Progtest

Tato semestrální práce patří do kategorie konzolových utilit. Vaším cílem je vytvořit nástroj, který uživatel z terminálu spustí (s různými parametry). Případné parametry nástroje se nesnažte definovat přímo v kódu (a to ani pomocí konstant). Umístěte je do konfiguračních souborů (jednoho nebo více), které váš program bude načítat.

Vaším úkolem je vytvořit nástroj pro indexaci a vyhledávání obsahu (souborů a dalších adresářů) v zadaném adresáři. Nástroj bude mít dvě části. První z nich bude obstarávat indexační část, tj. vytvoří vhodně reprezentovaný index. Druhá část programu umožňí nad indexem efektivně vyhledávat soubory.

Implementujte následující funcionality:

1. Pokud neexistuje index, nebo byly soubory pozměněny, nástroj na začátku svého spuštění provede indexaci. Během této fáze projde soubory, pro které to dává smysl, a zaindexuje jejich informace (vytvoří soubor/y, které budou obsahovat pomocné informace pro následné hledání). Tato část by měla jít spustit i manuálně v případě potřeby kompletní reindexace.

2. Vytvořte vhodný souborový formát a strukturu pro tento index.

   1. Zobrazte, které soubory bylo nutné přeindexovat.
   2. Pro každý soubor si bude pamatovat alespoň název, velikost a čas poslední modifikace (příp. vytvoření).

3. Pro alespoň 3 různé typy souborů implementuje speciální indexační atributy (př. textový soubor - slova a jejich frekvence, obrázek - velikost obrázku, barevný histogram, HTML dokument - seznam dokumentů, které na něj odkazují, …)

4. Umožněte vyhledávat právě podle těchto atributů (společných i individuálních), včetně různých kombinací (např. obsahuje slovo "auto" a je menší než 3KB). Implementujte vyhledávání alespoň podle dotazů:

   1. definovaných indexačními atributy,
   2. dvou dotazů ve smyslu "a zároveň platí",
   3. dvou dotazů ve smyslu "nebo platí".

5. Vyhledávání by mělo být efektivnější než manuální prohledávání všech souborů (samozřejmě bez indexační fáze, která se provádí jen pokud je to nezbytné).

## Upřesnění zadání

### Indexace

Indexuje se adresář specifikovaný v environment variable `XFF_DIR`. Soubor s uloženým indexem se nachází v adresáři, který indexuje (tj. `$XFF_DIR/.xff`). Full (úplná) indexace probíhá, pokud index neexistuje nebo je narušený (neplatný formát). V případě, že index existuje a lze úspěšně nahrát do paměti, dojde k přeindexování pozměněných souborů. To jsou soubory, u kterých od poslední indexace došlo ke změne.

Chce-li uživatel provést manuální reindexaci, spustí nástroj bez argumentů: `xff`.

V indexu jsou uložené následující údaje:

1. typ souboru
   * [0] nespecifikovaný
   * [1] TXT soubor (`.txt`)
   * [2] CSV soubor (`.csv`)
   * [3] C++ soubor (`.cpp`)
2. cesta k souboru
3. velikost
5. čas poslední modifikace

U speciálních typů souborů (txt, csv, c++) se navíc eviduje:

1. TXT soubor:
   1. počet slov
   2. 5 nejfrekventovanějších slov a jejich četnost
   3. čitelnost textového souboru (skóre Flesch-Kincaid testu)

2. CSV soubor:
   1. počet záznamů (počet řádků)
   2. konzistentnost = zda má každý záznam stejný počet sloupců (popř. kolik)
   3. počet znaků v nejdelším a nejkratším záznamu

3. C++ soubor:
   1. počet použitých C++ keywordů
   2. počet includovaných souborů (`#include` direktivů)

Index je uložený jako čitelný soubor a jednotlivé položky jsou oddělené novou řádkou. Fragment z obsah souboru `$XFF_DIR/.xff` by mohl kvalitativně vypadat takto:

```
... konec předchozího záznamu ...
1                                         // jedná se o TXT soubor
dir1/dir2/file1.txt                       // absolutní cesta k souboru je $XFF_DIR/dir1/dir2/file1.txt
13328                                     // soubor má 13328 B
2023-03-28 19:54:59.848947419 +0200       // byl naposledny modifikován
2430                                      // (txt) počet slov
ahoj 50                                   // (txt) následuje 5 nejčastějších slov
jak 43                                    // (txt)
se 21                                     // (txt)
mas 20                                    // (txt)
? 12                                      // (txt)
56                                        // (txt) Flesh-Kincaid skóre
... začátek dalšího záznamu ...
```

Každý typ záznamu v indexu má pevně daný počet řádků (určeno typem souboru).

### Vyhledávání

#### Podle názvu

* Najde soubory s názvem `file1`:

```
$ xxf name file1

1) /home/pepa/Documents/file1.txt
    file: file1.txt
    size: 13328 B
  modify: 2023-03-28 19:54:59 +0200
   words: 2430
          [50] ahoj
          [43] jak
          [21] se
          [20] mas
          [12] ?
  readability: (56.73) Fairly difficult to read.

2) /home/pepa/Desktop/file1.txt
    file: file1.txt
    size: 108 B
  modify: 2023-05-03 13:53:03 +0200
   words: 24
          [8] mama
          [8] tata
          [5] kocicka
          [3] pejsek
          [0]
  readability: (3.14) Extremely difficult to read.

3) /home/pepa/CLionProjects/file1.cpp
    file: file1.cpp
    size: 2038 B
  modify: 2022-11-09 13:53:03 +0200
keywords: 304
includes: 8

4) /home/pepa/DataGripProjects/file1.csv
    file: file1.csv
    size: 64543500 B
  modify: 2021-11-09 13:53:03 +0200
    rows: 76403
   const: Yes (12 columns)
 max_len: 84
 min_len: 21

5) /home/pepa/DataGripProjects/file1.json
    file: file1.json
    size: 95450 B
  modify: 2020-11-10 13:53:03 +0200
```

* Je možné použít regular expression: `$ xxf like fil??`

#### Podle velikosti

* Soubory o velikosti 108 B: `xxf size 108`
* Soubory o velikosti menší než 108 B: `xxf size- 108`
* Soubory o velikosti vetší než 108 B: `xxf size+ 108`

#### Podle času poslední modifikace

Soubory naposledy upravené `2023-03-28 19:54:59`: `xxf time 2023-03-28 19:54:59`
Soubory naposledy upravené před `2023-03-28 19:54:59`: `xxf time- 2023-03-28 19:54:59`
Soubory naposledy upravené po `2023-03-28 19:54:59`: `xxf time+ 2023-03-28 19:54:59`

#### Podle speciálních indexačních atributů

TXT soubory 