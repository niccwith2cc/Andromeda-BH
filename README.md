# Project 49: Andromeda

# Barnes Hut Algorithm

## Why Do We Need It?

When simulating a rather large data points in space, such as the many celestial bodies that are found in a singular galaxy. Calculating the force of gravity of that each body exerts on the other, over a specified time interval with a small time step, can lead to a very high computational intense calculations. Hence, the Barnes Hut Algorithm takes some assumptions about the celestial bodies according to their relative positions to each other and their placement on the overall 3D space and decreases the amount of calculations to do. If celestial bodies are sufficiently close to one another, they are approximated as one body with its position being the center of mass of the cluster of bodies. Ofcourse, this does come with some accuracy issues as the algorithm only approximates the values and does not completely calculate them. However, the theoretical dimensional savings go from O(n^2) to O(n log(n)).

## How It Works?

Constructing the Tree:
The Algorithm takes a 3D cubic space with (x, y, z) coordinates and splits it into 8 octants of equal lengths, this corresponds to an octree of a main root with 8 children nodes. As the bodies are inserted into the root of the tree, they are placed into each octant depending on their position, however, no bodies can be within the same octant, in the octree meaning no bodies can exist within the same node. Hence if two or more bodies are close to each other the algorithm will continue to split each octant into another separate octant until the bodies are no longer in the same octant. In terms of the octree, each time a body is inserted into an already filled node, the occupied node will become a parent node with eight accompagning children node and the two or more bodies will be inserted into their respective nodes depending on their position. The algorithm will run this until no two bodies are in the same octant.

Computing:
Once the tree is constructed and all the bodies are in their respective octants. The algorithm will now determine how to group sufficiently close bodies, this will create temporary bodies with a center of mass that can be used to compute the forces acting on bodies in neighboring octants.

But how does one define sufficiently close enough?
a calculated ratio of s/d will be considered. S is the length of the octant of the internal node, D is the distance between the body and the node's center of mass. This ratio is compared with the threshold value θ, which indicates how fast the algorithm computes but also how inaccurate it may be. a θ = 0, the algorithm receeds to the brute force method as if the algorithm does nothing. If s/d > θ then the body is still sufficiently close enough to the center of mass, and more recursions are needed to divide the octant into its own grid.

## Authors and acknowledgment
Alexandra Apostolidou, Nicolas Hanna.

Acknowledgment to: 
http://arborjs.org/docs/barnes-hut

https://en.wikipedia.org/wiki/Barnes%E2%80%93Hut_simulation

## Gitlab reminders

- [ ] [Create](https://docs.gitlab.com/ee/user/project/repository/web_editor.html#create-a-file) or [upload](https://docs.gitlab.com/ee/user/project/repository/web_editor.html#upload-a-file) files
- [ ] [Add files using the command line](https://docs.gitlab.com/ee/gitlab-basics/add-file.html#add-a-file-using-the-command-line) or push an existing Git repository with the following command:

```
cd existing_repo
git remote add origin https://gitlab.lrz.de/advprog2023/49.git
git branch -M main
git push -uf origin main
```
