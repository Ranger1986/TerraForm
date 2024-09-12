# HAI911I - TP1 Prise en main de Qt OpenGL

Noura Faraj : noura.faraj@umontpellier.fr

## Objectif

Le but de ce premier TP est de réaliser une application Qt OpenGL. Dans ce travail,
chaque étudiant aura pour but de réaliser différentes tâches :

- Ajouter des slots et des signaux pour faire communiquer des classes
- Charger un maillage et l’afficher

Vous devrez rendre un compte rendu ainsi que votre code source sur le moddle, le TP
doit compiler dans la salle de TP. 

## Framework

Pour nos différents TP, nous allons utiliser le Framework Qt. Il s’agit d’un Framework
spécialisé dans les UI. Nous nous servirons principalement de ce Framework afin de
gérer les IO de notre application.

## L’application Qt
Afin de vous familiariser avec Qt et de simplifier votre développement, nous vous
recommandons d’utiliser l’IDE Qt Creator. Pour vos premiers pas, avec QtOpenGL, votre
travail sera d’étudier l’affichage d’une forme en OpenGL avec Qt et d’enrichir
l’application.

## Question 1
Analyser le code et la structure du programme. Familiarisez avec l’application Qt. Vous
constatez qu’il y a des sliders à droite. Faites en sorte que les changements de sliders
fassent tourner l’objet en ajoutant des signaux et des slots et en les connectant. Faites en
sorte que les sliders soient mis à jour lorsque l’utilisateur fait tourner l’objet.

## Question 2
Créer une classe maillage. Y ajouter un vertex buffer et un index buffer. Remplir les
buffers pour créer un cube (maillage indexé). Affichez le.

## Question 3
Ajouter un item au menu pour charger un maillage. Mettre la géomét

## Ressources:

Tutoriel: http://guillaume.belz.free.fr/doku.php?id=qt_opengl_-_utilisation_du_pipeline_programmable

Slider: https://doc.qt.io/qt-6/qslider.html

Signal et slots: https://doc.qt.io/qt-6/signalsandslots.html
