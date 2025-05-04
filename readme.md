
Objectif Minishell

- recuperer des commandes terminal, les interpreter, executer par le systeme d'explotation



**Parsing**
- BUT : 
- recuperation des commandes
- interpretations des operateurs (redirections, pipe, $...)


# Tokenisation
le shell decoupe une ligne de commande en unites elementaire (token) pour analyser et executer


## Word Splitting : shell divise le resultat des expansions en mots distincts en respectant les regles d'IFS

Exemple : 
	VAR="Hello world"
echo $VAR  # → echo Hello world (deux arguments distincts)
echo "$VAR"  # → echo "Hello world" (un seul argument)	

Execution shell parent, (gestion interface principale(prompt, lecture des commandes, historique))
- Execution Commands interne : aller chercher dans le shell (cd, echo,...) Haby


- interpretation des commandes 

expansions des variables ($, $?)
tokenisation : Haby
shell enfant Mattis

- redirection entre/sortit
commande externe : creation de processus enfant : Mattis.
