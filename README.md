# AVL Tree Main Root Rotations

## Description

In this project, we explore a different approach to performing AVL Tree rotations. Traditional AVL Tree rotations focus on the critical node and perform rotations with the critical node as the new root. In this project, we deviate from the traditional approach and perform rotations with the main root as the critical node, even if it's not the actual critical node.

Here's the approach we take:

1. We start by inserting a node into the AVL tree.
2. Next, we calculate the heights of the nodes that were modified due to the insertion.
3. We then check the balance factor of the main root of the tree.
4. If the balance factor of the main root is either -2 or 2, we perform rotations at the main root based on the balance factors. This is where you'll need to provide rotation algorithms.
