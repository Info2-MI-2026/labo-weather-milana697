# Fichiers Météo

- **Durée**: 4 périodes
- **Format**: travail individuel

### Table des matières

- [Fichiers Météo](#fichiers-météo)
    - [Table des matières](#table-des-matières)
  - [Contexte](#contexte)
  - [Fichier de données météorologiques](#fichier-de-données-météorologiques)
  - [Déroulement](#déroulement)
  - [Structure de donnée](#structure-de-donnée)
  - [Programme](#programme)
    - [Version](#version)
    - [Help](#help)
    - [Sortie binaire](#sortie-binaire)
    - [Affichage du graphique](#affichage-du-graphique)
  - [Erreur et options](#erreur-et-options)
  - [Rendu du labo](#rendu-du-labo)

## Contexte

On vous demande de concevoir un programme capable de traiter des données météorologiques. Votre programme doit pouvoir agréger ces données en des statistiques annuelles lesquelles seront la température moyenne et les précipitations annuelles.

## Fichier de données météorologiques

Un exemple de fichier d'entrée est accessible à [assets/weather-bern.txt](assets/weather-bern.txt). Il s'agit des relevés météorologiques de la ville de Berne depuis 1864. Notons que le format de ce fichier peut varier. Vous remarquez que ce fichier scindé en deux parties:

- Un en-tête
- Des données

Sachez que l'en-tête peut varier légèrement, c'est-à-dire que le nombre de lignes vide peut changer et l'alignement des champs peut varier aussi.

Concernant les données, elles sont **toujours** exprimées sur quatre colonnes avec comme information:

1. L'année de la mesure.
2. Le mois de la mesure.
3. La température moyenne sur le mois en °C.
4. Les précipitations cumulées en millimètres sur le mois.

Notons deux informations importantes :

- les lignes du fichier ne dépassent jamais 80 caractères ;
- le nombre d'années ne dépasse jamais un demi-millénaire par fichier.

## Déroulement

Pour bien réussir ce travail pratique, il vous est proposé de respecter les étapes suivantes:

1. Lire la donnée en entier.
2. Prendre connaissance du fichier `weather-bern.txt`.
3. Identifier les deux parties distinctes.
4. Trouver un algorithme pour ignorer l'en-tête.
5. Écrire un premier programme qui écrit sur `stdout` chaque valeur lue.
6. Ajouter à votre programme les structures de données proposées.
7. Lire le fichier dans la structure de donnée.
8. Écrire la fonction `fprint_csv`.
9. Écrire la fonction `fprint_binary`.
10. Gérer les arguments (`-o`, ...) et le fichier d'entrée.
11. Gérer l'aide et la version.
12. Tester votre programme.
13. Faire un `commit` et un `push` de votre code.
14. Afficher un graphique du résultat dans un tableur.
15. Contrôler la sortie binaire avec `hexdump` ou `xxd`.

## Structure de donnée

Pour faciliter le stockage des données, il vous est demandé de créer un tableau de données agencé comme suit:
- Années
  - 2012
    - Mois
      - Janvier
        - Température moyenne
        - Précipitations cumulées
      - Février
        - Température moyenne
        - Précipitations cumulées
      - ...
    - Température moyenne annuelle
    - Précipitations cumulées
  - 2013
  - ...

Votre collègue sympathique vous a donné ce code:

```c 
typedef struct {     
  double temperature;
  double precipitations;
} WMonth;

typedef struct {     
  int year;
  WMonth months[MONTHS];
  double temperature;
  double precipitations;
} WYear;

typedef struct {
  WYear years[MAX_ENTRIES];
  int start;
} WData;
```

## Programme

Le programme que vous réaliserez s'appellera `app`, afin d'avoir un nom simple pour
le développement. Par la suite, vous pouvez modifier le nom dans le `Makefile`, ceci
pour avoir une application avec un nom plus réaliste comme `weather-analyser`. 

Pour le compiler vous disposez d'un `Makefile`, donc la commande `make` vous permet de directement compiler votre programme.

Votre programme calculera automatiquement des statistiques sur le fichier d'entrée:

- Total annuel des précipitations
- Température moyenne de l'année

Les résultats sont affichés soit sur la sortie standard, soit dans un fichier passé en paramètres.

### Version

Comme d'habitude, votre programme devra renseigner sa version:

```console ./weather-analyser --version
Version 0.0.1 Copyright(c) HEIG-VD
```

### Help

Une aide permettra au profane de comprendre le fonctionnement du programme:

```console $ ./weather-analyser --help
USAGE: ./weather-analyser [options] [filename]

This program processes weather data issued from the internet.
It reads from [filename] or if missing, directly from STDIN.

The output format is CSV compliant.

OPTIONS:

--version      Shows the version
--help         Shows this help dialog

--binary, -b   Output in binary mode, not CSV

-o<filename>   Write output on <filename> ```

### Sortie CSV

Le fichier de sortie est par défaut au format [CSV](https://en.wikipedia.org/wiki/Comma-separated_values). Voici un exemple de sortie:

```console $ ./weather-analyser assets/weather-bern.txt | head -n10 
year;temperature;precipitations
1864;6.125;603.9
1865;7.725;689.7
1866;8.19167;949.2
1867;7.43333;1100.5
1868;8.39167;947.2
1869;7.44167;912.2
1870;6.84167;1016.8
1871;6.3;653.7
1872;8.29167;944.3
```

On appelle donc le programme avec un argument, lequel est le nom du fichier de donnée. Dans l'exemple donné, la sortie est passée au programme `head` qui affiche que les 10 premières lignes du résultat. On y voit trois valeurs par lignes: l'année, la moyenne de température annuelle, la somme des précipitations annuelle.

### Sortie binaire

En mode binaire, le fichier de sortie est le suivant:

- Un en-tête comportant `WEATHER`
- 4 bytes pour l'année
- 4 bytes pour la précipitation (entier 32 bits multiplié par 100)
- 4 bytes pour la température (entier 32 bits multiplié par 10)

Voici un exemple de fonctionnement:

```console $ ./weather-analyser assets/weather-bern.txt --binary | hexdump -C -n100 00000000  57 45 41 54 48 45 52 48  07 00 00 64 02 00 00 97  |WEATHERH...d....|
00000010  17 00 00 49 07 00 00 04  03 00 00 f1 1a 00 00 4a  |...I...........J|
00000020  07 00 00 33 03 00 00 14  25 00 00 4b 07 00 00 e7  |...3....%..K....|
00000030  02 00 00 fd 2a 00 00 4c  07 00 00 47 03 00 00 00  |....*..L...G....|
00000040  25 00 00 4d 07 00 00 e8  02 00 00 a2 23 00 00 4e  |%..M........#..N|
00000050  07 00 00 ac 02 00 00 b8  27 00 00 4f 07 00 00 76  |........'..O...v|
00000060  02 00 00 88
00000064
```

### Affichage du graphique

Une fois votre implémentation terminée, vous pouvez ouvrir le fichier `.csv` avec Microsoft Excel ou tout autre tableur.

À partir des données. Affichez deux graphiques de l'évolution des températures et des précipitations au cours des années. Voici un exemple de ce que vous devriez pouvoir afficher:

![graph](assets/graph.png)

## Erreur et options
- Votre programme doit gérer les erreurs d'arguments.
- En cas d'erreur, le programme retourne un code d'erreur 1.
- Les options `--version` et `--help` sont prioritaires sur les autres options. Si l'une de ces options est présente, le programme doit afficher le message correspondant et retourner 0.

## Rendu du labo
Commit Github
