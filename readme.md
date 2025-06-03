
Objectif Minishell

- recuperer des commandes terminal, les interpreter, executer par le systeme d'explotation



**Parsing**
- BUT : 
- recuperation des commandes
- interpretations des operateurs (redirections, pipe, $...)


## Tokenisation: Haby
le shell decoupe une ligne de commande en unites elementaire (token) pour analyser et executer


## Word Splitting : shell divise le resultat des expansions en mots distincts en respectant les regles d'IFS

## Variable Expansion 
Identifie variables commencant par $ et les remplace par leur valeur d'environnements

## Variable speciale 
$?

Exemple : 
	VAR="Hello world"
echo $VAR  # → echo Hello world (deux arguments distincts)
echo "$VAR"  # → echo "Hello world" (un seul argument)	

Execution shell parent, (gestion interface principale(prompt, lecture des commandes, historique))

## Tokenisation
... (rempli les étapes à faire)

## Shell Basis
- [x] Readline [mregnaut](https://profile.intra.42.fr/users/mregnaut)
- [ ] History [hadia](https://profile.intra.42.fr/users/hadia)
- [ ] Environment variables interpretation [hadia](https://profile.intra.42.fr/users/hadia)
- [ ] Environment variables setter [hadia](https://profile.intra.42.fr/users/hadia)



## External execution [mregnaut](https://profile.intra.42.fr/users/mregnaut)
- [ ] anything already in $PATH
- [ ] from absolute path

## Built-in commands [mregnaut](https://profile.intra.42.fr/users/mregnaut)

- [ ] echo (-e)
- [ ] cd
- [ ] pwd
- [ ] export
- [ ] unset
- [ ] env
- [ ] exit

https://cdn.intra.42.fr/pdf/pdf/146856/en.subject.pdf

## execution


Trouver le path de la commande a executer
 si la command n'est pas trouve "command not found"


 >> > << < tcsetattr

## historique des commands

reproduire ~ cat .bash_history
history > history,txt 
