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

## Specifikace (upřesnění) zadání

### Indexace

Indexuje se adresář specifikovaný v environment variable `XFF_DIR`. Soubor s uloženým indexem se nachází v adresáři, který indexuje (tj. `$XFF_DIR/.xff`). Full (úplná) indexace probíhá, pokud index neexistuje nebo je narušený (má obsah v neplatném formátu). V případě, že index existuje a lze úspěšně nahrát do paměti, dojde k přeindexování pozměněných souborů. To jsou soubory, které jsou nové, byly odstraněny, nebo u kterých od poslední indexace došlo ke změně.

Chce-li uživatel provést manuální reindexaci, spustí nástroj bez argumentů: `xff`.

V indexu jsou uložené následující údaje (v hranatých závorkách je zobrazen formát):

1. typ souboru `[string]`
    * `txt` = TXT soubor
    * `csv` = CSV soubor
    * `cpp` = CPP soubor
    * `*` = ostatní
2. název souboru `[string]`
3. cesta adresáře, ve kterém soubor existuje `[string/.../string]`
4. velikost v bajtech `[int]`
5. čas poslední modifikace `[time]`

Tyto atributy se evidují u všech souborů. Nicméně existují i speciální typy souborů (txt, csv, cpp), u kterých navíc indexuje speciální atributy:

1. TXT soubor:
    1. počet slov `[int]`
    2. čitelnost obsahu (skóre Flesch-Kincaid testu) `[float]`
    3. 5 nejpoužívanějších slov a jejich četnost `[string (int), ...]`


2. CSV soubor:
    1. počet záznamů (řádků) `[int]`


3. CPP soubor:
    1. počet použitých C++ keywordů `[int]`
    2. seznam includovaných souborů (#include direktivů) `[int, string, ...]`

Index je uložený jako čitelný soubor a jednotlivé položky jsou oddělené novou řádkou. Fragment z obsahu souboru `$XFF_DIR/.xff` by mohl vypadat takto:

```
... konec předcházejícího záznamu ...
txt                                       // typ souboru
file1.txt                                 // název souboru
dir1/dir2                                 // relativní cesta od $XFF_DIR/ k rodičovskému adresáři
13328                                     // velikost v B(ytes)
2023-03-28 19:54:59 +0200                 // naposledny modifikován
2430                                      // (txt) počet slov
56.73                                     // (txt) Flesh-Kincaid skóre
... začátek následujícího záznamu ...
```

### Vyhledávání

#### Podle společných atributů

* Soubory s názvem: `xxf name file1.json`
* Soubory s názvem odpovídající regular expression: `xxf like file1.*`:
* Soubory o velikosti rovno/menší než/větší než v B: `xxf size/size-/size+ 108`
* Soubory naposledy upravené rovno/před/po: `xxf time/time-/time+ 2022-11-09 13:53:03`

Možný výstup:

```
$ xxf like file1.*

1) ====== /home/pepa/Documents/file1.txt                        // TXT soubor
       file: file1.txt
       type: TXT
       size: 13328 B
     modify: 2023-03-28 19:54:59 +0200
      words: 2430
        (68) ahoj
        (32) jak
        (20) se
        (13) mas
         (6) ?
readability: (56.73) Fairly difficult to read.

2) ====== /home/pepa/Desktop/for-kids/file1.txt                 // TXT soubor
       file: file1.txt
       type: TXT
       size: 108 B
     modify: 2023-05-03 13:53:03 +0200
      words: 24
        (10) mama
         (8) tata
         (4) kocicka
         (2) pejsek
         (0)
readability: (3.14) Extremely difficult to read.

3) ====== /home/pepa/CLionProjects/file1.cpp                    // CPP soubor
       file: file1.cpp
       type: CPP
       size: 2038 B
     modify: 2022-11-09 13:53:03 +0200
   keywords: 304
   includes: <cstdlib>,<cstdio>,<cassert>,<cctype>,<cmath>,
             <iostream>,<iomanip>,<sstream>,<set>,<list>,
             <map>,<vector>,<queue>,<string>,<stack>,
             <queue>,<deque>,<algorithm>,<unordered_map>,<unordered_set>,
             <memory>,<functional>,<iterator>,<stdexcept>

4) ====== /home/pepa/DataGripProjects/file1.csv                 // CSV soubor
       file: file1.csv
       type: CSV
       size: 64543500 B
     modify: 2021-11-09 13:53:03 +0200
       rows: 76403

5) ====== /home/pepa/DataGripProjects/file1.json                // ostatní soubory
       file: file1.json
       type: JSON
       size: 95450 B
     modify: 2020-11-10 13:53:03 +0200
```

#### Podle individuálních atributů

* TXT soubory s počtem slov rovno/méně než/více než: `xxf words/words-/words+ 42`
* TXT soubory se slovem v top 5 nejčastějších: `xxf top hello`
* TXT soubory s čítelností rovno/méně než/více než: `xxf readability/readability-/readability+ 50.00`


* CSV soubory s počtem záznamů rovno/méně než/více než: `xxf rows/rows-/rows+ 42`


* CPP soubory s počtem keywordů rovno/méně než/více než: `xxf keywords/keywords-/keywords+ 42`
* CPP soubory s includovanou knihovnou: `xxf include vector`

#### Spojování dotazů

* a zároveň: `xxf name file.txt && size 30 && words- 6`
    * soubory s názvem `file.txt` mající velikost `30 B` a méně než `6 slov`


* nebo: `xxf name file.txt || size 30 || words- 6 `
    * soubory s názvem `file.txt` nebo s velikostí `30 B` nebo mající méně než `6 slov`

Typy spojek nelze kombinovat (povolené jsou dotazy pouze s && nebo pouze s ||).

#### Help

Help se zobrazení po spuštění nástroje s argumentem help: `xxf help` nebo po zadání neplatného dotazu: `xxf párek.json` (místo `xxf name párek.json`).

## Kde mám polymorfismus?


