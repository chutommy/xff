# xff (indexovací nástroj)

## Zadání - Progtest

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

## Specifikace (upřesnění) zadání

### Indexace

Vždy se indexuje pracovní adresář `$PWD` (adresář, ve kterém se uživatel nachází). Indexový soubor se nachází v adresáři, který indexuje (tj. `$PWD/.xff`). Kompletní (úplná) reindexace probíhá, pokud index neexistuje, je narušený (má obsah v neplatném formátu) nebo v případě, že ji uživatel manuálně spustí. Pokud index existuje, ale není aktuální, dochází k částečné indexaci, tj. indexace nových souborů, smazání odstraněných a reindexace modifikovaných. Soubory se indexují rekurzivně i ve vnořených adresářů, indexační soubor je nicméně dostupný jen v daném adresáři.

Chce-li uživatel provést manuální reindexaci, spustí příkaz reset: `xff reset`, pro aktualizace existujícího (nebo vytvoření neexistujícího) indexu, stačí spustit nástroj bez argumentů: `xff`. Pro zobrazení help zprávy zadá uživatel příkaz help: `xxf help` (po neplatném použití nástroje se uživateli tento příkaz nabídne).

V indexu jsou uložené následující údaje:

1. název a adresa souboru
2. velikost v bajtech
3. čas poslední modifikace

Tyto atributy se evidují u všech souborů. U souborů `txt`, `csv` a `cpp` se navíc indexuje speciální atributy:

1. textový soubor:
    1. počet slov
    2. čitelnost obsahu (skóre Flesch-Kincaid testu)
    3. 5 nejpoužívanějších slov a jejich četnost


2. csv soubor:
    1. počet záznamů


3. C++ soubor:
    1. počet použitých C++ keywordů
    2. seznam includovaných souborů

Indexují se pouze takzvané `regular files`, tzn. adresáře, symbolické linky, sockety, či jiné Linux soubory spadající do kategorie `special files` (character, FIFO, descriptor, system console, ...) nástroj ignoruje.

Index je uložený jako čitelný soubor a jednotlivé položky jsou oddělené novou řádkou. Fragment z obsahu souboru `$XFF_DIR/.xff` vypadá takto:

```
... konec předcházejícího záznamu ...
"./hello.txt"                             // relativní adresa souboru
143237                                    // velikost souboru v B(ytes)
2023-05-14 12:10:09                       // čas poslední modifikace
23237                                     // počet slov
31.8157                                   // Flesh-Kincaid skóre
a 6 the 6 she 6 sees 5 is 2               // nejčastější slova a příslušná četnost
"./pa2/pa2-semestral/src/main.cpp"        // další soubor
1273                                      // ...
2023-05-14 12:10:09
930
algorithm cassert cctype cmath cstdio
  cstdlib cstring functional iomanip
  iostream list map memory set sstream
  string type_traits typeinfo unistd.h
  unordered_map unordered_set vector
... začátek následujícího záznamu ...
```

### Vyhledávání

#### Podle společných atributů

* Soubory s názvem: `xxf name file1.json`
* Soubory s názvem odpovídající regular expression: `xxf like ".*myfile.*"`:
* Soubory o velikosti rovno/menší než/větší než v B: `xxf size/size-/size+ 108`
* Soubory naposledy upravené rovno/před/po: `xxf time/time-/time+ 2022-11-09 13:53:03`

Příklad dotazu na všechny soubory:

```
$ xxf like ".*"

=== Search results ===

1) "/home/chutommy/CLionProjects/pa2-semestral/playground/./data2.json"
        File: "data2.json"
        Type: JSON
        Size: 43
      Modify: 2023-05-14 12:10:09

2) "/home/chutommy/CLionProjects/pa2-semestral/playground/./main.cpp"
        File: "main.cpp"
        Type: CPP
        Size: 1507
      Modify: 2023-05-15 01:03:39
    Keywords: 10
    Includes: CPPFile.h CSVFile.h DoubleTerm.h File.h IntTerm.h 
              StringTerm.h TXTFile.h Timestamp.h TimestampTerm.h filesystem 
              iostream string vector 

3) "/home/chutommy/CLionProjects/pa2-semestral/playground/./paper.txt"
        File: "paper.txt"
        Type: TXT
        Size: 7024
      Modify: 2023-05-15 01:02:20
       Words: 1066
         (72) the
         (38) of
         (30) and
         (27) quantum
         (25) a
 Readability: 66.8658

4) "/home/chutommy/CLionProjects/pa2-semestral/playground/./heights.csv"
        File: "heights.csv"
        Type: CSV
        Size: 3734
      Modify: 2023-05-15 01:00:39
        Rows: 201


=== Search complete. Matched files: [4/4] ===
```

#### Podle individuálních atributů

* TXT soubory s počtem slov rovno/méně než/více než: `xxf words/words-/words+ 42`
* TXT soubory se slovem v top 5 nejčastějších: `xxf top hello`
* TXT soubory s čítelností rovno/méně než/více než: `xxf readability/readability-/readability+ 50.00`


* CSV soubory s počtem záznamů rovno/méně než/více než: `xxf rows/rows-/rows+ 42`


* CPP soubory s počtem keywordů rovno/méně než/více než: `xxf keyword/keyword-/keyword+ 42`
* CPP soubory s includovanou knihovnou: `xxf include vector`

#### Spojování dotazů

* a zároveň: `xxf name file.txt and size 30 and words- 6`
    * soubory s názvem `file.txt` mající velikost `30 B` a méně než `6 slov`


* nebo: `xxf name file.txt or size 30 or words- 6 `
    * soubory s názvem `file.txt` nebo s velikostí `30 B` nebo mající méně než `6 slov`

Typy spojek nelze kombinovat (povolené jsou dotazy spojující pouze `and` nebo pouze `or`).

## Kde mám polymorfismus?

Polymorfismus používám především u trídy `File` reprezentující soubor, ze které dědí třídy `CPPFile`, `CSVFile` a `TXTFile` (speciální soubory). Po vytvoření jednotlivých instancí se na soubory již odkazuji přes rodičovskou třídu `File`, která má (pokud to dědící třída vyžaduje) příslušně přetížené metody `print`, `store` a odpovídající filtrační metody.

Polymorfická instance `File` se také předává jako vstup metodě `evaluate` třídy `Query`, která vyhodnocuje, zda daný soubor splňuje specifikovaný dotaz, přes virtuální metody `File`. Samotná třída `Query` (reprezentující jeden logický term) je virtuální a dědí z ní napříklady třídy `RegexNameQuery`, `SizeQuery`, `WordCountQuery`, které virtuální metodu `evaluate` přetěžují tak, aby ověřovaly specifickou podmínku s určitým parametrem (regularní výraz, velikost, počet slov). 

Dotazy se slučují v ultimátní virtuální třídě `MainQuery`, která drží jednotlivé atomické dotazy. Může být instancí buď `ANDQuery` nebo `ORQuery`, podle toho, zda uložené termy spojuje logickou konjunkcí nebo disjunkcí. Jedná se o podtřídu `Query`, tedy opět vyhodnocuje přes přetíženou metodu `evaluate`. Atomické dotazy `Query` se dají vkládat přes virtuální metodu `add`. Chová se tedy opět polymorficky (pracuji s `MainQuery`, se kterou pracuji nezávisle na tom, zda se jedná o `ANDQuery` nebo `ORQuery`).
