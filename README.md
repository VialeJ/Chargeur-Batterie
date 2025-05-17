# Développement d'un chargeur de batterie

## 1 Cellule entre 5 et 7 Ah
- Posibilité de pouvoir gérer deux cellules dans un second temps

## Différents types de batterie 
- Li-ion (3.6V nom.)
- LiPo (3.7V nom.)
- LiFePO4 (3.2V nom.)

## Systèmes de protection
Protège la cellule contre les surtensions, les soustensions et la chaleur
(Les cellules li-ion utilisées dans ce projet contiennent déjà un BMS)

## Fuel gauge en I2C
Permet la communication des informations de charge et de décharge à un MCU

## 3 types d'alimentation possible:
- USB (5V)
- 12V
- Depuis un panneau solaire 26V (on prendra un Vin max de 28V)

## Package des composants
- CMS
- Eviter un boitier BGA

## Courant de charge de 1A max

## Obtenir le coût le plus faible
Le but est de pouvoir rendre le système reproductible 

## Références des composants
- Ajout de la Datashette des composants utilisés dans le repo git

## Travail à faire :
- Etudier circuits intégrés existants pour la cherge et la fuel gauge et choisir
- INA219 : Mesure de tension, courant et puissance
- Documentation
