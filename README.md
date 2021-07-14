# RT
Another damned pixel matrice. Again.

<h1>Features :</h1>

<h2> Basics </h2>

- [ ] Architecture MLX

- [ ] Architecture host OpenCL

- [ ] Kernel raycaster (Raytracer core shader)

- [ ] Luminosite, Lumiere ambiante

- [ ] Objets mathématiques obligatoires (plan, cone, cylindre, sphère)

- [ ] Multi-Spots, Ombres

- [ ] Brillance

- [ ] Fichier scène (résolution, camera, objets, fichiers ressources, .obj, .mtl...)

- [ ] Parsing de fichiers .obj et .mtl

<h2> Mandatory </h2>

- [ ] Transparence

- [ ] Modification de l’ombre selon la transparence des objets

- [ ] Réflexion

- [ ] Réfraction

- [ ] Textures

<h2> Bonus </h2>

- [ ] Interface utilisateur

- [ ] FPS : Mode d'exploration en temps reel de la scene en FPS

- [ ] Image / Screenshot : Rendu d'une image a partir d'une scene et d'un fichier
de config. (resolution, position et inclinaison de la camera, path vers le fichier)

- [ ] Video : Rendu d'une video a partir d'une scene et d'un fichier d'animation.
(resolution, fps, position et inclinaison de la camera, mouvements / rotation de meshs,
mouvements / rotation de camera, path vers le fichier video, path vers le fichier scene)

- [ ] Anti-aliasings (Plusieurs eventuellement)

- [ ] Subdivision de l'espace pour les scenes .obj

- [ ] Voxelisation
Lancer un rayon pour plusieurs pixels pour diviser la quantite de calcul (par 4, 9, 16, 25)

- [ ] Move bluring
Voxelisation plus forte sur les cotes de l'image pendant le mouvements, a ajuster
avec un ratio.

<h1> Questions </h1>
gedemais : Le sujet demande la possibilité de manipulations de la scene en live. Quel interface utiliser entre l'user et le model ?

<h1> Development & GitFlow </h1>
Notre méthode de développement actuelle est inspirée de la méthode agile.

L'intégration des features se fait de manière cyclique pour séparer puis subdiviser la charge de travail.

Au début d'un cycle, ou "sprint" classique, chaque développeur s'attribue une liste de taches à faire en
s'adaptant aux contrats d'interface établis au début du projet. La durée de chaque cycle est a adapter en
fonction du ressenti de l'équipe (1 - 4 semaines).

![Screen Shot 2021-07-03 at 7 30 37 PM](https://user-images.githubusercontent.com/44742651/124362450-278f1d00-dc35-11eb-8ae2-98190400a245.png)

Le gitflow adopté comporte deux branches principales, la branche master, et la branche qa (quality assurance),
qui servira de tampon pour les tests et la combinaison de features. A chaque cycle, les devs vont créer des
branches secondaires pour chacune des taches qu'ils se sont attribués, puis développer et tester leur code
(tests unitaires) avant de le merge avec la branche qa. Une fois toutes les branches secondaires merge a qa,
on peut s'assurer du bon fonctionnement global du code intégré, puis merge qa et master. Le cycle s'achève
et un autre commence, après un meeting et une éventuelle pause.

<h2>Nomenclature Git</h2>

- Branches secondaires (persos) : nom-cycle-nom_de_la_tache (ex : gedemais-00-setup_OpenCL)

<h1> Liens utiles </h1>

Raytracing course :
https://www.scratchapixel.com/lessons/3d-basic-rendering/introduction-to-ray-tracing

Anti-aliasing algorithm choice :
https://www.malekal.com/quest-ce-que-anti-aliasing-et-anticrenelage-fxaa-smaa-msaa-ssaa-txaa/#FXAA_Fast_Approximate_Anti-Aliasing
