# Installation de inspircd 2.0 sous Ubuntu

## Désinstaller la version précédente

Si inspircd est déjà installé, l'enlever (cela supprimera la config actuelle, qui n'est pas compatible) :
<code>$ sudo apt-get purge inspircd</code>

## Ajout du dépôt

<code>$ sudo apt-add-repository ppa:dns/irc</code>
Et faire Entrée quand il le demande.

Puis :
<code>$ sudo apt-get update</code>

## Installation

Comme d'hab :
<code>$ sudo apt-get install inspircd</code>

## Configuration

Éditer /etc/default/inspircd et mettre la variable à 1.

Remplacer le fichier /etc/inspircd/inspircd.conf par celui présent dans ce dossier (il appartient à root).