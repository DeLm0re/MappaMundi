Normes de codage et conventions pour le projet technique
=

## Langage du code :

Tout le code rédigé pour le projet sera écrit dans la langue de Shakespeare, l'Anglais (ce n'est pas le cas pour les exemples de ce document). Cela comprend aussi bien le code, que les commentaires de celui-ci. <br>
À noté aussi que chaque instructions est collée à la conditions entre parenthese qui l'a suit. 

Pour exemple :

```C
//On écrira :
if(uneVariable == uneAutreVariable)

//et pas :
if (uneVariable == uneAutreVariable)
```

Dans le même principe, chaque clauses qui nécessite un bloc de code, sera suivit d'une accolade ouvrante et fermante *après un retour à la ligne*, même si ce bloc de code ne contient qu'une seule instruction.

Exemple à suivre :

```C
//On écrira :
for(i = 0; i < 10; i++)
{
    unTableau[i] = 0;
}

//et pas :
for (i = 0; i < 10; i++)
    unTableau[i]=0;

//ou encore :
for(i = 0;i < 10; i++) {
    unTableau[i] =0;
    unDeuxiemeTableau[i]= 0;
}
```

On notera dans cet exemple l'apparition d'une norme qui n'a pas encore été souligné. En effet, on devra respecter le fait de mettre un espace avant et après chaque opérateurs (+ - / * = < >).

## Les tabulations :

Le code et par conséquent, les bloc d'instructions, seront indentés grâce à des tabulations de *4 espaces*.

Illustration dans un exemple :

```C
//On écrira :
if(uneVariable == UNECONSTANTE)
{
    if(monMode == 3)
    {
        put("Vous êtes en mode 3");
    }
}

//et pas :
if(uneVariable == UNECONSTANTE)
{
        if(monMode == 3)
        {
        put("Vous êtes en mode 3");
        }
}
```

## Les déclarations de variables :

Les déclarations seront faites en respectant la convention ***Camel case***. Celle-ci stipule que chaque mots soient liés sans espace et que chaqun d'entre eux commence par une majuscule à l’exception du premier.

Voici quelques exemples :

```C
int monNomDeVariable = maNouvelleValeur;

monUrl = "https://unurl.fr"
```
>C’est surement la convention la plus utilisée. On la retrouve en JavaScript,en Java, C++, C# et bien plus. C’est également la forme la plus rependu dans les hashtag Twitter. cette convention est aussi utilisée par certaine marque pour afficher leur appartenance au monde numérique comme c’est le cas pour eBay et iPhone. Quand on utilise des acronymes (URL, CSS, HTML, etc…) en Camel Case, seule la première lettre de ces derniers sont à mettre en majuscule. <br>
*https://wprock.fr/blog/conventions-nommage-programmation/*

## Les déclarations de constantes :

Les déclarations de constantes doivent respecter une convention définie par l'équipe. Elle pose le fait que chaque constantes soient définies en majuscules. La principale raison qui explique l'application de cette convention est le fait que celle-ci permet de ne pas confondre les constantes et les variables, qui comme indiqué plus haut, sont sous la convention ***Camel case***.

Voici un exemple de déclaration d'une constante, avec cette norme :

```C
const int MACONSTANTE = 5 ;

#define UNEAUTRECONSTANTE (10);
```

## La déclaration de fonctions :

La déclaration de fonction s'aligne sur la même convention que la déclaration de variables. En effet, le ***Camel case*** sera, pour ce type de déclaration, la convention utilisée. Il est aussi important de souligner le fait que les paramètres seront listés en respectant la langue française, c'est à dire, séparés par une virgule, elle même suivie d'un espace.

Voici des exemples sur lesquels se baser :

```C
int maFonctionTouteBelle(maVariable1, maVariable2) { }

double uneAutreFonction(double maLargeur, double maHauteur);
```

## Les commentaires :

Les commentaires devront respecter la norme imposée par *Doxygen*. Une explication de chaque fonctions devra être fait par un commentaire en Doxygen avant celle-ci. Concernant les commentaires entre les instructions du code, ils seront fait **avant l'instruction** et avec un double slash, ou un slash + étoile si ceux-ci sont conséquents.

Des exemples à suivre :

```C
/**
 * \fn Str_t* strNew(const char* unParametre)
 * \brief Fonction de création d'une nouvelle d'un nouveau Str_t.
 * \param unParametre Chaîne à stocker dans l'objet Str_t, ne peut être NULL.
 * \return Str_t nouvellement allouée d'un objet de type Str_t ou NULL.
 */
 Str_t* strNew(const char* unParametre);

//On initialise maVariable à la valeur 7
int maVariable = 7;
/*On initialise le pointeur vers maVariable
en utilisant son adresse */
int* pointeurMaVariable = &maVariable;
```

## Les raccourcis de codage :

Certains raccourcis de codage voient leur utilisation autorisée. On pourra nottament utiliser les instructions du type *i++* ou encore les clauses *if* sans utiliser de *=* avec des données de type *bool*. Cependant les opérateurs ternaires conditionnels ne seront pas utilisés, mais on essayera de réduire au minimum les conditions de la clause *if* pour éviter toutes redondances.

Illustration par des exemples :

```C
int i = 0;
i++;

bool monTest = true;
if(monTest)
{
    puts("C'est vrai");
}
```
